//
// C++ Implementation: cindexitembase
//
// Description: 
//
//
// Author: The BibleTime team <info@bibletime.info>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "cindexitembase.h"
#include "cmainindex.h"

#include <QTreeWidgetItem>
#include <QString>
#include <QMimeSource>

CIndexItemBase::CIndexItemBase(CMainIndex* mainIndex, const Type type)
	: QTreeWidgetItem(mainIndex),
	m_type(type),
	m_sortingEnabled(true)
{}

CIndexItemBase::CIndexItemBase(CIndexItemBase* parentItem, const Type type)
	: QTreeWidgetItem(parentItem),
	m_type(type),
	m_sortingEnabled(true)
{}

CIndexItemBase::~CIndexItemBase() {}

const QString CIndexItemBase::toolTip()
{
	return QString::null;
}

/** Returns the used main index. */
CMainIndex* CIndexItemBase::listView() const {
	return dynamic_cast<CMainIndex*>( QTreeWidgetItem::listView() );
}

const bool CIndexItemBase::isFolder() {
	return false;
}

void CIndexItemBase::init()
{
	update();
}

void CIndexItemBase::update()
{}

const CIndexItemBase::Type& CIndexItemBase::type() const
{
	return m_type;
};

void CIndexItemBase::moveAfter( CIndexItemBase* const item )
{
	if (!item)
		return;

	if ( parent() == item->parent() ) { //same parent means level
		moveItem(item); //both items are on the same level, so we can use moveItem
	}
}

/** Returns true if the given action should be enabled in the popup menu. */
const bool CIndexItemBase::enableAction( const MenuAction /*action*/ )
{
	return false; //this base class has no valif actions
}

/** No descriptions */
const bool CIndexItemBase::isMovable()
{
	return false;
}

QDomElement CIndexItemBase::saveToXML( QDomDocument& /*document*/ )
{
	return QDomElement();
}

void CIndexItemBase::loadFromXML( QDomElement& /*element*/ )
{}

void dropped( QDropEvent* e)
{
	dropped(e,0);
}

void CIndexItemBase::dropped( QDropEvent* /*e*/, QTreeWidgetItem* /*after*/)
{}

const bool CIndexItemBase::allowAutoOpen( const QMimeSource* ) const
{
	return false;
};

/** This function engables or disables sorting depending on the parameter. */
void CIndexItemBase::setSortingEnabled( const bool& enableSort )
{
	m_sortingEnabled = enableSort;
}

/** Returns true whether the sorting is enabled or not. */
const bool CIndexItemBase::isSortingEnabled()
{
	return m_sortingEnabled;
}

/** Reimplementation which takes care of the our sortingEnabled setting. */
void CIndexItemBase::sortChildItems( int col, bool asc )
{
	if (!isSortingEnabled()) {
		return;
	}
	else {
		QTreeWidgetItem::sortChildItems( col, asc );
	}
}

/** Reimplementation which takes care of the our sortingEnabled setting. */
void CIndexItemBase::sort()
{
	if (!isSortingEnabled()) {
		return;
	}
	else {
		QTreeWidgetItem::sort();
	}
}
