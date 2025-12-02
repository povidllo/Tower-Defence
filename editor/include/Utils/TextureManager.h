#ifndef TOWERDEFENCE_TEXTUREMANAGER_H
#define TOWERDEFENCE_TEXTUREMANAGER_H

#include <QPixmap>
#include <string>
#include <unordered_map>

#include "Serializable.h"
#include "nlohmann/json.hpp"

class TextureManager : protected ISerializable {
public:
	using json = nlohmann::json;

	static TextureManager &instance() {
		static TextureManager inst;
		return inst;
	}

	TextureManager(const TextureManager &) = delete;
	TextureManager &operator=(const TextureManager &) = delete;

	void registerTexture(int id, const std::string &path) {
		paths[id] = path;
		pixmaps[id] = QPixmap(QString::fromStdString(path));
	}

	const QPixmap &get(int id) const {
		static QPixmap empty;
		auto it = pixmaps.find(id);
		return (it != pixmaps.end()) ? it->second : empty;
	}

	bool has(int id) const { return paths.find(id) != paths.end(); }

	const std::string &getPath(int id) const {
		static std::string empty;
		auto it = paths.find(id);
		return (it != paths.end()) ? it->second : empty;
	}

	json toJson() const override {
		json arr = json::array();
		for (const auto &[id, path]: paths) {
			arr.push_back({{"id", id}, {"path", path}});
		}
		return arr;
	}

	void fromJson(const json &j) override {
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

	int getImageSize() const { return imageSize; }

	const std::unordered_map<int, std::string> &getAllTextures() const { return paths; }

	std::unordered_map<int, QPixmap> pixmaps;

	void setCurrentTexture(const int id) {
		if (has(id)) {
			currentTextureId = id;
		}
	}

	int getCurrentTexture() const { return currentTextureId; }

private:
	TextureManager() = default;

	int imageSize = 32;

	std::unordered_map<int, std::string> paths;

	int currentTextureId = 0;
};

#endif
