#!/bin/bash

echo "\nTrying to compile and run the import tool\n";
qmake import_db.pro && make && ./import_db;

echo "\nThe schema and mapping databases are in db/, if everything went ok.\n";
echo "Cleaning up."
make distclean;
