#include <algorithm>
#include <cctype>
#include <functional>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

using std::string;

struct verse {
	verse(string b, string c, string v) : book_name(b), chapter_num(c), verse_num(v) {}
	string book_name, chapter_num, verse_num;
};

struct range {
	range(const verse& v) : lower(v), upper(v) {}
	range(const verse& l, const verse& u) : lower(l), upper(u) {}
	verse lower, upper;
};

/* tokenize_refs: this function takes a reference string, and tokenizes it. 
	Tokenizing rules are as follows:
	- whitespace separates into different tokens, but whitespace itself
		is never included in a token. The characters . and : are treated like whitespace
	- a non-number followed by a number is seperated into different tokens
		(but not a number followed by a non-number)
	- the characters ,;- are always placed in tokens of their own
		(and thus cause separation on either side)

e.g. "1 Peter1:5-8" -> "1","Peter","1",":","5","-","8"*/

void tokenize_refs(const string& ref, std::vector<string>& res)
{
	//find the first non-space character
	const string::const_iterator first = std::find_if(ref.begin(), ref.end(), isgraph);
	//if we didn't find anything, just return
	if (first == ref.end()) return;

	//the list of possible separators
	static const string sep = ",;:-. ";

	//if this is a separator character, add it as a token, and
	//recurse with the remaining substring (we know that this
	//character must not be a space from above)
	if(std::find(sep.begin(),sep.end(),*first) != sep.end()) {
		//things with no semantic meaning, which should be ignored
		static const string skip = ".:";
		if(std::find(skip.begin(),skip.end(),*first) == skip.end())
						res.push_back(string(1,*first));
		tokenize_refs(string(first+1,ref.end()),res);
		return;
	}
	
	//find the separator, or the end of the string
	const string::const_iterator end = std::find_first_of(first, ref.end(), sep.begin(), sep.end());
	
	//we still haven't satisfied the rule that if a non-number is
	//followed by a number, we have to treat that as a boundary.
	//Iterate over every digit in the substring we now have,
	//and if it is preceeded by a non-number, we have to stop there
	string::const_iterator last = std::find_if(first+1,end,isdigit);
	while(last != end && isdigit(*(last-1)))
		last = std::find_if(last+1,end,isdigit);
	
	//add this token
	res.push_back(string(first,last));

	//call the function again, with the remaining substring
	tokenize_refs(string(last,ref.end()),res);
}

bool is_separator(const string& str) {
	static const string sep[] = {",",";",":","-","v","ver","V","VER","Ver"};
	static const string* const begin = sep;
	static const string* const end = sep + sizeof(sep)/sizeof(*sep); //TODO: fix
	return std::find(begin,end,str) != end;
}

bool is_entity(const string& str) {
	return std::find_if(str.begin(),str.end(),isalnum) != str.end() && !is_separator(str);
}

bool is_roman(const string& str) {
	return str.find_first_not_of("ivxlIVXL") == string::npos;
}

bool is_number(const string& str) {
	return str.find_first_not_of("0123456789") == string::npos;
}

bool is_chapter_verse(const string& str) {
	return is_number(str) || is_roman(str);
}

bool is_word(const string& str) {
	return std::find_if(str.begin(),str.end(),isalpha) != str.end() && !is_roman(str) && !is_separator(str);
}

void get_verses(std::vector<string>::const_iterator start, std::vector<string>::const_iterator end,	verse& default_ref,	std::vector<std::pair<bool,verse> >& res)
{
	typedef std::vector<string> token_list;
	typedef token_list::const_iterator token_itor;
	token_itor first = std::find_if(start,end,is_entity);

	if (first == end) return;

	//find the end of this reference portion
	static const string ref_sep[] = {";",",","-"};
	static const int nref_sep = sizeof(ref_sep)/sizeof(*ref_sep);
	token_itor last = std::find_first_of(first,end,ref_sep,ref_sep+nref_sep);

	//try to find a book name. If we find a word, we assume everything before it is part of it (e.g. 1 Peter)
	const token_itor word = std::find_if(first,last,is_word);
	const token_itor end_book = (word != last) ? std::find_if(word+1,last,is_chapter_verse) : first;

	//ok, [first,end_book) now holds the book, we now want to find two numbers - the chapter and verse
	token_itor chap = std::find_if(end_book,last,is_chapter_verse);
	token_itor vers = std::find_if(chap+1,last,is_chapter_verse);

	//if there are more tokens before the separator, we might as well
	//leave the rest, and attempt to parse it, it might hold another reference
	if (vers < last) last = vers+1;
	
	//form the book, by joining the book tokens together, separate with spaces
	std::ostringstream book_stream;
	std::copy(first,end_book,std::ostream_iterator<string>(book_stream," "));
	string book_name = book_stream.str();
	if (book_name.empty()) 
	{
		book_name = default_ref.book_name;

		//if the verse could not be found, it should take priority over the chapter in terms of finding a match, so swap them
		//if however, we don't have a default for the verse, it means we are in a construction like John 3-8, and we are now
		//parsing the '8', in which case we shouldn't swap
		if (vers == last && default_ref.verse_num != "*") std::swap(chap,vers);
	} 
	else 
	{
		book_name.resize(book_name.size()-1); //cut off extra space at end
		default_ref.chapter_num = "*";
		default_ref.verse_num = "*";
	}
	
	//work out the chapter and verse, use default values if they
	//are not available
	const string chapter_num = chap != last ?*chap:default_ref.chapter_num;
	const string verse_num = vers != last ?*vers:default_ref.verse_num;
	
	//this is a range if the first value was a '-'
	const bool is_range = (*start == "-");
	
	verse new_verse(book_name,chapter_num,verse_num);
	res.push_back(std::make_pair(is_range,new_verse));
	get_verses(last,end,new_verse,res);
}

void parse_and_print(std::string input)
{
	std::vector<string> tokens;
	tokenize_refs(input,tokens);
	std::vector< std::pair<bool,verse> > verses;
	verse v("Genesis","1","1");
	get_verses(tokens.begin(),tokens.end(),v,verses);

	std::cout << "\"" << input << "\" -> ";
	for(std::vector<std::pair<bool,verse> >::const_iterator i = verses.begin(); i != verses.end(); ++i) 
	{
		std::cout << (i->first ? "-":",") << "\"" << i->second.book_name << "|" << i->second.chapter_num << "|" << i->second.verse_num << "\"";
	}
	std::cout << std::endl;
}

#include <QString>
#include <QRegExp>
#include <QStringList>
#include <QDebug>

void parse_and_print_qt(const QString& reflist)
{
	QRegExp toplevelSeparator;
	//If ":" is used to separate chapter and verse, then both ";" and "," 
	//can be used to separate ranges, if not, only ";" can be used
	if (reflist.contains(":")) 
		toplevelSeparator.setPattern(";|,");
	else 
		toplevelSeparator.setPattern(";");

	qDebug() << "splitting up toplevel Ranges in" << reflist;
	
	//Start by splitting up the toplevel ranges which are independent of each other
	// e.g. Gen 1,2; Mt 3,2 -> "Gen 1,2", "Mt 3,2"
	QStringList toplevelRanges = reflist.split( toplevelSeparator, QString::SkipEmptyParts);
	QStringList::const_iterator toplevelRange;
	for (toplevelRange = toplevelRanges.constBegin(); toplevelRange != toplevelRanges.constEnd(); ++toplevelRange)
	{
		qDebug() << "splitting up sublevel Ranges in" << *toplevelRange;
		//Now look for real indicated ranges and split them also
		// e.g. Gen 3,3-4,1 -> "Gen 3,3" "4,1"
		QStringList sublevelRanges = toplevelRange->split( "-", QString::SkipEmptyParts);
		QStringList::const_iterator sublevelRange;
		for (sublevelRange = sublevelRanges.constBegin(); sublevelRange != sublevelRanges.constEnd(); ++sublevelRange)
		{
			//TODO: Missing: Look for list items indicated by a ".", e.g. Gen 1,2.5
			//TODO: Implement in-range and general parsing context
			//TODO: Implement interpolation of missing information
			qDebug() << "parsing:" << *sublevelRange;
			//              book                        chapter            verse
			QRegExp rx("\\s*(\\d*[^0-9:,]+[^0-9:,\\s])?\\s*(\\d+)?\\s*[:,]?\\s*(\\d+)?\\s*"); 
			if (rx.indexIn(*sublevelRange) != -1)
			{
				qDebug() << "result:" << rx.cap(1) << " " << rx.cap(2) << "," << rx.cap(3);
			}
			else
				qDebug() << "error: no match found";
		}
	}
}
//zum Aufsplitten von .-Ranges (z.B. Gen 1,1.2-3;5,4.7) kann man "[.](?=[0-9]+)" nehmen


int main()
{
	//parse_and_print("Gen 1:2,2-4;Lev 3:5-7;4:4-5:4;Mt-Jn");
	parse_and_print_qt("Gen 1:23; Ex 2:3");
	parse_and_print_qt("1. Mose 3,25; 2 Mose 3,2-4; 5,4; 3; Gen");
} 
