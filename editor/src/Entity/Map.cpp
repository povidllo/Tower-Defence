#include "../../include/Entity/Map.h"

#include <algorithm>

Map::Map(std::string name, const int height, const int width) : name(std::move(name)), height(height), width(width),
																tiles(height, std::vector<int>(width, 0)) {
	playerSpots.resize(1);
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

	json onlineConfig = {
		{"enabled", onlineEnabled},
		{"maxPlayers", maxPlayers}
	};

	json playerSpotsArray = json::array();
	for (const auto &playerSpotList : playerSpots) {
		json spotsForPlayer = json::array();
		for (const auto &spotName : playerSpotList) {
			spotsForPlayer.push_back(spotName);
		}
		playerSpotsArray.push_back(spotsForPlayer);
	}
	onlineConfig["playerSpots"] = playerSpotsArray;

	json teamsArray = json::array();
	for (const auto &team : teams) {
		teamsArray.push_back(team->toJson());
	}
	onlineConfig["teams"] = teamsArray;

	return {
		{"name", name},
		{"height", height},
		{"width", width},
		{"startCurrency", startCurrency},
		{"hp", hp},
		{"waves", wavesArray},
		{"spots", spotArrays},
		{"tiles", tileArray},
		{"online", onlineConfig}
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

	if (j.contains("online") && j["online"].is_object()) {
		const auto &onlineJson = j["online"];
		onlineEnabled = onlineJson.value("enabled", false);
		maxPlayers = onlineJson.value("maxPlayers", 1);

		playerSpots.clear();
		if (onlineJson.contains("playerSpots") && onlineJson["playerSpots"].is_array()) {
			for (const auto &playerSpotsJson : onlineJson["playerSpots"]) {
				std::vector<std::string> spotsForPlayer;
				if (playerSpotsJson.is_array()) {
					for (const auto &spotName : playerSpotsJson) {
						spotsForPlayer.push_back(spotName.get<std::string>());
					}
				}
				playerSpots.push_back(spotsForPlayer);
			}
		}

		teams.clear();
		if (onlineJson.contains("teams") && onlineJson["teams"].is_array()) {
			for (const auto &teamJson : onlineJson["teams"]) {
				teams.push_back(std::make_shared<Team>(teamJson));
			}
		}
		clampTeamsToMax();
	} else {
		playerSpots.clear();
		playerSpots.resize(maxPlayers);
		teams.clear();
	}
}

void Map::syncOnlineTeamsWithPlayerCount(const int effectiveMaxPlayers, const bool resizePlayerSpots) {
	if (effectiveMaxPlayers < 1) {
		return;
	}

	if (resizePlayerSpots) {
		while (playerSpots.size() < static_cast<size_t>(effectiveMaxPlayers)) {
			playerSpots.emplace_back();
		}
		while (playerSpots.size() > static_cast<size_t>(effectiveMaxPlayers)) {
			playerSpots.pop_back();
		}
	}

	if (!onlineEnabled) {
		return;
	}

	const auto slotId = [](const int oneBased) {
		return "player_" + std::to_string(oneBased);
	};

	for (int j = effectiveMaxPlayers + 1; j <= 64; ++j) {
		const std::string excess = slotId(j);
		for (auto &t : teams) {
			t->removePlayer(excess);
		}
	}

	if (teams.empty()) {
		for (int i = 1; i <= effectiveMaxPlayers; ++i) {
			auto team = std::make_shared<Team>("Team " + std::to_string(i));
			team->addPlayer(slotId(i));
			teams.push_back(team);
		}
		clampTeamsToMax();
		return;
	}

	for (int i = 1; i <= effectiveMaxPlayers; ++i) {
		const std::string id = slotId(i);
		int occurrences = 0;
		for (const auto &t : teams) {
			for (const auto &p : t->getPlayers()) {
				if (p.getPlayerName() == id) {
					++occurrences;
				}
			}
		}
		if (occurrences == 0) {
			if (static_cast<int>(teams.size()) >= getMaxOnlineTeams()) {
				teams.back()->addPlayer(id);
			} else {
				auto team = std::make_shared<Team>("Team " + std::to_string(static_cast<int>(teams.size()) + 1));
				team->addPlayer(id);
				teams.push_back(team);
			}
		}
	}

	clampTeamsToMax();
}

int Map::findTeamIndexForPlayer(const std::string &playerId) const {
	for (size_t ti = 0; ti < teams.size(); ++ti) {
		for (const auto &p : teams[ti]->getPlayers()) {
			if (p.getPlayerName() == playerId) {
				return static_cast<int>(ti);
			}
		}
	}
	return 0;
}

void Map::applyPlayerTeamAssignments(const std::vector<int> &playerIndexToTeamIndex) {
	if (teams.empty()) {
		return;
	}
	for (auto &t : teams) {
		t->clearPlayers();
	}
	for (size_t i = 0; i < playerIndexToTeamIndex.size(); ++i) {
		const int ti = playerIndexToTeamIndex[i];
		if (ti < 0 || static_cast<size_t>(ti) >= teams.size()) {
			continue;
		}
		const std::string id = "player_" + std::to_string(static_cast<int>(i) + 1);
		teams[static_cast<size_t>(ti)]->addPlayer(id);
	}
}

void Map::clampTeamsToMax() {
	const int kMax = getMaxOnlineTeams();
	while (static_cast<int>(teams.size()) > kMax) {
		const auto &from = teams.back();
		auto &into = teams[static_cast<size_t>(kMax) - 1];
		for (const auto &pid : from->getPlayers()) {
			bool already = false;
			for (const auto &p : into->getPlayers()) {
				if (p.getPlayerName() == pid.getPlayerName()) {
					already = true;
					break;
				}
			}
			if (!already) {
				into->addPlayer(pid.getPlayerName());
			}
		}
		teams.pop_back();
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

void Map::hydrateSpotTemplates(const std::vector<std::shared_ptr<TowerSample> > &projectTowers) {
	for (auto &spot : spots) {
		if (!spot->isMapSpotReference()) {
			continue;
		}
		for (const auto &t : projectTowers) {
			if (t->getName() == spot->getTowerTemplateName()) {
				spot->applyTemplate(*t);
				break;
			}
		}
	}
}
