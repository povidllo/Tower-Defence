#include <algorithm>

#include "Team.h"

Team::json Team::toJson() const {
	json arr = json::array();
	for (const auto &p : players) {
		arr.push_back(p);
	}
	return {
		{"teamName", teamName},
		{"players", arr},
	};
}

void Team::fromJson(const json &j) {
	teamName = j.value("teamName", teamName);
	players.clear();
	if (j.contains("players") && j["players"].is_array()) {
		for (const auto &item : j["players"]) {
			if (item.is_string()) {
				players.push_back(item.get<std::string>());
			}
		}
	}
}

bool Team::removePlayer(const std::string &playerName) {
	auto it = std::find(players.begin(), players.end(), playerName);
	if (it == players.end()) {
		return false;
	}
	players.erase(it);
	return true;
}
