#!/bin/sh

(cd website-generated/postats && ./make_postats.pl)
(cd website-generated/postats && ./update_qt_stats.sh)
