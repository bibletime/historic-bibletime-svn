/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2008 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#ifndef CMDIAREA_H
#define CMDIAREA_H

#include "util/cpointers.h"
#include "backend/drivers/cswordmoduleinfo.h"

//Qt includes
#include <QMdiArea>
#include <QList>

class CSwordModuleInfo;
class CSwordKey;
class QEvent;
class QResizeEvent;
class QChildEvent;
class QMdiSubWindow;

/** The MDI widget we use in BibleTime.
 * Enhances QMdiArea.
 */
class CMDIArea : public QMdiArea {
	Q_OBJECT

public:
	/**
	* The options you can set for this widget.
	*/
	enum MDIOption {
		autoTileVertical,
		autoTileHorizontal,
		autoCascade,
		Nothing
	};
	CMDIArea(QWidget *parent);
	/**
	* Enable / disable autoCascading
	*/
	void setGUIOption( const MDIOption& newOption );
	/**
	*/
	void emitWindowCaptionChanged();
	/**
	* Forces an update of the currently chosen window arrangement.
	*/
	void triggerWindowUpdate();
	QList<QMdiSubWindow*> usableWindowList();

public slots:
	/**
	* Called whan a client window was activated
	*/
	void slotClientActivated(QMdiSubWindow* client);
	/**
	* Deletes all the presenters in the MDI area.
	*/
	void deleteAll();
	/** Our own cascade version which, if only one window is left, shows this maximized.
	* Also necessary for autoCasacde feature
	*/
	void myCascade();
	/** Our own cascade version which, if only one window is left, shows this maximized.
	* Also necessary for autoTile feature
	*/
	void myTileVertical();
	/** Horizontal tile function
	* This function was taken from Qt's MDI example.
	*/
	void myTileHorizontal();
	/**
	 * Emits the signal to create a new display window in the MDI area.
	 */
	inline void emitCreateDisplayWindow( ListCSwordModuleInfo modules, const QString keyName );

signals: // Signals
	/**
	* Emits a signal to set the acption of the toplevel widget.
	*/
	void sigSetToplevelCaption(const QString&);
	/**
	 */
	void createReadDisplayWindow(ListCSwordModuleInfo modules, const QString& keyName);

private:
	/**
	* Used to make use of the fixedGUIOption part.
	*/
	virtual void childEvent(QChildEvent * e);
	/**
	* Reimplementation
	*/
	virtual void resizeEvent(QResizeEvent* e);

	bool eventFilter( QObject *o, QEvent *e );
	/**
	 */
	MDIOption m_guiOption;
};

/** Emits the signal to create a new display window in the MDI area. */
inline void CMDIArea::emitCreateDisplayWindow( ListCSwordModuleInfo modules, const QString keyName ) {
	emit createReadDisplayWindow(modules, keyName);
}


#endif
