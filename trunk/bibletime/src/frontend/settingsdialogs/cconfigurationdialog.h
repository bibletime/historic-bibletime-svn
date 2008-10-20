//
// C++ Interface: cconfigurationdialog
//
// Description: BibleTime Configuration dialog.
//
//
// Author: The BibleTime team <info@bibletime.info>, (C) 1999-2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef CCONFIGURATIONDIALOG_H
#define CCONFIGURATIONDIALOG_H

//#include "util/cpointers.h"

#include "frontend/bookshelfmanager/btconfigdialog.h"

class KActionCollection;
class QWidget;
class QAbstractButton;

class CDisplaySettingsPage;
class CSwordSettingsPage;
class CLanguageSettingsPage;
class CAcceleratorSettingsPage;

class CConfigurationDialog : public BtConfigDialog {
	Q_OBJECT

public:
	CConfigurationDialog(QWidget *parent, KActionCollection* actionCollection);
	virtual ~CConfigurationDialog();
protected slots:
	void slotButtonClicked(QAbstractButton *);
private:
	KActionCollection* m_actionCollection;
	CDisplaySettingsPage* m_displayPage;
	CSwordSettingsPage* m_swordPage;
	CAcceleratorSettingsPage* m_acceleratorsPage;
	CLanguageSettingsPage* m_languagesPage;

signals:
	void signalSettingsChanged();
};


#endif
