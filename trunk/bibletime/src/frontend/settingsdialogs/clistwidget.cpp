/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2008 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#include "clistwidget.h"

CListWidget::CListWidget(QWidget* parent)
 : QListWidget(parent)
{
	setMaximumHeight(95);
}

CListWidget::~CListWidget() 
{
}

QSize CListWidget::sizeHint () const
{
	QSize size = QListWidget::sizeHint();
	size.setWidth(100);
	return size;
}