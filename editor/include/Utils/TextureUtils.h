#ifndef TOWERDEFENCE_IMAGEUTILS_H
#define TOWERDEFENCE_IMAGEUTILS_H
#include <fstream>
#include <iosfwd>

class TextureUtils {
public:
	static bool isPngBySignature(const std::string &path);
};

#endif // TOWERDEFENCE_IMAGEUTILS_H
