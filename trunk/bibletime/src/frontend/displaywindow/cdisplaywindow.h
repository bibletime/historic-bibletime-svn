/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2008 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/



#ifndef CDISPLAYWINDOW_H
#define CDISPLAYWINDOW_H

//BibleTime includes
#include "util/cpointers.h"

#include "backend/drivers/cswordmoduleinfo.h"
#include "backend/managers/cswordbackend.h"
#include "backend/keys/cswordkey.h"
#include "backend/keys/cswordversekey.h"
#include "backend/keys/cswordtreekey.h"
#include "backend/keys/cswordldkey.h"

#include "frontend/profile/cprofilewindow.h"

//Qt includes
#include <QWidget>
#include <QCloseEvent>
#include <QStringList>

//#include <QMdiSubWindow>
#include <QMainWindow>


//Forward declarations
class CMDIArea;
class CReadWindow;
class CWriteWindow;
class CDisplaySettingsButton;
class CDisplay;
class CKeyChooser;
class CModuleChooserBar;

class KToolBar;
class QMenu;
class KActionCollection;

/** The base class for all display windows of BibleTime.
  * @author The BibleTime team
  */

class CDisplayWindow : public QMainWindow, public CPointers  {
	Q_OBJECT
public:
	enum WriteWindowType {
		HTMLWindow = 1,
		PlainTextWindow = 2
	};

	/**
	* Insert the keyboard accelerators of this window into the given KAccel object.
	*/
	//   static void insertKeyboardActions( KAccel* const accel );
	static void insertKeyboardActions( KActionCollection* const a );

	CMDIArea* const mdi() const;
	/**
	* Returns the right window caption.
	*/
	const QString windowCaption();
	/**
	* Returns the used modules as a QPtrList
	*/
	ListCSwordModuleInfo modules();
	/**
	* Store the settings of this window in the given CProfileWindow object.
	*/
	virtual void storeProfileSettings( Profile::CProfileWindow* profileWindow ) = 0;
	/**
	* Store the settings of this window in the given profile window.
	*/
	virtual void applyProfileSettings( Profile::CProfileWindow* profileWindow ) = 0;
	/**
	* Set the window caption.
	*/
	virtual void setCaption( const QString& );
	/**
	* Sets the new filter options of this window.
	*/
	void setFilterOptions( CSwordBackend::FilterOptions& filterOptions );
	/**
	* Sets the new display options for this window.
	*/
	void setDisplayOptions( const CSwordBackend::DisplayOptions& displayOptions );
	/**
	* Returns the display options used by this display window.
	*/
	CSwordBackend::DisplayOptions& displayOptions();
	/**
	* Returns the filter options used by this window.
	*/
	CSwordBackend::FilterOptions& filterOptions();
	/**
	* Set the ready status
	*/
	void setReady( const bool& ready );
	/**
	* Returns true if the widget is ready for use.
	*/
	const bool isReady() const;
	/**
	* Returns true if the window may be closed.
	*/
	virtual bool queryClose();
	/**
	* Returns the keychooser widget of this display window.
	*/
	CKeyChooser* const keyChooser() const;
	/**
	* Sets the new sword key.
	*/
	void setKey( CSwordKey* key );
	/**
	* Returns the key of this display window.
	*/
	CSwordKey* key() const;
	/**
	* Initialize the window. Call this method from the outside, because calling this in the constructor is not possible!
	*/
	virtual const bool init();
	/**
	* Sets the main toolbar.
	*/
	void setMainToolBar( QToolBar* bar );
	/**
	* Sets the buttons toolbar.
	*/
	void setButtonsToolBar( QToolBar* bar );
	/**
	* Returns the main toolbar.
	*/
	QToolBar* const mainToolBar() const;
	/**
	* Returns the buttons toolbar.
	*/
	QToolBar* const buttonsToolBar() const;
	/**
	* Initialize the toolbars
	*/
	virtual void initToolbars() = 0;
	/**
	* Returns the display settings button
	*/
	CDisplaySettingsButton* const displaySettingsButton() const;
	/**
	* Sets the display settings button.
	*/
	void setDisplaySettingsButton( CDisplaySettingsButton* button );
	virtual void setupPopupMenu() = 0;
	/**
	* Returns the display widget used by this implementation of CDisplayWindow.
	*/
	virtual CDisplay* const displayWidget() const;
	/**
	* Sets the display widget used by this display window.
	*/
	virtual void setDisplayWidget( CDisplay* newDisplay );

	/** Returns whether syncs to the active window are allowed at this time for this display window
	* @return boolean value whether sync is allowed
	*/
	virtual const bool syncAllowed() const {
		return false;
	};

	KActionCollection* actionCollection();

public slots:
	/**
	* Lookup the specified key in the given module. If the module is not chosen withing
	* this display window create a new displaywindow with the right module in it.
	*/
	virtual void lookup( const QString& module, const QString& key );
	/**
	* Lookup the key in the chosen modules.
	*/
	virtual void lookup( const QString& key );
	/**
	* Refresh the settings of this window.
	*/
	virtual void reload();

protected:
	friend class CMDIArea;
	friend class CBibleReadWindow;

	CDisplayWindow(ListCSwordModuleInfo modules, CMDIArea* parent);
	virtual ~CDisplayWindow();
	/**
	* Initializes the intern keyboard actions.
	*/
	virtual void initActions();
	/**
	 Is called when this window gets the focus or looses the focus.
	*/
	virtual void windowActivated( const bool hasFocus );
	/**
	* Sets the keychooser widget for this display window.
	*/
	void setKeyChooser( CKeyChooser* ck );
	/**
	* Returns the module chooser bar.
	*/
	CModuleChooserBar* const moduleChooserBar() const;
	/**
	* Lookup the given key.
	*/
	virtual void lookup( CSwordKey* ) = 0;
	/**
	* Sets the module chooser bar.
	*/
	void setModuleChooserBar( CModuleChooserBar* bar );
	/**
	* Sets the modules.
	*/
	void setModules( const ListCSwordModuleInfo& modules );
	/**
	* Initializes the signal / slot connections of this display window.
	*/
	virtual void initConnections() = 0;
	/**
	* Initialize the view of this display window.
	*/
	virtual void initView() = 0;
	/**
	* Returns the installed popup menu.
	*/
	QMenu* const popup();
	virtual void closeEvent(QCloseEvent* e);

protected slots:
	virtual void modulesChanged();
	/**
	* Lookup the current key. Used to refresh the display.
	*/
	void lookup();
	virtual void updatePopupMenu();

	void slotSearchInModules();

	void printAll();
	void printAnchorWithText();


private:
	KActionCollection* m_actionCollection;
	CMDIArea* m_mdi;

	//we may only cache the module names bacause after a backend relaod the pointers are invalid!
	QStringList m_modules;

	CSwordBackend::FilterOptions m_filterOptions;
	CSwordBackend::DisplayOptions m_displayOptions;

	CDisplaySettingsButton* m_displaySettingsButton;
	CKeyChooser* m_keyChooser;
	CSwordKey* m_swordKey;
	bool m_isReady;
	CModuleChooserBar* m_moduleChooserBar;
	QToolBar* m_mainToolBar;
	QToolBar* m_buttonsToolBar;
	QMenu* m_popupMenu;
	CDisplay* m_displayWidget;
};

#endif
