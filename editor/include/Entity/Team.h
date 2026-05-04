#ifndef TOWERDEFENCE_TEAM_H
#define TOWERDEFENCE_TEAM_H

#include <string>
#include <vector>

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

	const std::vector<std::string> &getPlayers() const { return players; }

	void addPlayer(const std::string &playerName) { players.push_back(playerName); }

	bool removePlayer(const std::string &playerName);

	void clearPlayers() { players.clear(); }

private:
	std::string teamName;
	std::vector<std::string> players;
};


#endif // TOWERDEFENCE_TEAM_H
