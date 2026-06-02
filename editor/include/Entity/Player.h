#ifndef TOWERDEFENCE_PLAYERS_H
#define TOWERDEFENCE_PLAYERS_H

#include <string>
#include <vector>

#include "Serializable.h"

class Player : protected ISerializable {
public:
	using json = nlohmann::ordered_json;

	Player(std::string playerName, const double startCurrency) :
		playerName{std::move(playerName)}, startCurrency{startCurrency} {}
	Player(const json &j);

	json toJson() const override;
	void fromJson(const json &j) override;

	const std::string &getPlayerName() const { return playerName; }
	void setPlayerName(std::string name) { playerName = std::move(name); }

	double getStartCurrency() const { return startCurrency; }
	void setStartCurrency(const double start_currency) { startCurrency = start_currency; }

	const std::vector<std::string> &getAbilityNames() const { return abilityNames; }
	void setAbilityNames(std::vector<std::string> names) { abilityNames = std::move(names); }
	void addAbility(const std::string &name);
	bool removeAbility(const std::string &name);
	void renameAbilityReference(const std::string &oldName, const std::string &newName);
	void removeAbilityReference(const std::string &name);

private:
	std::string playerName;
	double startCurrency{0};
	std::vector<std::string> abilityNames;

};


#endif // TOWERDEFENCE_PLAYERS_H
