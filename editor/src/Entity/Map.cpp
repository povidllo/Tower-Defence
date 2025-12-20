#include "../../include/Entity/Map.h"

Map::Map(std::string name, const int height, const int width) : name(std::move(name)), height(height), width(width),
																tiles(height, std::vector<int>(width, 0)) {
}

Map::Map(const json &j) { Map::fromJson(j); }

json Map::toJson() const {
	json tileArray = json::array();
	for (const auto &row: tiles) {
		tileArray.push_back(row);
	}

	json wavesArray = json::array();
	for (const auto &wave: waves) {
		wavesArray.push_back(wave->toJson());
	}

	json spotArrays = json::array();
	for (const auto &spot: spots) {
		spotArrays.push_back(spot->toJson());
	}

	return {
		{"name", name},
		{"height", height},
		{"width", width},
		{"startCurrency", startCurrency},
		{"hp", hp},
		{"waves", wavesArray},
		{"spots", spotArrays},
		{"tiles", tileArray}
	};
}

void Map::fromJson(const json &j) {
	name = j.value("name", name);
	height = j.value("height", height);
	width = j.value("width", width);

	startCurrency = j.value("startCurrency", startCurrency);
	hp = j.value("hp", hp);

	finalMapImagePath = j.value("finalMapImagePath", finalMapImagePath);

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

	if (j.contains("waves") && j["waves"].is_array()) {
		waves.clear();
		for (const auto &waveJson: j["waves"]) {
			auto wave = std::make_shared<WaveSample>(waveJson);
			waves.push_back(wave);
		}
	}

	if (j.contains("spots") && j["spots"].is_array()) {
		spots.clear();
		for (const auto &spotJson: j["spots"]) {
			auto spot = std::make_shared<TowerSample>(spotJson);
			spots.push_back(spot);
		}
	}
}

int Map::getTile(const int x, const int y) const { return tiles[y][x]; }

void Map::setTile(const int x, const int y, const int tileId) { tiles[y][x] = tileId; }

std::vector<std::vector<int> > &Map::getTiles() { return tiles; }

std::string Map::getName() { return name; }

void Map::setName(const std::string &n) { name = n; }

int Map::getHeight() const { return height; }

int Map::getWidth() const { return width; }

double Map::getHp() {
	return hp;
}

void Map::setHp(double hp) {
	this->hp = hp;
}

double Map::getStartCurrency() {
	return startCurrency;
}

void Map::setStartCurrency(double currency) {
	startCurrency = currency;
}

std::string Map::getFinalMapImagePath() {
	return finalMapImagePath;
}
