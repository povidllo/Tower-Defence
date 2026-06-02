#ifndef TOWERDEFENCE_MAP_H
#define TOWERDEFENCE_MAP_H
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "Serializable.h"
#include "Team.h"
#include "TowerSample.h"
#include "WaveChain.h"
#include "WaveSample.h"

class Map : protected ISerializable {
public:
	using json = nlohmann::ordered_json;

	Map(std::string name, const int height, const int width);

	explicit Map(const json &j);

	json toJson() const override;

	void fromJson(const json &j) override;

	int getTile(int x, int y) const;

	void setTile(int x, int y, const int tileId);

	std::vector<std::vector<int> > &getTiles();

	[[nodiscard]] std::string getName();

	void setName(const std::string &n);

	int getHeight() const;

	int getWidth() const;

	std::vector<std::shared_ptr<WaveSample> > &getWaves() { return waves; }

	std::vector<std::shared_ptr<WaveChain> > &getStartWaves() { return startWaves; }

	const std::vector<std::shared_ptr<WaveChain> > &getStartWaves() const { return startWaves; }

	void removeWaveReferences(const std::string &waveName);

	void renameWaveReferences(const std::string &oldName, const std::string &newName);

	std::vector<std::shared_ptr<TowerSample> > &getSpots() { return spots; }

	double getHp();

	void setHp(double hp);

	double getStartCurrency();

	void setStartCurrency(double currency);

	std::string getFinalMapImagePath();

	bool isOnlineEnabled() const { return onlineEnabled; }

	void setOnlineEnabled(bool enabled) { onlineEnabled = enabled; }

	int getMaxPlayers() const { return maxPlayers; }

	void setMaxPlayers(int count) { maxPlayers = count; }

	std::vector<std::shared_ptr<Team> > &getTeams() { return teams; }

	const std::vector<std::shared_ptr<Team> > &getTeams() const { return teams; }

	void syncOnlineTeamsWithPlayerCount(int effectiveMaxPlayers);

	[[nodiscard]] int findTeamIndexForPlayer(const std::string &playerId) const;

	void applyPlayerTeamAssignments(const std::vector<int> &playerIndexToTeamIndex);

	static constexpr int getMaxOnlineTeams() { return 5; }

	void hydrateSpotTemplates(const std::vector<std::shared_ptr<TowerSample> > &projectTowers);

private:
	void clampTeamsToMax();

	void migrateLegacyPlayerSpots(const std::vector<std::vector<std::string>> &legacyPlayerSpots);

	std::string name;
	int height{0};
	int width{0};
	double startCurrency{0};
	double hp{0};

	std::string finalMapImagePath;

	std::vector<std::shared_ptr<WaveSample> > waves;
	std::vector<std::shared_ptr<WaveChain> > startWaves;
	std::vector<std::shared_ptr<TowerSample> > spots;
	std::vector<std::vector<int> > tiles;

	bool onlineEnabled{false};
	int maxPlayers{1};
	std::vector<std::shared_ptr<Team> > teams;
};

#endif // TOWERDEFENCE_MAP_H
