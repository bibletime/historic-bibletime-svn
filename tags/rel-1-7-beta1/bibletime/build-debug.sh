# This little script creates a build/ directory, enters it
# and builds BibleTime there.
#
# BibleTime will be installed to build/install and can be run 
# just from there: ./build/install/bin/bibletime. It will find all
# required files.

#TMP FIX; expand path to let kde4-config be found
PATH="$PATH:/usr/lib/kde4/bin"

if [ ! -d build ]; then mkdir build; fi
cd build
cmake -D CMAKE_BUILD_TYPE=Debug -D CMAKE_INSTALL_PREFIX="install/" ..  || exit 1
make install || exit 1
cd ..

echo
echo "BibleTime has been installed to ./build/install."
echo "You can run it by typing ./build/install/bin/bibletime."
echo
