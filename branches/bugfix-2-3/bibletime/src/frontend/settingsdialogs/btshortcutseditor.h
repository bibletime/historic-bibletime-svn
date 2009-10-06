/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2009 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/


#ifndef BT_SHORTCUTS_EDITOR_H
#define BT_SHORTCUTS_EDITOR_H

#include <QWidget>

// forwards
class BtActionCollection;
class BtShortcutsEditorItem;
class BtShortcutsDialog;
class QPushButton;
class QRadioButton;
class QTableWidget;
class QLabel;

// This class is the table in the center of the Shortcuts page of the config dialog
class BtShortcutsEditor : public QWidget
{
	Q_OBJECT
public:
	BtShortcutsEditor(BtActionCollection* collection, QWidget* parent);
	BtShortcutsEditor(QWidget* parent);

	// saves shortcut keys into the QAction
	void commitChanges();

	// puts actions and shortcut keys into QTableWidget
	void addCollection(BtActionCollection* collection, const QString& title = QString());

	// clears any shortcut keys in the table matching the specified keys
	void clearConflictWithKeys(const QString& keys);

	// finds any shortcut keys in the table matching the specified keys - returns the Action Name for it.
	QString findConflictWithKeys(const QString& keys);

	// used by application to complete the keyChangeRequest signal
	// stores "keys" into the custom shortcuts dialog field
	void changeShortcutInDialog(const QString& keys);

signals:
	// make a keyChangeRequest back to the application
	void keyChangeRequest(BtShortcutsEditor*, const QString& keys);

private slots:

	// called when a different action name row is selected
	void changeRow(int row, int column);

	// called when the none radio button is clicked
	void noneButtonClicked(bool checked);

	// called when the default radio button is clicked
	void defaultButtonClicked(bool checked);

	// called when the custom radio button is clicked
	void customButtonClicked(bool checked);

	// makes the keyChangeRequest
	void makeKeyChangeRequest(const QString& keys);

private:
	
	// create the action and shortcuts table
	QTableWidget* createShortcutsTable();

	// create the area below the table where the shortcuts are edited
	QWidget* createShortcutChooser();

	// get the shortcut editor item from the zeroth column of the table
	BtShortcutsEditorItem* getShortcutsEditor(int row);

	// initialize this widget
	void init();

	BtShortcutsDialog* m_dlg;
	QTableWidget* m_table;
	QWidget* m_shortcutChooser;
	QRadioButton* m_noneButton;
	QRadioButton* m_defaultButton;
	QRadioButton* m_customButton;
	QPushButton* m_customPushButton;
	QLabel* m_defaultLabelValue;
	int m_currentRow;
};

#endif

