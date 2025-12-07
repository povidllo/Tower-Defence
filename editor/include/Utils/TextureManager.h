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

	static TextureManager &instance();

	TextureManager(const TextureManager &) = delete;

	TextureManager &operator=(const TextureManager &) = delete;

	void registerTexture(int id, const std::string &path);

	const QPixmap &get(int id) const;

	bool has(int id) const;

	const std::string &getPath(int id) const;

	json toJson() const override;

	void fromJson(const json &j) override;

	int getImageSize() const;

	const std::unordered_map<int, std::string> &getAllTextures() const;

	std::unordered_map<int, QPixmap> pixmaps;

	void setCurrentTexture(const int id);

	int getCurrentTexture() const;

private:
	TextureManager() = default;

	int imageSize = 32;

	std::unordered_map<int, std::string> paths;

	int currentTextureId = 0;
};

#endif
