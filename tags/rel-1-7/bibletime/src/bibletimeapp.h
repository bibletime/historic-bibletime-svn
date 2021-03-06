/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2008 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#ifndef BIBLETIMEAPP_H
#define BIBLETIMEAPP_H

//BibleTime
#include "util/cpointers.h"

//#include <QApplication>
//KDE
#include <kapplication.h>

/** The BibleTimeApp class is used to clean up all instances of the backend and to delete all created module objects.
  * @author The BibleTime team
  */
class BibleTimeApp : public KApplication, public CPointers {
public:
	BibleTimeApp();
//	BibleTimeApp( int & argc, char ** argv );#for QApplication
	virtual ~BibleTimeApp();

protected:
//	void initDCOP();
};

#endif
