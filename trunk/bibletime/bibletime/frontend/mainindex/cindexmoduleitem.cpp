//
// C++ Implementation: cindexmoduleitem
//
// Description: 
//
//
// Author: The BibleTime team <info@bibletime.info>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "cindexmoduleitem.h"
#include "cindexitembase.h"
#include "cindextreefolder.h"

#include "util/ctoolclass.h"

#include "backend/drivers/cswordmoduleinfo.h"

#include <QTreeWidgetItem>
#include <QMimeSource>

CIndexModuleItem::CIndexModuleItem(CIndexTreeFolder* parentItem, CSwordModuleInfo* module)
	: CIndexItemBase(parentItem),
	m_module(module)
{}

CIndexModuleItem::~CIndexModuleItem() {}

/** No descriptions */
void CIndexModuleItem::update() {
	if (m_module) {
		setIcon(0, QIcon(CToolClass::getIconForModule(m_module)));
		setText(0,m_module->name() );
	}
}

void CIndexModuleItem::init() {
	//setDragEnabled(false);
	//setDropEnabled(true);

	update();
}


/** Reimplementation to handle text drops on a module. In this case open the searchdialog. In the case of a referebnce open the module at the given position. */
bool CIndexModuleItem::acceptDrop( const QMimeSource* src ) const {
// 	if (CDragDropMgr::canDecode(src)) {
// 	if (CDragDropMgr::dndType(src) == CDragDropMgr::Item::Bookmark) {
// 			CDragDropMgr::Item item = CDragDropMgr::decode(src).first();
// 			CSwordModuleInfo* m = CPointers::backend()->findModuleByName( item.bookmarkModule() );
// 			if (m && (module()->type() == m->type())) { //it makes only sense
// 				return true;
// 			}
// 			//but we also allow drops from bibles on commentaries and the other way from commentaries
// 			else if (m && (module()->type() == CSwordModuleInfo::Bible) && (m->type() == CSwordModuleInfo::Commentary)) {
// 				return true;
// 			}
// 			else if (m && (module()->type() == CSwordModuleInfo::Commentary) && (m->type() == CSwordModuleInfo::Bible)) {
// 				return true;
// 			}
// 		}
// 		else if(CDragDropMgr::dndType(src) == CDragDropMgr::Item::Text) { //text drop on a module
// 			return true;
// 		};
// 	}
// 	return false; //default return value
}

/** No descriptions */
void CIndexModuleItem::dropped( QDropEvent* e, QTreeWidgetItem* after) {
// 	/* Something was dropped on a module item
// 	*
// 	* 1. If the drop type is plain text open the searchdialog for this text and start the search
// 	* 2. If the type is Bookmark, open the module at the specified position
// 	*
// 	* We support only the first drop item, more is not useful
// 	*/
// 
// 	if (acceptDrop(e)) {
// 		CDragDropMgr::ItemList dndItems = CDragDropMgr::decode(e);
// 		CDragDropMgr::Item item = dndItems.first();
// 		if (CDragDropMgr::dndType(e) == CDragDropMgr::Item::Text) { //open the searchdialog
// 			//      qWarning("Text dropped!");
// 			if ( module() ) {
// 				ListCSwordModuleInfo modules;
// 				modules.append(module());
// 
// 				Search::CSearchDialog::openDialog(modules, item.text());
// 			}
// 		}
// 		else if (CDragDropMgr::dndType(e) == CDragDropMgr::Item::Bookmark) { //open the module
// 			CSwordModuleInfo* m = CPointers::backend()->findModuleByName( item.bookmarkModule() );
// 			if (m) { //it makes only sense to create a new window for a module with the same type
// 				if ((module()->type() == m->type()) ||
// 						((module()->type() == CSwordModuleInfo::Bible || module()->type() == CSwordModuleInfo::Commentary)
// 						 && (m->type() == CSwordModuleInfo::Bible || m->type() == CSwordModuleInfo::Commentary))) { //same base type of module
// 					ListCSwordModuleInfo modules;
// 					modules.append(module());
// 
// 					listView()->emitModulesChosen(modules, item.bookmarkKey());
// 				};
// 			}
// 		}
// 	};
}


/** Reimplementation. */
const QString CIndexModuleItem::toolTip() {
	return CToolClass::moduleToolTip(module());
}

/** Returns the used module. */
CSwordModuleInfo* const CIndexModuleItem::module() const {
	return m_module;
}

/** Reimplementation from  CItemBase. */
const bool CIndexModuleItem::enableAction( const MenuAction action ) {
	if (action == EditModule) {
		if (!module())
			return false;
		return module()->isWritable();
	}

	if (action == SearchInModules || action == AboutModule)
		return true;
	if (module()->isEncrypted() && action == UnlockModule)
		return true;
	return false;
}
