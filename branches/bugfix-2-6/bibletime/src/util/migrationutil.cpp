/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2010 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#include "util/migrationutil.h"

#include <QMessageBox>
#include <QSettings>
#include "backend/config/cbtconfig.h"
#include "util/directory.h"

// Sword includes:
#include "swversion.h"


namespace util {
namespace migration {

void checkMigration() {
    if (CBTConfig::get(CBTConfig::bibletimeVersion) != BT_VERSION) {
        sword::SWVersion lastVersion(CBTConfig::get(CBTConfig::bibletimeVersion).toUtf8());
        //lastVersion will be 0.0, if it was an old KDE install,
        //because the config could not be found yet
        if (lastVersion < sword::SWVersion("1.7.0") ) {
            tryMigrationFromKDE3(); //
        }
    }
}

// Migration code for KDE 4 port, moves from old config dir to ~/.bibletime/
void tryMigrationFromKDE3() {
    namespace DU = util::directory;

    //List of potential old KDE directories to load data from.
    QStringList searchDirs;
    searchDirs << "/.kde" << "/.kde3" << "/.kde3.5";
    searchDirs << "/.kde3.4" << "/.kde3.3" << "/.kde3.2";
    searchDirs << "/.kde3.1" << "/.kde3.0";

    foreach (QString searchDir, searchDirs) {
        QString currSearch = QDir::homePath() + searchDir;
        QDir searchHome(currSearch);
        QFile oldRc(currSearch + "/share/config/bibletimerc");
        //Copy our old bibletimerc into the new KDE4 directory.
        QString newRcLoc(DU::getUserBaseDir().absolutePath() + "/bibletimerc");
        QFile newRc(newRcLoc);

        //Migrate only if the old config exists and the new doesn't
        if (oldRc.exists() && !newRc.exists()) {
            QMessageBox msg (QMessageBox::Question, QObject::tr("Settings Migration"),
                             QObject::tr("It appears you have a BibleTime configuration from KDE 3 stored in %1, and you have not migrated it to this version.  Would you like to import it?").arg(currSearch), QMessageBox::Yes | QMessageBox::No);
            int result = msg.exec();
            if (result != QMessageBox::Yes) {
                break;
            }
            oldRc.copy(newRcLoc);
            QFile oldBookmarks(currSearch + "/share/apps/bibletime/bookmarks.xml");
            if (oldBookmarks.exists()) {
                QString newBookmarksLoc(DU::getUserBaseDir().absolutePath() + "/" + "bookmarks.xml");
                QFile newBookmarks(newBookmarksLoc);
                newBookmarks.remove();
                oldBookmarks.copy(newBookmarksLoc);
            }
            QDir sessionDir(currSearch + "/share/apps/bibletime/sessions");
            if (sessionDir.exists()) {
                DU::copyRecursive(
                    sessionDir.absolutePath(),
                    DU::getUserSessionsDir().absolutePath());
            }
            else {
                QDir oldSessionDir(currSearch + "/share/apps/bibletime/profiles");
                if (oldSessionDir.exists()) {
                    DU::copyRecursive(
                        oldSessionDir.absolutePath(),
                        DU::getUserSessionsDir().absolutePath());
                }
            }
            //We found at least a config file, so we are done
            //searching for migration data.
            break;
        }
    }
    CBTConfig::syncConfig();
}

} // namespace migration
} // namespace util
