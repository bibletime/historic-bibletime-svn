/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2006 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#ifndef UTIL_FILESDIRECTORYUTIL_H
#define UTIL_FILESDIRECTORYUTIL_H

#include <QString>
#include <QDir>

namespace util {

namespace filesystem {

/**
 * Tools for working with directories.
 * @author The BibleTime team <info@bibletime.info>
*/
class DirectoryUtil {
private:
	DirectoryUtil() {};
	~DirectoryUtil() {};

public:
	/** Removes the given dir with all it's files and subdirs.
	 *
	 * TODO: Check if it's suitable for huge dir trees, as it holds a QDir object
	 * for each of it at the same time in the deepest recursion.
	 * For really deep dir tree this may lead to a stack overflow.
	 */
	static void removeRecursive(const QString dir);

	/** Returns the size of the directory including the size of all its files
	 * and its subdirs.
	 * 
	 * TODO: Check if it's suitable for huge dir trees, as it holds a QDir object
	 * for each of it at the same time in the deepest recursion.
	 * For really deep dir tree this may lead to a stack overflow.
	 *
	 * @return The size of the dir in bytes
	 */
	static unsigned long getDirSizeRecursive(const QString dir);
	
	/** Call this on program startup. Will cache a few directories like icon directory */
	static void initDirectoryCache(QString executableFilePath);
	
	/** Return the path to the icons. initDirectoryCache must be called before. */
	static QDir getIconDir(void);
};
	
}

}

#endif
