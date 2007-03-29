/***************************************************************************
 *   Copyright (C) 2007 by the BibleTime team (info@bibletime.info         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

//QT includes
#include <QString>
#include <QtDebug>

#include "backend/osisparser.h"

int main(int argc, char *argv[])
{
	qDebug() << "\n\nHello world. This is BibleTime2. Almost. ;)\n\n\n";

	QString testReflist = "Gen 1:2,4-6;3-5;5:1-6:2;Lev 3:5-7;4:4-5:4;Mt 1-Jn";

	qDebug() << "Parser test: parsing " << testReflist;

	qDebug() << "Result: " << backend::parse_reflist(testReflist);

//	for (int i = 0; i<10000; i++)
//		backend::parse_reflist(testReflist);

	return EXIT_SUCCESS;
}
