#include "Player.h"

Player::Player(const json &j) {
	fromJson(j);
}

Player::json Player::toJson() const {
	return {
		{"playerName", playerName},
		{"startCurrency", startCurrency},
		{"hp", hp},
	};
}

void Player::fromJson(const json &j) {
	playerName = j.value("playerName", playerName);
	startCurrency = j.value("startCurrency", startCurrency);
	hp = j.value("hp", hp);
}
