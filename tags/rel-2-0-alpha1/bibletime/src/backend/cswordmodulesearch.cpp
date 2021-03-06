/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2008 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

//BibleTime includes
#include "cswordmodulesearch.h"

#include "cswordmodulesearch.moc"
#include "backend/drivers/cswordmoduleinfo.h"
#include "backend/managers/cswordbackend.h"
#include "backend/config/cbtconfig.h"

//Sword includes
#include <swmodule.h>
#include <swkey.h>
#include <listkey.h>


CSwordModuleSearch* CSwordModuleSearch::searcher = 0;

CSwordModuleSearch::CSwordModuleSearch()
	: m_searchedText(QString::null),
	m_searchOptions(0),
	m_foundItems(false)
{
	searcher = this;
}

CSwordModuleSearch::~CSwordModuleSearch() {
	searcher = 0;
}

/** This function sets the modules which should be searched. */
void CSwordModuleSearch::setModules( const QList<CSwordModuleInfo*>& list ) {
	m_moduleList = list;
}

/** Starts the search for the search text. */
bool CSwordModuleSearch::startSearch() {
	backend()->setFilterOptions ( CBTConfig::getFilterOptionDefaults() );
	m_foundItems   = false;

	bool foundItems = false;

	// for (m_moduleList.first(); m_moduleList.current() && !m_terminateSearch; m_moduleList.next()) {
	QList<CSwordModuleInfo*>::iterator end_it = m_moduleList.end();

	for (QList<CSwordModuleInfo*>::iterator it = m_moduleList.begin(); it != end_it; ++it) {
		if ( (*it)->searchIndexed(m_searchedText/*, m_searchOptions*/, m_searchScope) ) {
			foundItems = true;
		}
	}

	m_foundItems = foundItems;

	//m_finishedSig.activate();
	emit finished();
	return true;
}

/** Sets the text which should be search in the modules. */
void CSwordModuleSearch::setSearchedText( const QString& text ) {
	m_searchedText = text;
}

/** Sets the search scope. */
void CSwordModuleSearch::setSearchScope( const sword::ListKey& scope ) {
	m_searchScope.copyFrom( scope );

	if (!strlen(scope.getRangeText())) { //we can't search with an empty search scope, would crash
		//reset the scope
		resetSearchScope();

		//disable searching with a scope!
		//  if (m_searchOptions | useScope) {
		//      qWarning("using the scope!");
		//set back the scope flag
		// }
	}
}

/** Sets the search scope back. */
void CSwordModuleSearch::resetSearchScope() {
	m_searchScope.ClearList();
}

/** Returns true if in the last search the searcher found items, if no items were found return false. */
bool CSwordModuleSearch::foundItems() const {
	return m_foundItems;
}

/** Returns a copy of the used search scope. */
const sword::ListKey& CSwordModuleSearch::searchScope() const {
	return m_searchScope;
}

void CSwordModuleSearch::connectFinished( QObject *receiver, const char *member ) {
	//m_finishedSig.connect(receiver, member);
	QObject::connect(this, SIGNAL(finished()), receiver, member);
}

/** Should be called when the search finished. */
void CSwordModuleSearch::searchFinished() {
	//m_finishedSig.activate();
	emit finished();
}

bool CSwordModuleSearch::modulesHaveIndices( const QList<CSwordModuleInfo*>& modules )
{
	bool hasIndices = true;
	QList<CSwordModuleInfo*>::const_iterator end_it = modules.end();
	for( QList<CSwordModuleInfo*>::const_iterator it = modules.begin(); it != end_it; ++it) {
		if (!(*it)->hasIndex()) {
			hasIndices = false;
			break;
		}
	}
	return hasIndices;
}
