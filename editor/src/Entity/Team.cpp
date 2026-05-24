#include <algorithm>
#include "Team.h"

Team::json Team::toJson() const {
	json arr = json::array();

	for (const auto &p : players) {
		arr.push_back(p.toJson());
	}

	return {
	        {"teamName", teamName},
			{"hp", hp},
			{"players", arr},
		};
}

void Team::fromJson(const json &j) {
	teamName = j.value("teamName", teamName);
	const bool hasTeamHp = j.contains("hp");
	hp = j.value("hp", hp);

	players.clear();

	if (j.contains("players") && j["players"].is_array()) {
		for (const auto &item : j["players"]) {
			if (item.is_object()) {
				if (!hasTeamHp && item.contains("hp")) {
					hp = item.value("hp", hp);
				}
				players.emplace_back(item);
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