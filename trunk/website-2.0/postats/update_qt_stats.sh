#!/bin/sh
D=../../../bibletime-2.0/i18n/messages

(cd $D && for f in *.ts; do test "$f" = "bibletime_ui.ts" || (echo -n "$f " | sed -e "s/.ts//" | sed -e "s/bibletime_ui_//"; echo -n "\t"; grep -c -E '<translation>[^<]+</translation>' $f | tr '\n' '\t'; grep -c -E '<translation[^>]*></translation>' $f | tr '\n' '\t'; grep -c -E '<translation type="unfinished">[^<]+</translation>' $f;); done;) > messages_stats.txt
