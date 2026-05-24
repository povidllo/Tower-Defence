#include "Player.h"

#include <algorithm>

Player::Player(const json &j) {
	fromJson(j);
}

Player::json Player::toJson() const {
	json abilities = json::array();
	for (const auto &abilityName: abilityNames) {
		abilities.push_back(abilityName);
	}
	return {
		{"playerName", playerName},
		{"startCurrency", startCurrency},
		{"abilities", abilities},
	};
}

void Player::fromJson(const json &j) {
	playerName = j.value("playerName", playerName);
	startCurrency = j.value("startCurrency", startCurrency);
	if (j.contains("abilities") && j["abilities"].is_array()) {
		abilityNames.clear();
		for (const auto &abilityName: j["abilities"]) {
			if (abilityName.is_string()) {
				abilityNames.push_back(abilityName.get<std::string>());
			}
		}
	}
}

void Player::addAbility(const std::string &name) {
	if (std::find(abilityNames.begin(), abilityNames.end(), name) == abilityNames.end()) {
		abilityNames.push_back(name);
	}
}

bool Player::removeAbility(const std::string &name) {
	const auto it = std::remove(abilityNames.begin(), abilityNames.end(), name);
	if (it == abilityNames.end()) {
		return false;
	}
	abilityNames.erase(it, abilityNames.end());
	return true;
}

void Player::renameAbilityReference(const std::string &oldName, const std::string &newName) {
	for (auto &abilityName: abilityNames) {
		if (abilityName == oldName) {
			abilityName = newName;
		}
	}
}

void Player::removeAbilityReference(const std::string &name) {
	removeAbility(name);
}
