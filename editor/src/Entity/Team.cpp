#include <algorithm>
#include "Team.h"

Team::json Team::toJson() const {
	json arr = json::array();

	for (const auto &p : players) {
		arr.push_back(p.toJson());
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
			if (item.is_object()) {
				players.emplace_back(item); // Player(const json &)
			}
		}
	}
}

bool Team::removePlayer(const std::string &playerName) {
	auto it = std::find_if(players.begin(), players.end(),
		[&](const Player &p) {
			return p.getPlayerName() == playerName;
		});

	if (it == players.end()) {
		return false;
	}

	players.erase(it);
	return true;
}