/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2008 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/



#ifndef CHTMLREADDISPLAY_H
#define CHTMLREADDISPLAY_H

//BibleTime includes
#include "creaddisplay.h"
//#include "frontend/ctooltip.h"

//Qt includes
//Added by qt3to4:
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QTimerEvent>

//KDE includes
#include <khtml_part.h>
#include <khtmlview.h>
#include <kparts/browserextension.h>

class CHTMLReadDisplayView;

class QWidget;

/** The implementation for the HTML read display.
  * @author The BibleTime team
  */
class CHTMLReadDisplay : public KHTMLPart, public CReadDisplay {
	Q_OBJECT
public:
	//reimplemented functions from CDisplay
	/**
	* Returns the right text part in the specified format.
	*/
	virtual const QString text( const CDisplay::TextType format = CDisplay::HTMLText, const CDisplay::TextPart part = CDisplay::Document );

	/**
	* Sets the new text for this display widget.
	*/
	virtual void setText( const QString& newText );
	virtual bool hasSelection();
	/**
	* Reimplementation.
	*/
	virtual QScrollArea* view();
	virtual void selectAll();
	virtual void moveToAnchor( const QString& anchor );

	virtual void zoomIn();
	virtual void zoomOut();
	virtual void openFindTextDialog();

	virtual QMap<CDisplay::NodeInfoType, QString> getCurrentNodeInfo() {
		return m_nodeInfo;
	}

protected:
	friend class CDisplay;
	CHTMLReadDisplay( CReadWindow* readWindow, QWidget* parent = 0 );
	virtual ~CHTMLReadDisplay();

	virtual bool urlSelected(const QString &url, int button, int state,
                            const QString &_target,
                            const KParts::OpenUrlArguments& args = KParts::OpenUrlArguments(),
                            const KParts::BrowserArguments& browserArgs = KParts::BrowserArguments() );
//	 const QString& url, int button, int state, const QString& _target, KParts::OpenUrlArguments args, KParts::BrowserArguments b_args);
	/**
	* Reimplementation.
	*/
	virtual void khtmlMouseReleaseEvent( khtml::MouseReleaseEvent* event );
	virtual void khtmlMousePressEvent( khtml::MousePressEvent* event );
	virtual void khtmlMouseMoveEvent( khtml::MouseMoveEvent* event );
	virtual void timerEvent(QTimerEvent *event);


	struct DNDData {
		bool mousePressed;
		bool isDragging;
		DOM::Node node;
		DOM::DOMString anchor;
		QString selection;
		QPoint startPos;
		enum DragType {
			Link,
			Text
		} dragType;
	}
	m_dndData;

	QMap<NodeInfoType, QString> m_nodeInfo;
	int m_magTimerId;

private:
	CHTMLReadDisplayView* m_view;
	struct URLWorkaroundData {
		bool doWorkaround;
		QString url;
		int button;
		int state;
		QString target;
		KParts::OpenUrlArguments args;
		KParts::BrowserArguments b_args;

		QString module;
		QString key;
	}
	m_urlWorkaroundData;

	DOM::Node m_previousEventNode;
	QString m_currentAnchorCache;

protected:
	void slotGoToAnchor(const QString& anchor);
};

class CHTMLReadDisplayView : public KHTMLView, public CPointers {
	Q_OBJECT
protected: // Protected methods
	friend class CHTMLReadDisplay;


	CHTMLReadDisplayView(CHTMLReadDisplay* display, QWidget* parent);
	/**
	* Reimplementation from QScrollArea. Sets the right slots
	*/
	virtual bool event(QEvent*);
	/**
	* Reimplementation from QScrollArea.
	*/
	virtual void dropEvent( QDropEvent* );
	/**
	* Reimplementatiob from QScrollArea.
	*/
	virtual void dragEnterEvent( QDragEnterEvent* );

protected slots: // Protected slots
	/**
	* Opens the popupmenu at the given position.
	*/
	void popupMenu( const QString&, const QPoint& );

private:
	CHTMLReadDisplay* m_display;
};

#endif
