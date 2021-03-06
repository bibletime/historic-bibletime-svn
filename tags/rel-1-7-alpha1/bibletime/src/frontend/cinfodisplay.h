//
// C++ Interface: cinfodisplay
//
// Description:
//
//
// Author: The BibleTime team <info@bibletime.info>, (C) 2004,2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef CINFODISPLAY_H
#define CINFODISPLAY_H

//Backend
#include "backend/rendering/ctextrendering.h"

//Qt includes
#include <QWidget>
#include <QHash>
#include <QPair>

//class forward declarations
class CReadDisplay;
class KAction;

namespace InfoDisplay {

	/**
	@author The BibleTime team
	*/
class CInfoDisplay : public QWidget {
		Q_OBJECT
public:
		enum InfoType {
			Abbreviation,
			CrossReference,
			Footnote,
			Lemma,
			Morph,
			WordTranslation,
			WordGloss
		};

		typedef QPair<InfoType, QString> InfoData;
		typedef QList<InfoData> ListInfoData;

		CInfoDisplay(QWidget *parent = 0);
		virtual ~CInfoDisplay();

		void setInfo(const InfoType, const QString& data);
		void setInfo(const ListInfoData&);
		void clearInfo();

protected:
		const QString decodeAbbreviation( const QString& data );
		const QString decodeCrossReference( const QString& data );
		const QString decodeFootnote( const QString& data );
		const QString decodeStrongs( const QString& data );
		const QString decodeMorph( const QString& data );
		const QString getWordTranslation( const QString& data );

protected slots:
		void lookup(const QString &, const QString &);

private:
		CReadDisplay* m_htmlPart;
		KAction* m_copyAction;
	};

} //end of InfoDisplay namespace

#endif
