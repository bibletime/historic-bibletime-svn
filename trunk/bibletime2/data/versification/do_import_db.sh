#!/bin/bash

echo -e "\nUnpacking the source files\n"
(cd ccel; gunzip *gz);

echo -e "\nTrying to compile and run the import tool\n";
qmake import_db.pro && make && ./import_db;

echo -e "\nThe schema and mapping databases are in db/, if everything went ok.\n";
echo -e "Cleaning up."
make distclean;
