/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2008 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#include "cmoduleindexdialog.h"
#include "cmoduleindexdialog.moc"

#include <boost/scoped_ptr.hpp>

//Qt includes
#include <QString>
#include <QProgressDialog>
#include <QDebug>
#include <QApplication>

//KDE includes



CModuleIndexDialog* CModuleIndexDialog::getInstance() {
	qDebug("CModuleIndexDialog::getInstance");
	static CModuleIndexDialog* instance = 0;
	if (instance == 0) {
		instance = new CModuleIndexDialog();
	}
	qDebug("CModuleIndexDialog::getInstance end");
	return instance;
}

void CModuleIndexDialog::indexAllModules( const ListCSwordModuleInfo& modules )
{
	qDebug("indexAllModules");
	static bool indexing = false;
	if (!indexing) {
		indexing = true;
		qDebug() << &modules << modules << modules.count();
		//qDebug() << modules[0]->name();
		if (modules.count() < 1) {
			return;
		}
		
		m_currentModuleIndex = 0;
		m_progress = new QProgressDialog(QString(""), 0, 0, modules.count()*100);
		m_progress->setWindowModality(Qt::WindowModal); // not useful actually, should have parent for this
		m_progress->show();
		m_progress->raise();
		
		foreach (CSwordModuleInfo* info, modules) {
			info->connectIndexingFinished(this, SLOT(slotFinished()));
			info->connectIndexingProgress(this, SLOT(slotModuleProgress(int)) );
			QString modname(info->name());
			const QString labelText = tr("Creating index for work: ").append(modname);
			m_progress->setLabelText(labelText);
			info->buildIndex(); //waits until this module is finished
	
			m_currentModuleIndex++;
			info->disconnectIndexingSignals(this);
		}
	
		delete m_progress;
		m_progress = 0;
		indexing = false;
	}
}

void CModuleIndexDialog::indexUnindexedModules( const ListCSwordModuleInfo& modules ) {
// 	qDebug("indexUnindexedModules");
	//qDebug() << modules << modules.count();
	ListCSwordModuleInfo unindexedMods;
	
	ListCSwordModuleInfo::const_iterator end_it = modules.end();
	for( ListCSwordModuleInfo::const_iterator it = modules.begin(); it != end_it; ++it) {
		if ((*it)->hasIndex()) {
			continue;
		}

		unindexedMods << (*it);
	}
	//qDebug() << unindexedMods << unindexedMods.count();
	indexAllModules(unindexedMods);
}

void CModuleIndexDialog::slotModuleProgress( int percentage ) {
	m_progress->setValue(m_currentModuleIndex * 100 + percentage);
	qApp->processEvents();
}

void CModuleIndexDialog::slotFinished( ) {
	m_progress->setValue(m_progress->maximum());
	qApp->processEvents();
}
