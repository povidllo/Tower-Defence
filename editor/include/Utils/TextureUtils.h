#ifndef TOWERDEFENCE_IMAGEUTILS_H
#define TOWERDEFENCE_IMAGEUTILS_H
#include <fstream>
#include <iosfwd>


class TextureUtils {
public:
	static bool isPngBySignature(const std::string &path) {
		std::ifstream file(path, std::ios::binary);
		if (!file)
			return false;

		unsigned char pngSignature[8] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
		unsigned char header[8] = {};

		file.read(reinterpret_cast<char *>(header), 8);

		return std::equal(std::begin(pngSignature), std::end(pngSignature), header);
	}
};


#endif // TOWERDEFENCE_IMAGEUTILS_H
