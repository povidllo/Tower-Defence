#ifndef TOWERDEFENCE_MAPCONTROLLER_H
#define TOWERDEFENCE_MAPCONTROLLER_H
#include "Map.h"

class ProjectController;

class MapController {
public:
	explicit MapController(ProjectController *projectController);

	ProjectController *getProjectController();

	void setCurrentMap(const std::string &name);

	std::shared_ptr<Map> getCurrentMap() const;

	void addMap(const std::string &name, int height, int width);

	bool removeMap(const std::string &name) const;

	std::vector<std::string> getMapNames() const;

	bool mapExists(const std::string &name) const;

	void addWave(const std::string &name);

	bool removeWave(const std::string &name);

	std::vector<std::string> getWavesNames() const;

	bool waveExists(const std::string &name) const;

	std::shared_ptr<WaveSample> getWave(const std::string &name) const;

	bool addPathPoint(const std::string &waveName, int tx, int ty);

	bool deletePathPoint(const std::string &waveName);

	std::vector<std::string> getAvailableEnemies() const;

	std::vector<std::string> getAvailableTowers() const;

	std::shared_ptr<WaveSample> getWave(const std::string &name);

	void addSpot(const std::string &name, int tx, int ty);

	bool spotExist(const std::string &name);

	bool removeSpot(const std::string &name);

	std::shared_ptr<TowerSample> getSpot(const std::string &name);

private:
	ProjectController *projectController;
	std::shared_ptr<Map> currentMap;
};

#endif // TOWERDEFENCE_MAPCONTROLLER_H
