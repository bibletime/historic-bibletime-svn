//
// C++ Interface: cconfigurationdialog
//
// Description: BibleTime Configuration dialog.
//
//
// Author: The BibleTime team <info@bibletime.info>, (C) 1999-2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef CCONFIGURATIONDIALOG_H
#define CCONFIGURATIONDIALOG_H

#include "util/cpointers.h"

#include <kpagedialog.h>

class KActionCollection;
class QWidget;
class KPageWidgetItem;

class CDisplaySettingsPage;
class CSwordSettingsPage;

class CConfigurationDialog : public KPageDialog, public CPointers  {
	Q_OBJECT

public:
	CConfigurationDialog(QWidget *parent, KActionCollection* actionCollection);
	virtual ~CConfigurationDialog();
protected slots:
	void slotOk();
	void slotApply();
private:
	KActionCollection* m_actionCollection;
	CDisplaySettingsPage* m_displayPage;
	CSwordSettingsPage* m_swordPage;
	//KPageWidgetItem* m_acceleratorsPage;
	//KPageWidgetItem* m_languagesPage;

signals:
	void signalSettingsChanged();
};


#endif