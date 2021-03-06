/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2008 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

//Backend
#include "cchapterdisplay.h"
#include "cdisplayrendering.h"
#include "backend/keys/cswordversekey.h"
#include "backend/drivers/cswordbiblemoduleinfo.h"

const QString Rendering::CChapterDisplay::text( const ListCSwordModuleInfo& modules, const QString& keyName, const CSwordBackend::DisplayOptions displayOptions, const CSwordBackend::FilterOptions filterOptions ) {
	Q_ASSERT( modules.count() >= 1 );
	Q_ASSERT( !keyName.isEmpty() );

	CSwordModuleInfo* module = modules.first();

	if (modules.count() == 1) module->module()->setSkipConsecutiveLinks( true ); //skip empty, linked verses

	CTextRendering::KeyTreeItem::Settings settings;
	settings.keyRenderingFace =
		displayOptions.verseNumbers
		? CTextRendering::KeyTreeItem::Settings::SimpleKey
		: CTextRendering::KeyTreeItem::Settings::NoKey;

	QString startKey = keyName;
	QString endKey = startKey;

	//check whether there's an intro we have to include
	Q_ASSERT((module->type() == CSwordModuleInfo::Bible));

	if (module->type() == CSwordModuleInfo::Bible) {
		((sword::VerseKey*)(module->module()->getKey()))->Headings(1); //HACK: enable headings for VerseKeys

		CSwordBibleModuleInfo* bible = dynamic_cast<CSwordBibleModuleInfo*>(module);
		Q_ASSERT(bible);

		CSwordVerseKey k1(module);
		k1.Headings(1);
		k1.key(keyName);

		if (k1.Chapter() == 1)	k1.Chapter(0); //Chapter 1, start with 0:0, otherwise X:0
		
		k1.Verse(0);

		startKey = k1.key();
		
		if (k1.Chapter() == 0) k1.Chapter(1);
		k1.Verse(bible->verseCount(k1.book(), k1.Chapter()));
		endKey = k1.key();
	}

	CDisplayRendering render(displayOptions, filterOptions);
	return render.renderKeyRange( startKey, endKey, modules, keyName, settings );
}
