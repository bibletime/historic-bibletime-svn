#/bin/sh

DIR=~/Projekte/C++/BibleTime/bibletime-2.0/i18n/messages;
cd $DIR

(for f in *.ts; do echo -n "$f " | sed -e "s/\.ts//"; grep -c -E '<translation type=\"unfinished\">|<translation></translation>' $f | tr '\n' ' '; echo -n " 0  "; grep -c -E '<translation>.+</translation>' $f; done;) > 
