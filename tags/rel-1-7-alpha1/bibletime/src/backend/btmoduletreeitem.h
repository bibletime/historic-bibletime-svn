//
// C++ Interface: btmoduletreeitem
//
// Description: 
//
//
// Author: The BibleTime team <info@bibletime.info>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BTMODULETREEITEM_H
#define BTMODULETREEITEM_H

#include "backend/drivers/cswordmoduleinfo.h"

#include <QString>


/**
Item of a tree which represents Sword modules categorized and filtered.
Can be used when building trees for different views.

The tree will be created with the public constructor. It creates the root item and
populates it with the rest of the tree. The root item is the handle to the tree.
Users can get the list of the children and operate on it recursively.

The tree is meant to be created, read and then deleted. If you need to apply for example
different set of filters you have to create a new tree - it's not possible to modify the tree.

Example:

	...
	BTModuleTreeItem root(QList<BTModuleTreeItem::Filter*>(), BTModuleTreeItem::CatLangMod);
	add_to_view(&root, qtreewidget->invisibleRootItem());
	...
	void add_to_view(BTModuleTreeItem* item, QTreeWidgetItem* widgetItem) {
		foreach (BTModuleTreeItem* i, item->children()) {
			add_to_view(i, new QTreeWidgetItem(widgetItem));
		}
		if (item->type() == BTModuleTreeItem::Category) prepare_category_item(widgetItem, item);
		...
	}
	

	@author The BibleTime team <info@bibletime.info>
*/
class BTModuleTreeItem
{
public:

	/**
	* A filter which is given to the root constructor. It filters some modules
	* out from the tree. If it returns true when the filter() is called the module will be added,
	* if it returns false the module will be left out.
	*
	* If you want for example to get only unindexed modules in the list you should
	* write a class (possibly a small inner class inside the calling class) which
	* inherits Filter and write the operator() function which returns true if the
	* module is unindexed and false if it's indexed.
	*
	* It's also possible to do arbitrary tasks to modules by using more complex subclasses.
	*
	* The filters will be applied in the order in which they are in the list.
	* A module will be filtered out if even one filter rejects it and testing
	* will stop with the first negative.
	*
	* Example:
	*	QList<BTModuleTreeItem::Filter> filters;
	*	MyFilter filter; BTModuleTreeItem::HiddenOff hideFilter; 
	*	filters.append(&hideFilter); filters.append(&filter);
	*	BTModuleTreeItem root(filters, BTModuleTreeItem::CatLangMod);
	*/
	struct Filter {
		virtual bool filter(CSwordModuleInfo*) = 0;
	};

	/**
	* One example of a filter which can be used with any view. If the module has been
	* set "hidden" it will be filtered out.
	*/
	struct HiddenOff : public Filter {
		inline bool filter(CSwordModuleInfo* mi) { return !mi->isHidden(); }
	};
	
	/**
	* Type of the item: root item, category (Bibles, Commentaries etc.), language or module.
	*/
	enum Type {Root, Category, Language, Module};
	
	/**
	* Tells how to group the modules. For example:
	* CatLangMod: first category, second language, third module. Mod: don't use
	* Category or Language at all, Module is toplevel and tree is flat.
	*/
	enum Grouping {CatLangMod, CatMod, LangCatMod, LangMod, Mod};


	/**
	* This constructor creates a root item. Create it for example with scoped_ptr or in stack.
	* The root item is populated with the item tree.
	* The constructor takes a list of filters (see Filter), grouping indicator (see Grouping)
	* and optionally the module list from which the tree is constructed
	* (by default CPointers::backend()->moduleList() is used).
	*/
	BTModuleTreeItem(QList<BTModuleTreeItem::Filter*>& filters,
					BTModuleTreeItem::Grouping grouping, ListCSwordModuleInfo* modules = 0);

	/** When the root item is deleted the whole tree is deleted. */
	~BTModuleTreeItem();

	/**
	* Returns the item type.
	*/
	inline BTModuleTreeItem::Type type() const {return m_type;}
	/**
	* Returns the item text (category name, language name or module name).
	*/
	inline QString text() const {return m_text;}
	/**
	* Returns the path to the icon which is appropriate for this type of item, or QString::null.
	*/
	QString iconName() const;
	/**
	* If the type is Module returns a pointer to the corresponding CSwordModuleInfo object,
	* otherwise returns 0.
	*/
	inline CSwordModuleInfo* moduleInfo() const {return m_moduleInfo;}
	/**
	* Returns a list of the direct children of this item.
	*/
	QList<BTModuleTreeItem*> children() const;

	/**
	* For alphabetical sorting which uses text(). See QString::localeAwareCompare().
	* Categories will always be in the same order regardless of the i18n.
	*/
	static bool localeAwareLessThan(BTModuleTreeItem* first, BTModuleTreeItem* second);


private:
	/**
	* Private constructor which sets the members.
	*/
	BTModuleTreeItem(BTModuleTreeItem* parentItem, const QString& text, Type type, CSwordModuleInfo* info=0, CSwordModuleInfo::Category category=CSwordModuleInfo::UnknownCategory);
	/** Default ctor is private because it is not to be called.*/
	BTModuleTreeItem();

	/** Creates the tree under this root item (called only from root ctor). */
	void create_tree(QList<BTModuleTreeItem::Filter*>& filters, BTModuleTreeItem::Grouping grouping);
	/** Sorts recursively the children of of the given item. */
	void sort_children(BTModuleTreeItem* parent);
	/** Helper function for creating a group item while creating the tree. */
	BTModuleTreeItem* create_parent_item(BTModuleTreeItem* parent, const QString& text, BTModuleTreeItem::Type type, CSwordModuleInfo::Category category=CSwordModuleInfo::UnknownCategory);

	CSwordModuleInfo* m_moduleInfo;
	QString m_text;
	Type m_type;
	CSwordModuleInfo::Category m_category;
	BTModuleTreeItem* m_next;
	BTModuleTreeItem* m_firstChild;
	ListCSwordModuleInfo m_originalModuleList;
};

#endif
