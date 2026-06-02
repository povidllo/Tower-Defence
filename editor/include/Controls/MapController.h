#ifndef TOWERDEFENCE_MAPCONTROLLER_H
#define TOWERDEFENCE_MAPCONTROLLER_H
#include "Map.h"
#include "WaveChain.h"

class ProjectController;

class MapController {
public:
	explicit MapController(ProjectController *projectController);

	ProjectController *getProjectController() const { return projectController; }

	void setCurrentMap(const std::string &name);

	std::shared_ptr<Map> getCurrentMap() const;

	void addMap(const std::string &name, int height, int width);

	bool removeMap(const std::string &name) const;

	std::vector<std::string> getMapNames() const;

	bool mapExists(const std::string &name) const;

	void addWave(const std::string &name);

	bool removeWave(const std::string &name);

	void renameWave(const std::string &oldName, const std::string &newName);

	std::vector<std::string> getTeamNames() const;

	void addStartWaveChain();

	bool removeStartWaveChain(size_t index);

	std::shared_ptr<WaveChain> getStartWaveChain(size_t index);

	size_t getStartWaveChainCount() const;

	std::vector<std::string> getWavesNames() const;

	bool waveExists(const std::string &name) const;

	std::shared_ptr<WaveSample> getWave(const std::string &name) const;

	bool addPathPoint(const std::string &waveName, int tx, int ty);

	bool deletePathPoint(const std::string &waveName);

	std::vector<std::string> getAvailableEnemies() const;

	std::vector<std::string> getAvailableTowers() const;

	std::shared_ptr<WaveSample> getWave(const std::string &name);

	/** Map spot instance id + project tower template name (see towers[] in project JSON). */
	bool addSpot(const std::string &instanceName, const std::string &towerTemplateName, int tx, int ty);

	bool spotExist(const std::string &name);

	bool removeSpot(const std::string &name);

	std::shared_ptr<TowerSample> getSpot(const std::string &name);

	[[nodiscard]] std::vector<std::string> getPlayerSlotIds() const;

private:
	ProjectController *projectController;
	std::shared_ptr<Map> currentMap;
};

#endif // TOWERDEFENCE_MAPCONTROLLER_H
