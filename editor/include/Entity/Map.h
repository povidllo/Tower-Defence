#ifndef TOWERDEFENCE_MAP_H
#define TOWERDEFENCE_MAP_H
#include <utility>

#include "Serializable.h"


class Map : protected ISerializable {
public:
	using json = nlohmann::json;

	Map(std::string name, const int height, const int width) :
		name(std::move(name)), height(height), width(width), tiles(height, std::vector<int>(width, 0)) {}

	explicit Map(const json &j) { Map::fromJson(j); }

	json toJson() const override {
		json tileArray = json::array();
		for (const auto &row: tiles) {
			tileArray.push_back(row);
		}

		return {{"name", name}, {"height", height}, {"width", width}, {"tiles", tileArray}};
	}

	void fromJson(const json &j) override {
		name = j.value("name", name);
		height = j.value("height", height);
		width = j.value("width", width);

		tiles.assign(height, std::vector<int>(width, 0));

		if (j.contains("tiles")) {
			const auto &tileArray = j["tiles"];
			for (int y = 0; y < std::min<int>(tileArray.size(), height); ++y) {
				const auto &row = tileArray[y];
				for (int x = 0; x < std::min<int>(row.size(), width); ++x) {
					tiles[y][x] = row[x].get<int>();
				}
			}
		}
	}

	int getTile(const int x, const int y) const { return tiles[y][x]; }
	void setTile(const int x, const int y, const int tileId) { tiles[y][x] = tileId; }

	std::vector<std::vector<int>> &getTiles() { return tiles; }

	[[nodiscard]] std::string getName() { return name; }

	void setName(const std::string &n) { name = n; }

	int getHeight() const { return height; }

	int getWidth() const { return width; }

private:
	std::string name;
	int height;
	int width;

	std::vector<std::vector<int>> tiles;
};


#endif // TOWERDEFENCE_MAP_H
