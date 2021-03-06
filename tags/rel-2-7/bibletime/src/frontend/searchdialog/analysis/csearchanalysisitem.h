/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2010 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#ifndef SEARCHCSEARCHANALYSISITEM_H
#define SEARCHCSEARCHANALYSISITEM_H

#include <QGraphicsRectItem>

#include <QGraphicsScene>


class CSwordModuleInfo;

namespace Search {

/**
    @author The BibleTime team <info@bibletime.info>
*/
class CSearchAnalysisItem : public QGraphicsRectItem {
    public:

        CSearchAnalysisItem(const int moduleCount, const QString& bookname, double *scaleFactor, QList<CSwordModuleInfo*>* modules);
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
        * Does one of the modules contain hits?
        */
        bool hasHitsInAnyModule();
        /**
        * Returns the width of this item.
        */
        virtual int width();
        /**
        * Returns the tooltip for this item.
        */
        const QString getToolTip();

    private:
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);

        QList<CSwordModuleInfo*>* m_moduleList;
        double *m_scaleFactor;
        QString m_bookName;
        int m_moduleCount;
        QVector<int> m_resultCountArray;
        QPixmap* m_bufferPixmap;

};

}

#endif
