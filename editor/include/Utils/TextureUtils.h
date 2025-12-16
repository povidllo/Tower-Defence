#ifndef TOWERDEFENCE_IMAGEUTILS_H
#define TOWERDEFENCE_IMAGEUTILS_H
#include <fstream>
#include <iosfwd>
#include <qcoreapplication.h>
#include <qdir.h>
#include <qstring.h>

class TextureUtils {
public:
	static bool isPngBySignature(const std::string &path);

	static QString returnRelativeOrAbsolutePath(const QString &path);
};

#endif // TOWERDEFENCE_IMAGEUTILS_H
