#include "../../include/Utils/TextureManager.h"

TextureManager &TextureManager::instance() {
	static TextureManager inst;
	return inst;
}

void TextureManager::registerTexture(int id, const std::string &path) {
	paths[id] = path;
	pixmaps[id] = QPixmap(QString::fromStdString(path));
}

const QPixmap &TextureManager::get(int id) const {
	static QPixmap empty;
	auto it = pixmaps.find(id);
	return (it != pixmaps.end()) ? it->second : empty;
}

bool TextureManager::has(int id) const { return paths.find(id) != paths.end(); }

const std::string &TextureManager::getPath(int id) const {
	static std::string empty;
	auto it = paths.find(id);
	return (it != paths.end()) ? it->second : empty;
}

TextureManager::json TextureManager::toJson() const {
	json arr = json::array();
	for (const auto &[id, path]: paths) {
		arr.push_back({{"id", id}, {"path", path}});
	}
	return arr;
}

void TextureManager::fromJson(const json &j) {
	paths.clear();
	pixmaps.clear();

	for (auto &item: j) {
		int id = item.value("id", -1);
		std::string path = item.value("path", "");

		if (id >= 0 && !path.empty()) {
			registerTexture(id, path);
		}
	}
}

int TextureManager::getImageSize() const { return imageSize; }

const std::unordered_map<int, std::string> &TextureManager::getAllTextures() const { return paths; }

void TextureManager::setCurrentTexture(const int id) {
	if (has(id)) {
		currentTextureId = id;
	}
}

int TextureManager::getCurrentTexture() const { return currentTextureId; }
