/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2010 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#ifndef CREADDISPLAY_H
#define CREADDISPLAY_H

#include "frontend/display/cdisplay.h"

#include <QString>
#include "backend/managers/cswordbackend.h"


/** The base class for all read-only widgets like KHTMLView.
  *@author The BibleTime team
  */
class CReadDisplay :  public CDisplay {
    public:
        /**
        * Returns true if the display has an active anchor.
        */
        bool hasActiveAnchor();
        /**
        * Returns the current active anchor.
        */
        const QString& activeAnchor();
        /**
        * Moves the widget to the given anchor.
        */
        virtual void moveToAnchor( const QString& ) = 0;
        virtual void print(const CDisplay::TextPart, CSwordBackend::DisplayOptions displayOptions,
                           CSwordBackend::FilterOptions filterOptions);

        void setMouseTracking(const bool trackingEnabled) {
            m_useMouseTracking = trackingEnabled;
        };
        bool getMouseTracking() const {
            return m_useMouseTracking;
        };

    protected: // Protected methods
        friend class BtHtmlReadDisplay;
        friend class BtHtmlReadDisplayView;
        friend class BtHtmlJsObject;
        friend class CDisplay;
        friend class CHTMLReadDisplay;
        friend class CHTMLReadDisplayView;

        CReadDisplay( CReadWindow* readWindow );
        ~CReadDisplay();

        /**
        * Sets the current anchor to the parameter.
        */
        void setActiveAnchor( const QString& );

    private: // Public attributes
        /**
        * The member which hols the current anchor.
        */

        QString m_activeAnchor;

        bool m_useMouseTracking;
};

#endif
