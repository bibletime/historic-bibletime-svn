/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2008 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

//TODO: rework this code

//own includes
#include "cresmgr.h"

#include <QObject> //for tr()

//KDE includes


namespace CResMgr {
	namespace modules {
		namespace bible {
			const QString icon_unlocked  = "bible.svg";
			const QString icon_locked    = "bible_locked.svg";
			const QString icon_add       = "bible_add.svg";
		}
		namespace commentary {
			const QString icon_unlocked  = "commentary.svg";
			const QString icon_locked    = "commentary_locked.svg";
			const QString icon_add       = "commentary_add.svg";
		}
		namespace lexicon {
			const QString icon_unlocked  = "lexicon.svg";
			const QString icon_locked    = "lexicon_locked.svg";
			const QString icon_add       = "lexicon_add.svg";
		}
		namespace book {
			const QString icon_unlocked  = "book.svg";
			const QString icon_locked    = "book_locked.svg";
			const QString icon_add       = "book_add.svg";
		}
	}

	namespace categories {
		namespace bibles {
			const QString icon 			= "bible.svg";
		}
		namespace commentaries {
			const QString icon 			= "commentary.svg";
		}
		namespace lexicons {
			const QString icon 			= "dictionary.svg";
		}
		namespace dailydevotional {
			const QString icon 			= "calendar.svg";
		}
		namespace books {
			const QString icon 			= "books.svg";
		}
		namespace glossary {
			const QString icon 			= "dictionary.svg";
		}
		namespace images {
			const QString icon 			= "map.svg";
		}
		namespace cults {
			const QString icon 			= "";
		}
	}
	namespace mainMenu { //Main menu
		namespace file { //Main menu->File
			namespace print { //a standard action
				QString tooltip;

			}
			namespace quit { //a standard action
				QString tooltip;

			}
		}

		namespace view { //Main menu->View
			namespace showMainIndex {
				QString tooltip;

				const QString icon        = "view_index.svg";
				const QKeySequence accel(Qt::Key_F9);
				const char* actionName    = "viewMainIndex_action";
			}
			namespace showInfoDisplay {
				QString tooltip;

				const QString icon        = "view_mag.svg";
				const QKeySequence accel(Qt::Key_F8);
				const char* actionName    = "viewInfoDisplay_action";
			}
			namespace showToolBar { //a standard action
				QString tooltip;

			}
		}

		namespace mainIndex { //Main menu->Settings
			namespace search {
				QString tooltip;

				const QString icon        = "find.svg";
				const QKeySequence accel(Qt::CTRL + Qt::Key_O);
				const char* actionName    = "mainindex_search_action";
			}
			namespace searchdefaultbible {
				QString tooltip;

				const QString icon        = "find.svg";
				const QKeySequence accel(Qt::CTRL + Qt::ALT + Qt::Key_F);
				const char* actionName    = "mainindex_searchdefaultbible_action";
			}
		}

		namespace window { //Main menu->Window
			namespace loadProfile {
				QString tooltip;

				const QString icon        = "view_profile.svg";
				const char* actionName    = "windowLoadProfile_action";
			}
			namespace saveProfile {
				QString tooltip;

				const QString icon        = "view_profile.svg";
				const char* actionName    = "windowSaveProfile_action";
			}
			namespace saveToNewProfile {
				QString tooltip;

				const QString icon        = "view_profile.svg";
				const QKeySequence accel(Qt::CTRL + Qt::ALT + Qt::Key_S);
				const char* actionName    = "windowSaveToNewProfile_action";
			}
			namespace deleteProfile {
				QString tooltip;

				const QString icon        = "view_profile.svg";
				const char* actionName    = "windowDeleteProfile_action";
			}
			namespace showFullscreen {
				QString tooltip;

				const QString icon        = "window_fullscreen.svg";
				const QKeySequence accel(Qt::CTRL + Qt::SHIFT + Qt::Key_F);
				const char* actionName    = "windowFullscreen_action";
			}
			namespace arrangementMode {
				QString tooltip;

				const QString icon        = "cascade_auto.svg";
				const QKeySequence accel;
				const char* actionName    = "windowArrangementMode_action";

				namespace manual {
					QString tooltip;

					const QString icon        = "tile.svg";
					const QKeySequence accel(Qt::CTRL + Qt::ALT + Qt::Key_M);
					const char* actionName    = "windowArrangementManual_action";
				}
				namespace autoTileHorizontal {
					QString tooltip;

					const QString icon        = "tile_auto.svg";
					const QKeySequence accel(Qt::CTRL + Qt::ALT + Qt::Key_H);
					const char* actionName    = "windowAutoTileHorizontal_action";
				}
				namespace autoTileVertical {
					QString tooltip;

					const QString icon        = "tile_auto.svg";
					const QKeySequence accel(Qt::CTRL + Qt::ALT + Qt::Key_G);
					const char* actionName    = "windowAutoTileVertical_action";
				}
				namespace autoCascade {
					QString tooltip;

					const QString icon        = "cascade_auto.svg";
					const QKeySequence accel(Qt::CTRL + Qt::ALT + Qt::Key_J);
					const char* actionName    = "windowAutoCascade_action";
				}
			}
			namespace tileHorizontal {
				QString tooltip;

				const QString icon        = "tile.svg";
				const QKeySequence accel(Qt::CTRL + Qt::Key_H);
				const char* actionName    = "windowTileHorizontal_action";
			}
			namespace tileVertical {
				QString tooltip;

				const QString icon        = "tile.svg";
				const QKeySequence accel(Qt::CTRL + Qt::Key_G);
				const char* actionName    = "windowTileVertical_action";
			}
			namespace cascade {
				QString tooltip;

				const QString icon        = "cascade.svg";
				const QKeySequence accel(Qt::CTRL + Qt::Key_J);
				const char* actionName    = "windowCascade_action";
			}
			namespace closeAll {
				QString tooltip;

				const QString icon        = "fileclose.svg";
				const QKeySequence accel(Qt::CTRL + Qt::ALT + Qt::Key_W);
				const char* actionName    = "windowCloseAll_action";
			}
		}

		namespace settings { //Main menu->Settings
			namespace editToolBar { // available as KStdAction
				QString tooltip;

			}
			namespace optionsDialog { // available as KStdAction
				QString tooltip;

			}
			namespace swordSetupDialog {
				QString tooltip;

				const QString icon        = "swordconfig.svg";
				const QKeySequence accel(Qt::Key_F4);
				const char* actionName    = "options_sword_setup";
			}

		}

		namespace help { //Main menu->Help
			namespace handbook {
				QString tooltip;

				const QString icon        = "contents2.svg";
				const QKeySequence accel(Qt::Key_F1);
				const char* actionName    = "helpHandbook_action";
			}
			namespace bibleStudyHowTo {
				QString tooltip;

				const QString icon        = "contents2.svg";
				const QKeySequence accel(Qt::Key_F2);
				const char* actionName    = "helpHowTo_action";
			}
			namespace bugreport { // available as KStdAction
				QString tooltip;

			}
			namespace aboutBibleTime { // available as KStdAction
				QString tooltip;

			}
			namespace aboutKDE { // available as KStdAction
				QString tooltip;

			}
		}
	}  //end of main menu

	namespace searchdialog {
		const QString icon = "find.svg";

		namespace searchButton {
			QString tooltip;

		}
		namespace cancelSearchButton {
			QString tooltip;

		}

		namespace options {
			namespace moduleChooserButton {
				QString tooltip;

			}
			namespace searchedText {
				QString tooltip;

			}
			namespace searchType {
				namespace multipleWords_and {
					QString tooltip;

				}
				namespace multipleWords_or {
					QString tooltip;

				}
				namespace exactMatch {
					QString tooltip;

				}
				namespace regExp {
					QString tooltip;

				}
			}

			namespace searchOptions {
				namespace caseSensitive {
					QString tooltip;

				}
			}
			namespace chooseScope {
				QString tooltip;

			}
			namespace scopeEditor {
				namespace rangeList {
					QString tooltip;

				}
				namespace nameEdit {
					QString tooltip;

				}
				namespace editRange {
					QString tooltip;

				}
				namespace parsedResult {
					QString tooltip;

				}
				namespace addNewRange {
					QString tooltip;

				}
				namespace deleteCurrentRange {
					QString tooltip;

				}

			}
		}
		namespace result {
			namespace moduleList {
				QString tooltip;


				namespace copyMenu {
					const QString icon = "edit_copy.svg";
				}
				namespace saveMenu {
					const QString icon = "file_save.svg";
				}
				namespace printMenu {
					const QString icon = "print.svg";
				}
			}
			namespace foundItems {
				QString tooltip;


				namespace copyMenu {
					const QString icon = "edit_copy.svg";
				}
				namespace saveMenu {
					const QString icon = "file_save.svg";
				}
				namespace printMenu {
					const QString icon = "print.svg";
				}
			}
			namespace textPreview {
				QString tooltip;

			}
		}
	}

	namespace workspace {}

	namespace displaywindows {
/*		namespace transliteration {
			const QString icon = "bt_displaytranslit";
		}*/
		namespace displaySettings {
			const QString icon = "displayconfig.svg";
		}

		namespace general {
			namespace scrollButton {
				QString tooltip;
			}

			namespace search {
				QString tooltip;

				const QString icon        = "find.svg";
				const QKeySequence accel(Qt::CTRL + Qt::Key_L);
				const char* actionName    = "window_search_action";
			}

			namespace backInHistory {
				QString tooltip;

				const QString icon        = "back.svg";
				const QKeySequence accel(Qt::ALT + Qt::Key_Left);
				const char* actionName    = "window_history_back_action";
			}
			namespace forwardInHistory {
				QString tooltip;

				const QString icon        = "forward.svg";
				const QKeySequence accel(Qt::ALT + Qt::Key_Right);
				const char* actionName    = "window_history_forward_action";
			}
			namespace findStrongs {
				QString tooltip;

				const QString icon        = "bt_findstrongs.svg";
				const QKeySequence accel;
				const char* actionName    = "window_find_strongs_action";
			}

		}
		namespace bibleWindow {
			namespace bookList {
				QString tooltip;

			}
			namespace nextBook {
				QString tooltip;

				const QKeySequence accel(Qt::CTRL + Qt::Key_Y);
			}
			namespace previousBook {
				QString tooltip;

				const QKeySequence accel(Qt::CTRL + Qt::SHIFT + Qt::Key_Y);
			}

			namespace chapterList {
				QString tooltip;
			}
			namespace nextChapter {
				QString tooltip;
				const QKeySequence accel(Qt::CTRL + Qt::Key_X);
			}
			namespace previousChapter {
				QString tooltip;
				const QKeySequence accel(Qt::CTRL + Qt::SHIFT + Qt::Key_X);
			}
			namespace verseList {
				QString tooltip;
			}
			namespace nextVerse {
				QString tooltip;
				const QKeySequence accel(Qt::CTRL + Qt::Key_V);
			}
			namespace previousVerse {
				QString tooltip;
				const QKeySequence accel(Qt::CTRL + Qt::SHIFT + Qt::Key_V);
			}

			namespace copyMenu {
				const QString icon = "edit_copy.svg";
			}
			namespace saveMenu {
				const QString icon = "file_save.svg";
			}
			namespace printMenu {
				const QString icon = "print.svg";
			}
		}
		namespace commentaryWindow {
			namespace syncWindow {
				QString tooltip;
				const QString icon   = "sync.svg";
				const QKeySequence accel;
				const char* actionName = "commentary_syncWindow";
			}
		}
		namespace lexiconWindow {
			namespace entryList {
				QString tooltip;
			}
			namespace nextEntry {
				QString tooltip;
				const QKeySequence accel(Qt::CTRL + Qt::Key_V);
			}
			namespace previousEntry {
				QString tooltip;
				const QKeySequence accel(Qt::CTRL + Qt::SHIFT + Qt::Key_V);
			}

			namespace copyMenu {
				const QString icon = "edit_copy.svg";
			}
			namespace saveMenu {
				const QString icon = "file_save.svg";
			}
			namespace printMenu {
				const QString icon = "print.svg";
			}
		}
		namespace bookWindow {
			namespace toggleTree {
				const QString icon = "view_sidetree.svg";
				const QKeySequence accel;
			}
		}

		namespace writeWindow {
			namespace saveText {
				QString tooltip;

				const QString icon      = "file_save.svg";
				const QKeySequence accel;
				const char* actionName  = "writeWindow_saveText";
			}
			namespace restoreText {
				QString tooltip;

				const QString icon      = "undo.svg";
				const QKeySequence accel;
				const char* actionName  = "writeWindow_restoreText";
			}
			namespace deleteEntry {
				QString tooltip;

				const QString icon      = "edit_delete.svg";
				const QKeySequence accel;
				const char* actionName  = "writeWindow_deleteEntry";
			}

			//formatting buttons
			namespace boldText {
				QString tooltip;

				const QString icon      = "text_bold.svg";
				const QKeySequence accel;
				const char* actionName  = "writeWindow_boldText";
			}
			namespace italicText {
				QString tooltip;

				const QString icon      = "text_italic.svg";
				const QKeySequence accel;
				const char* actionName  = "writeWindow_italicText";
			}
			namespace underlinedText {
				QString tooltip;

				const QString icon      = "text_under.svg";
				const QKeySequence accel;
				const char* actionName  = "writeWindow_underlineText";
			}

			namespace alignLeft {
				QString tooltip;

				const QString icon      = "text_left.svg";
				const QKeySequence accel;
				const char* actionName  = "writeWindow_alignLeft";
			}
			namespace alignCenter {
				QString tooltip;

				const QString icon      = "text_center.svg";
				const QKeySequence accel;
				const char* actionName  = "writeWindow_alignCenter";
			}
			namespace alignRight {
				QString tooltip;

				const QString icon      = "rightjust.svg";
				const QKeySequence accel;
				const char* actionName  = "writeWindow_alignRight";
			}
			namespace alignJustify {
				QString tooltip;

				const QString icon      = "text_block.svg";
				const QKeySequence accel;
				const char* actionName = "writeWindow_alignJustify";
			}

			namespace fontFamily {
				QString tooltip;

				const QKeySequence accel;
				const char* actionName = "writeWindow_fontFamily";
			}
			namespace fontSize {
				QString tooltip;

				const QKeySequence accel;
				const char* actionName = "writeWindow_fontSize";
			}
			namespace fontColor {
				QString tooltip;
			}
		}
	}

	namespace settings {
		namespace startup {
			const QString icon = "startconfig.svg";
			namespace showLogo {
				QString tooltip;
			}
			namespace restoreWorkingArea {
				QString tooltip;
			}
		}
		namespace fonts {
			const QString icon = "fonts.svg";

			namespace typeChooser {
				QString tooltip;
			}
		}
		namespace profiles {
			const QString icon = "view_profile.svg";

			namespace list {
				QString tooltip;
			}
			namespace createNew {
				QString tooltip;
			}
			namespace deleteCurrent {
				QString tooltip;
			}
			namespace renameCurrent {
				QString tooltip;
			}
		}
		namespace sword {
			const QString icon = "swordconfig.svg";

			namespace general {
				namespace language {
					QString tooltip;
				}
			}
			namespace modules {
				namespace bible {
					QString tooltip;
				}
				namespace commentary {
					QString tooltip;
				}
				namespace lexicon {
					QString tooltip;

				}
				namespace dailyDevotional {
					QString tooltip;
				}
				namespace hebrewStrongs {
					QString tooltip;
				}
				namespace greekStrongs {
					QString tooltip;
				}
				namespace hebrewMorph {
					QString tooltip;
				}
				namespace greekMorph {
					QString tooltip;
				}
			}
		}
		namespace keys {
			const QString icon = "key_bindings.svg";
		}
	}

	namespace mainIndex { // Bookshelf view
		namespace search {
			QString tooltip;

			const QString icon        = "find.svg";
			const QKeySequence accel(Qt::CTRL + Qt::ALT + Qt::Key_M);
			const char* actionName    = "GMsearch_action";
		}
		namespace newFolder {
			const QString icon = "folder_new.svg";
		}
		namespace changeFolder {
			const QString icon = "folder.svg";
		}
		namespace openedFolder {
			const QString icon = "folder_open.svg";
		}
		namespace closedFolder {
			const QString icon = "folder.svg";
		}

		namespace bookmark {
			const QString icon = "bookmark.svg";
		}
		namespace changeBookmark {
			const QString icon = "bookmark.svg";
		}
		namespace importBookmarks {
			const QString icon = "bookmark.svg";
		}
		namespace exportBookmarks {
			const QString icon = "bookmark.svg";
		}
		namespace printBookmarks {
			const QString icon = "print.svg";
		}
		namespace deleteItems {
			const QString icon = "edit_delete.svg";
		}

		namespace editModuleMenu {
			const QString icon = "pencil.svg";
		}
		namespace editModulePlain {
			const QString icon = "pencil.svg";
		}
		namespace editModuleHTML {
			const QString icon = "pencil.svg";
		}

		namespace unlockModule {
			const QString icon = "unlock.svg";
		}
		namespace aboutModule {
			const QString icon = "info.svg";
		}
		namespace grouping {
			const QString icon = "view-tree.svg";
		}
	}
}



namespace CResMgr {
	void init_tr() {
		using namespace CResMgr;
		{
			using namespace mainMenu;
			{
				using namespace file;
				{
					using namespace print;
					tooltip     = QObject::tr("Open the printer dialog of BibleTime, where you can edit the print queue, assign styles to the items and print them.") ;
				}
				{
					using namespace quit;
					tooltip     = QObject::tr("Close BibleTime and save the settings.") ;
				}

				{
					using namespace view;
					{
						using namespace showMainIndex;
						tooltip     = QObject::tr("Show or hide the bookshelf.") ;
					}
					{
						using namespace showToolBar;
						tooltip     = QObject::tr("Toggle the main toolbar view.") ;
					}
				}

				{
					using namespace mainMenu::mainIndex;
					{
						using namespace search;
						tooltip     = QObject::tr("Open the search dialog to search in all works that are currently open.") ;
					}
					{
						using namespace searchdefaultbible;
						tooltip     = QObject::tr("Open the search dialog to search in the standard Bible.") ;
					}
				};

				{
					using namespace window;
					{
						using namespace loadProfile;
						tooltip     = QObject::tr("Restore a saved BibleTime session.") ;
					}
					{
						using namespace saveProfile;
						tooltip     = QObject::tr("Save current BibleTime session so that it can be reused later.") ;
					}
					{
						using namespace saveToNewProfile;
						tooltip     = QObject::tr("Create and save a new session.") ;
					}
					{
						using namespace deleteProfile;
						tooltip     = QObject::tr("Delete a BibleTime session.") ;
					}
					{
						using namespace showFullscreen;
						tooltip     = QObject::tr("Toggle fullscreen mode of the main window.") ;
					}
					{
						using namespace tileVertical;
						tooltip     = QObject::tr("Vertically tile the open windows.") ;
					}
					{
						using namespace tileHorizontal;
						tooltip     = QObject::tr("Horizontally tile the open windows.") ;
					}
					{
						using namespace cascade;
						tooltip     = QObject::tr("Cascade the open windows.") ;
					}
					{
						{
							using namespace arrangementMode;
							tooltip     = QObject::tr("Choose the way that is used to arrange the windows.") ;
						}
						{
							using namespace arrangementMode::autoTileVertical;
							tooltip     = QObject::tr("Automatically tile the open windows vertically.") ;
						}
						{
							using namespace arrangementMode::autoTileHorizontal;
							tooltip     = QObject::tr("Automatically tile the open windows horizontally.") ;
						}
						{
							using namespace arrangementMode::autoCascade;
							tooltip     = QObject::tr("Automatically cascade the open windows.") ;
						}
					}
					{
						using namespace closeAll;
						tooltip     = QObject::tr("Close all open windows.") ;
					}
				}

				{
					using namespace mainMenu::settings;
					{
						using namespace editToolBar;
						tooltip     = QObject::tr("Open BibleTime's toolbar editor.") ;
					}
					{
						using namespace optionsDialog;
						tooltip     = QObject::tr("Open the dialog to set most of BibleTime's preferences.") ;
					};
					{
						using namespace swordSetupDialog;
						tooltip     = QObject::tr("Open the dialog to configure your bookshelf and install/update/remove works.") ;
					}

				}

				{
					using namespace help;
					{
						using namespace handbook;
						tooltip     = QObject::tr("Open BibleTime's handbook in the KDE helpbrowser.") ;
					}
					{
						using namespace bibleStudyHowTo;
						tooltip     = QObject::tr("Open the Bible study HowTo included with BibleTime in the KDE helpbrowser. <BR>This HowTo is an introduction on how to study the Bible in an efficient way.") ;
					}
					{
						using namespace bugreport;
						tooltip     = QObject::tr("Send a bugreport to the developers of BibleTime.") ;
					}
					{
						using namespace aboutBibleTime;
						tooltip     = QObject::tr("Show detailed information about BibleTime.") ;
					}
					{
						using namespace aboutKDE;
						tooltip     = QObject::tr("Show detailed information about the KDE project.") ;
					}
				}
			}
		}

		{
			using namespace searchdialog;
			{
				using namespace searchButton;
				tooltip     = QObject::tr("Start to search the text in each of the chosen work(s).") ;
			}
			{
				using namespace cancelSearchButton;
				tooltip     = QObject::tr("Stop the active search.") ;
			}

			{
				using namespace options;
				{
					using namespace moduleChooserButton;
					tooltip     = QObject::tr("Open a dialog to choose work(s) for the search.") ;
				}
				{
					using namespace searchedText;
					tooltip     = QObject::tr("Enter the text you want to search in the chosen work(s) here.") ;
				}
				{
					using namespace searchType;
					{
						using namespace multipleWords_and;
						tooltip     = QObject::tr("Treat the search text as multiple words. A text must contain all of the words to match. The order of the words is unimportant.") ;
					}
					{
						using namespace multipleWords_or;
						tooltip     = QObject::tr("Treat the search text as multiple words. A text must contain one or more words of to match. The order is unimportant.") ;
					}
					{
						using namespace exactMatch;
						tooltip     = QObject::tr("The search text will be used exactly as entered.") ;
					}
					{
						using namespace regExp;
						tooltip     = QObject::tr("Treat the search string as a GNU regular expression. The BibleTime handbook contains an introduction to regular expressions.") ;
					}
				}

				{
					using namespace searchOptions;
					{
						using namespace caseSensitive;
						tooltip     = QObject::tr("If you choose this option the search will distinguish between upper and lowercase characters.") ;
					}
				}
				{
					using namespace chooseScope;
					tooltip     = QObject::tr("Choose a scope from the list. \
Select the first item to use no scope, the second one is to use each work's last search result as search scope. \
The others are user defined search scopes.");
				}
				{
					using namespace scopeEditor;
					{
						using namespace rangeList;
						tooltip     = QObject::tr("Select an item from the list to edit the search scope.") ;
					}
					{
						using namespace nameEdit;
						tooltip     = QObject::tr("Change the name of the selected search scope.") ;
					}
					{
						using namespace editRange;
						tooltip     = QObject::tr("Change the search ranges of the selected search scope item. Have a look at the predefined search scopes to see how search ranges are constructed.") ;
					}
					{
						using namespace parsedResult;
						tooltip     = QObject::tr("Contains the search ranges which will be used for the search.") ;
					}
					{
						using namespace addNewRange;
						tooltip     = QObject::tr("Add a new search scope. First enter an appropriate name, then edit the search ranges.") ;
					}
					{
						using namespace deleteCurrentRange;
						tooltip     = QObject::tr("Deletes the selected search scope. If you close the dialog using Cancel the settings won't be saved.") ;
					}
				}
			}
			{
				using namespace result;
				{
					using namespace moduleList;
					tooltip     = QObject::tr("The list of works chosen for the search.") ;
				}
				{
					using namespace foundItems;
					tooltip     = QObject::tr("This list contains the search result of the selected work.") ;
				}
				{
					using namespace textPreview;
					tooltip     = QObject::tr("The text preview of the selected search result item.") ;
				}
			}
		}

		{
			using namespace displaywindows;
			{
				using namespace general;
				{
					{
						using namespace scrollButton;
						tooltip = QObject::tr("This button is useful to scroll through the entries of the list. Press the button and move the mouse to increase or decrease the item.") ;
					}
					{
						using namespace search;
						tooltip = QObject::tr("This button opens the search dialog with the work(s) of this window.") ;

					}
					{
						using namespace backInHistory;
						tooltip = QObject::tr("Go back one item in the display history.") ;
					}
					{
						using namespace forwardInHistory;
						tooltip = QObject::tr("Go forward one item in the display history.") ;

					}
					{
						using namespace findStrongs;
						tooltip = QObject::tr("Show all occurences of the Strong number currently under the mouse cursor.") ;
					}
				}
				using namespace bibleWindow;
				{
					using namespace bookList;
					tooltip = QObject::tr("This list contains the books which are available in this work.") ;
				}
				{
					using namespace nextBook;
					tooltip = QObject::tr("Show the next book of this work.") ;
				}
				{
					using namespace previousBook;
					tooltip = QObject::tr("Show the previous book of this work.") ;
				}
				{
					using namespace chapterList;
					tooltip = QObject::tr("This list contains the chapters which are available in the current book.") ;
				}
				{
					using namespace nextChapter;
					tooltip = QObject::tr("Show the next chapter of the work.") ;
				}
				{
					using namespace previousChapter;
					tooltip = QObject::tr("Show the previous chapter of the work.") ;
				}
				{
					using namespace verseList;
					tooltip = QObject::tr("This list contains the verses which are available in the current chapter.") ;
				}
				{
					using namespace nextVerse;
					tooltip = QObject::tr("In  Bible texts, the next verse will be highlighted. In commentaries, the next entry will be shown.") ;

				}
				{
					using namespace previousVerse;
					tooltip = QObject::tr("In Bible texts, the previous verse will be highlighted. In commentaries, the previous entry will be shown.") ;
				}
			}
			{
				using namespace commentaryWindow;
				{
					using namespace syncWindow;
					tooltip = QObject::tr("Synchronize the displayed entry of this work with the active Bible window.") ;
				}
			}
			{
				using namespace lexiconWindow;
				{
					using namespace entryList;
					tooltip = QObject::tr("This list contains the entries of the current work.") ;
				}
				{
					using namespace nextEntry;
					tooltip = QObject::tr("The next entry of the work will be shown.") ;
				}
				{
					using namespace previousEntry;
					tooltip = QObject::tr("The previous entry of the work will be shown.") ;
				}
			}

			{
				using namespace writeWindow;
				{
					using namespace saveText;
					tooltip   = QObject::tr("Save the curent text into the work. The old text will be overwritten.") ;
				}
				{
					using namespace restoreText;
					tooltip   = QObject::tr("Loads the old text from the work and loads it into the edit area. The unsaved text will be lost.") ;
				}
				{
					using namespace deleteEntry;
					tooltip   = QObject::tr("Deletes the current entry out of the work. The text will be lost.") ;
				}

				//formatting buttons
				{
					using namespace boldText;
					tooltip   = QObject::tr("Toggle bold formatting of the selected text.") ;
				}
				{
					using namespace italicText;
					tooltip   = QObject::tr("Toggle italic formatting of the selected text.") ;
				}
				{
					using namespace underlinedText;
					tooltip   = QObject::tr("Toggle underlined formatting of the selected text.") ;
				}

				{
					using namespace alignLeft;
					tooltip   = QObject::tr("The text will be aligned on the left side of the page.") ;
				}
				{
					using namespace alignCenter;
					tooltip   = QObject::tr("Centers the text horizontally.") ;
				}
				{
					using namespace alignRight;
					tooltip   = QObject::tr("Aligns the text on the right side of the page.") ;
				}
				{
					using namespace alignJustify;
					tooltip   = QObject::tr("Justifies the text on the page.") ;
				}

				{
					using namespace fontFamily;
					tooltip   = QObject::tr("Choose a new font for the selected text.") ;
				}
				{ using namespace fontSize;
					tooltip   = QObject::tr("Choose a new font size for the selected text.") ;
				}
				{ using namespace fontColor;
					tooltip   = QObject::tr("Choose a new color for the selected text.") ;
				}
			}
		}
		{
			using namespace settings;
			{
				using namespace startup;
				{
					using namespace showLogo;
					tooltip   = QObject::tr("Activate this to see the BibleTime logo on startup.") ;
				}
				{
					using namespace restoreWorkingArea;
					tooltip   = QObject::tr("Save the user's session when BibleTime is closed and restore it on the next startup.") ;
				}
			}
			{
				using namespace fonts;
				{
					using namespace typeChooser;
					tooltip   = QObject::tr("The font selection below will apply to all texts in this language.") ;
				}
			}
			{
				using namespace settings::sword;
				{
					using namespace general;
					{
						using namespace language;
						tooltip   = QObject::tr("Contains the languages which can be used for the biblical booknames.") ;
					}
				}
				{
					using namespace settings::sword::modules;
					{
						using namespace bible;
						tooltip   = QObject::tr("The standard Bible is used when a hyperlink into a Bible is clicked.") ;
					}
					{
						using namespace commentary;
						tooltip   = QObject::tr("The standard commentary is used when a hyperlink into a commentary is clicked.") ;
					}
					{
						using namespace lexicon;
						tooltip   = QObject::tr("The standard lexicon is used when a hyperlink into a lexicon is clicked.") ;
					}
					{
						using namespace dailyDevotional;
						tooltip   = QObject::tr("The standard devotional will be used to display a short start up devotional.") ;
					}
					{
						using namespace hebrewStrongs;
						tooltip   = QObject::tr("The standard Hebrew lexicon is used when a hyperlink into a Hebrew lexicon is clicked.") ;
					}
					{
						using namespace greekStrongs;
						tooltip   = QObject::tr("The standard Greek lexicon is used when a hyperlink into a Greek lexicon is clicked.") ;
					}
					{
						using namespace hebrewMorph;
						tooltip   = QObject::tr("The standard morphological lexicon for Hebrew texts is used when a hyperlink of a morphological tag in a Hebrew text is clicked.") ;
					}
					{
						using namespace greekMorph;
						tooltip   = QObject::tr("The standard morphological lexicon for Greek texts is used when a hyperlink of a morphological tag in a Greek text is clicked.") ;
					}
				}
			}
		}
		{
			using namespace mainIndex;
			{
				using namespace search;
				tooltip     = QObject::tr("Opens the search dialog to search in the work(s) that are currently open.") ;
			}
		}
	}
}
