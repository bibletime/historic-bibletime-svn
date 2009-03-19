#!/bin/bash
. ./languages.conf
. ./local.conf

#Appends the postats data for one language to a file
#param 1  == pofile, 2 == language name, 3 == target
function appendPoStats {
	pofile=$1
	lang=$2
	target=$3
	
	perl postats/make_postats.pl $pofile $lang >> $target
}

### Website
echo "Website po statistics ..."

echo -n "" > website-generated/postats/website_stats.txt
for lang in $BT_LANG_ALL; do \
	appendPoStats i18n/$lang.po $lang website-generated/postats/website_stats.txt; \
done;

### BibleStudy stats
echo "Bible study po statistics ..."
dir="$BIBLETIME_SVN_DIR/i18n/howto"
stats=website-generated/postats/howto_stats.txt
echo -n "" > $stats

for pofile in $(ls $dir/*.po); do \
	lang=$(basename $pofile | sed -e s/howto\-// | sed -e s/\.po//); \
	appendPoStats $pofile $lang $stats; \
done;


### Handbook stats
echo "Handbook po statistics ..."
dir="$BIBLETIME_SVN_DIR/i18n/handbook"
stats=website-generated/postats/handbook_stats.txt
echo -n "" > $stats

for pofile in $(ls $dir/*.po); do \
	lang=$(basename $pofile | sed -e s/handbook\-// | sed -e s/\.po//); \
	appendPoStats $pofile $lang $stats; \
done;
	
echo "Updating program i18n statistics ..."
D=$BIBLETIME_SVN_DIR/i18n/messages
stats=website-generated/postats/messages_stats.txt

(cd $D && for f in *.ts; do \
  test "$f" = "bibletime_ui.ts" || (echo -n "$f " | sed -e "s/.ts//" | sed -e "s/bibletime_ui_//"; \
  echo -n -e "\t"; \
  grep -c -E '<translation>[^<]+</translation>' $f | tr '\n' '\t'; \
  grep -c -E '<translation[^>]*></translation>' $f | tr '\n' '\t'; \
  grep -c -E '<translation type="unfinished">[^<]+</translation>' $f;); \
done;) > $stats

