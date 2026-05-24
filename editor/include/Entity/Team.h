#ifndef TOWERDEFENCE_TEAM_H
#define TOWERDEFENCE_TEAM_H

#include <string>
#include <vector>

#include "Player.h"
#include "Serializable.h"

class Team : protected ISerializable {
public:
	using json = nlohmann::ordered_json;

	explicit Team(std::string name) : teamName{std::move(name)} {}
	explicit Team(const json &j) : teamName{} { fromJson(j); }

	json toJson() const override;
	void fromJson(const json &j) override;

	const std::string &getTeamName() const { return teamName; }
	void setTeamName(std::string name) { teamName = std::move(name); }

	std::vector<Player> &getPlayers() { return players; }
	const std::vector<Player> &getPlayers() const { return players; }

	void addPlayer(const std::string &playerName) {
		players.emplace_back(playerName, 0.0);
	}

	double getHp() const { return hp; }
	void setHp(const double value) { hp = value; }

	bool removePlayer(const std::string &playerName);

	void clearPlayers() { players.clear(); }

private:
	std::string teamName;
	double hp{0};
	std::vector<Player> players;
};


#endif // TOWERDEFENCE_TEAM_H
