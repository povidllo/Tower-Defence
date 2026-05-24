#ifndef TOWERDEFENCE_WAVESAMPLE_H
#define TOWERDEFENCE_WAVESAMPLE_H

#include <qstring.h>
#include <string>
#include <utility>
#include <vector>
#include "Serializable.h"

using json = nlohmann::ordered_json;

class WaveSample : public ISerializable {
public:
	WaveSample(std::string name);

	explicit WaveSample(const json &j);

	json toJson() const override;

	void fromJson(const json &j) override;

	const std::string &getName() const;

	void setName(const std::string &n);

	const std::vector<std::pair<std::string, int> > &getEnemies() const;

	std::vector<std::pair<std::string, int> > &getEnemies();

	void addEnemy(const std::string &enemyName, int count = 1);

	void removeEnemyByName(const std::string &enemyName);

	void clearEnemies();

	int getTotalEnemyCount() const;

	std::vector<std::pair<int, int> > &getPath();

	void addPathPoint(int tileX, int tileY);

	void removeLastPathPoint();

	void clearPath();

	double getTimeForWave() const;

	void setTimeForWave(double timeForWave);

	double getEnemySpawnInterval();

	void setEnemySpawnInterval(double spawnInterval);

	const std::vector<std::string> &getBelongs() const;

	std::vector<std::string> &getBelongs();

	void setBelongs(std::vector<std::string> teams);

	void addBelongsTeam(const std::string &teamName);

	void removeBelongsTeam(const std::string &teamName);

	bool belongsToTeam(const std::string &teamName) const;

private:
	std::string name;
	double timeForWave{0};
	double enemySpawnInterval{0};
	std::vector<std::pair<std::string, int> > enemies;
	std::vector<std::pair<int, int> > path;
	std::vector<std::string> belongs;
};

#endif // TOWERDEFENCE_WAVESAMPLE_H
