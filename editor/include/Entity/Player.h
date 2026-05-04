#ifndef TOWERDEFENCE_PLAYERS_H
#define TOWERDEFENCE_PLAYERS_H

#include <string>

#include "Serializable.h"

class Player : protected ISerializable {
public:
	using json = nlohmann::ordered_json;

	Player(std::string playerName, const double startCurrency, const double hp) :
		playerName{std::move(playerName)}, startCurrency{startCurrency}, hp{hp} {}

	json toJson() const override;
	void fromJson(const json &j) override;

	const std::string &getPlayerName() const { return playerName; }
	void setPlayerName(std::string name) { playerName = std::move(name); }

	double getStartCurrency() const { return startCurrency; }
	void setStartCurrency(const double start_currency) { startCurrency = start_currency; }
	double getHp() const { return hp; }
	void setHp(const double hp) { this->hp = hp; }

private:
	std::string playerName;
	double startCurrency{0};
	double hp{0};

};


#endif // TOWERDEFENCE_PLAYERS_H
