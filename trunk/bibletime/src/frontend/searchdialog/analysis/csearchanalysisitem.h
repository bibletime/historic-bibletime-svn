//
// C++ Interface: csearchanalysisitem
//
// Description: 
//
//
// Author: The BibleTime team <info@bibletime.info>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SEARCHCSEARCHANALYSISITEM_H
#define SEARCHCSEARCHANALYSISITEM_H

#include <QGraphicsRectItem>

namespace Search {

/**
	@author The BibleTime team <info@bibletime.info>
*/
class CSearchAnalysisItem : public QGraphicsRectItem
{
public:
    CSearchAnalysisItem();
	CSearchAnalysisItem(QGraphicsScene* parent, const int moduleCount, const QString& bookname, double *scaleFactor, ListCSwordModuleInfo* modules);
    ~CSearchAnalysisItem();
	/**
	* Sets the resultcount of this item
	*/
	void setCountForModule( const int moduleIndex, const int count);

	/**
	* Returns the resultcount of this item
	*/
	int getCountForModule( const int moduleIndex);
	/**
	* Returns the width of this item.
	*/
	virtual int width();
	/**
	* Returns the tooltip for this item.
	*/
	const QString getToolTip();

private:
	virtual void draw (QPainter & painter);

	ListCSwordModuleInfo* m_moduleList;
	double *m_scaleFactor;
	QString m_bookName;
	int m_moduleCount;
	Q3MemArray<int> m_resultCountArray;
	QPixmap* m_bufferPixmap;

};

}

#endif