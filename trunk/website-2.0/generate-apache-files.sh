#!/bin/bash
# This script generates the apachage files are necessary so that a request to /index.html 
# returns the index.html in the langugage prefered by the old user.

### This script is automatically called by generate-website.sh .

. ./languages.conf

function resetFiles {
	rm -f website-generated/*.var
}

function appendLanguageRewrites {
	lang=$1
	files=$(cd website-generated/en; ls *.html)

	for f in $files; do \
		target="website-generated/$(echo $f | sed -e s/\.html/.var/)"
		echo -e "URI: $lang/$f" >> $target
		echo -e "Content-type: text/html" >> $target
		echo -e "Content-language: $lang\n" >> $target
	done;	
}

function appendDefaultRewrites {
	for f in $(cd website-generated/en; ls *.html); do \
		target="website-generated/$(echo $f | sed -e s/\.html/.var/)"
		echo -e "URI: en/$f" >> $target
		echo -e "Content-type: text/html" >> $target
	done;
}

resetFiles

for lang in en $BT_LANG_ALL; do \
	appendLanguageRewrites $lang 
done;

appendDefaultRewrites

