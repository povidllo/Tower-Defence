#include "../../include/Utils/TextureUtils.h"

bool TextureUtils::isPngBySignature(const std::string &path) {
	std::ifstream file(path, std::ios::binary);
	if (!file)
		return false;

	unsigned char pngSignature[8] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
	unsigned char header[8] = {};

	file.read(reinterpret_cast<char *>(header), 8);

	return std::equal(std::begin(pngSignature), std::end(pngSignature), header);
}

QString TextureUtils::returnRelativeOrAbsolutePath(const QString &path) {
	QString absolutePath = QFileInfo(path).absoluteFilePath();

	QDir baseDir(QDir::currentPath());

	QString relativePath = baseDir.relativeFilePath(absolutePath);

	QString pathToStore;
	if (!relativePath.startsWith("..") && !QDir::isAbsolutePath(relativePath)) {
		pathToStore = relativePath;
	} else {
		pathToStore = absolutePath;
	}
	return pathToStore;
}
