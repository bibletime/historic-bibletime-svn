/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2008 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

//BibleTime includes
#include "btinstallmgr.h"

#include "frontend/bookshelfmanager/instbackend.h"
#include "backend/managers/cswordbackend.h"

//Qt includes
#include <QObject>
#include <QList>
#include <QString>
#include <QStringList>

#include <QDebug>

//sword includes
#include <installmgr.h>
#include <ftptrans.h>

using namespace sword;


BtInstallMgr::BtInstallMgr()
	: InstallMgr(instbackend::configPath().toLatin1(), this),
	m_firstCallOfPreStatus(true)
{ //use this class also as status reporter
	qDebug("BtInstallMgr::BtInstallMgr");
	this->setFTPPassive(true);
#ifdef SWORD_INTERNET_WARNING
	// We should use this getter/setter to show a dialog before
	// the user opens network connection for the first time and
	// get rid of the static warning.
	setUserDisclaimerConfirmed(true);
#endif
}

BtInstallMgr::~BtInstallMgr() {
	//doesn't really help because it only sets a flag
	terminate(); //make sure to close the connection
}

void BtInstallMgr::statusUpdate(double dltotal, double dlnow)
{
	//qDebug("BtInstallMgr::statusUpdate");
	if (dlnow > dltotal)
		dlnow = dltotal;

	int totalPercent = (int)((float)(dlnow + m_completedBytes) / (float)(m_totalBytes) * 100.0);

	if (totalPercent > 100) {
		totalPercent = 100;
	}
	else if (totalPercent < 0) {
		totalPercent = 0;
	}

	int filePercent  = (int)((float)(dlnow) / (float)(dltotal+1) * 100.0);
	if (filePercent > 100) {
		filePercent = 100;
	}
	else if (filePercent < 0) {
		filePercent = 0;
	}
	//qApp->processEvents();
	//qDebug() << "status: total"<<totalPercent<<"file"<<filePercent;
	emit percentCompleted(totalPercent, filePercent);
}


void BtInstallMgr::preStatus(long totalBytes, long completedBytes, const char* message)
{
	if (m_firstCallOfPreStatus) {
		m_firstCallOfPreStatus = false;
		emit downloadStarted();
	}
	qDebug() << "BtInstallMgr::preStatus:" << (int)totalBytes << "/" << (int)completedBytes << QString(message);
	m_completedBytes = completedBytes;
	m_totalBytes = (totalBytes > 0) ? totalBytes : 1; //avoid division by zero
}
