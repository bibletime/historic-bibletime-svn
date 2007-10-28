/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2007 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/



#include "ckeychooser.h"
#include "ckeychooser.moc"

#include "backend/drivers/cswordmoduleinfo.h"
#include "backend/drivers/cswordbiblemoduleinfo.h"
#include "backend/drivers/cswordcommentarymoduleinfo.h"
#include "backend/drivers/cswordlexiconmoduleinfo.h"

#include "backend/keys/cswordkey.h"

#include "bthistory.h"
#include "clexiconkeychooser.h"
#include "cbiblekeychooser.h"
#include "cbookkeychooser.h"

#include <QAction>
#include <QDebug>

CKeyChooser::CKeyChooser(ListCSwordModuleInfo, CSwordKey *, QWidget *parent)
	: QWidget(parent),
	m_history(0)
{
	m_history = new BTHistory(this);
}

CKeyChooser::~CKeyChooser() {}

CKeyChooser* CKeyChooser::createInstance(ListCSwordModuleInfo modules, CSwordKey *key, QWidget *parent) {
	if (!modules.count()) {
		return 0;
	}

	CKeyChooser* ck = 0;
	switch ( modules.first()->type() ) {
		case CSwordModuleInfo::Commentary:  //Bibles and commentaries use the same key chooser
		case CSwordModuleInfo::Bible:
			return new CBibleKeyChooser(modules,key,parent);
			break;
		case CSwordModuleInfo::Lexicon:
			return new CLexiconKeyChooser(modules,key,parent);
		case CSwordModuleInfo::GenericBook:
			return new CBookKeyChooser(modules,key,parent);
		default:
			return 0;
	}
}


BTHistory* CKeyChooser::history()
{
	return m_history;
}

