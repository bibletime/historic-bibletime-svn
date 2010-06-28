/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2010 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#ifndef BTSIGNAL_H
#define BTSIGNAL_H

#include <QObject>


/**
* BtSignal
* The purpose of this class is to emit Qt signals for other classes
* that are not derived from QObject. It can be used as a member
* variable of those classes.
*
* There are some classes it is not possible to derive from QObject and
* have the signals work. Certain multiple inheritance classes which cannot
* have QObject as the first derived class, cannot use Qt signals.
*/
class BtSignal : public QObject {
        Q_OBJECT

    public:
        inline BtSignal(QObject *parent = 0) : QObject(parent) {};

        /**
          Immediately emits the changed() signal.
        */
        inline void emitChanged() { emit changed(); }

    signals:
        void changed();
};
#endif
