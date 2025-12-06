#ifndef TOWERDEFENCE_PROJECTCONTROLLER_H
#define TOWERDEFENCE_PROJECTCONTROLLER_H

#include "BaseController.h"
#include "EnemyController.h"
#include "EnemyEditor.h"
#include "MapController.h"
#include "TowerController.h"

/**
 * Control class that manage entity class Project
 */
class ProjectController : public BaseController, std::enable_shared_from_this<ProjectController> {
public:
	using json = nlohmann::json;

	/**
	 * Constructor for loading existing project from json file
	 *
	 * @param path path to json file
	 */
	ProjectController(const std::string &path);

	/**
	 * Creating new project at the specified path
	 *
	 * @param path path where we will create a new project
	 * @param name project's name
	 */
	ProjectController(const std::string &path, const std::string &name);

	json loadFromFile(const std::string &path);

	bool saveProject();
	std::shared_ptr<TowerController> getTowerController();
	std::shared_ptr<EnemyController> getEnemyController();
	std::shared_ptr<MapController> getMapController();

	json toJson() const { return currentProject->toJson(); }

	void fromJson(const json &j) { currentProject->fromJson(j); }

	std::string getProjectName() const { return currentProject->getName(); }

	void setProjectName(const std::string &name) const { currentProject->setName(name); }

	std::string getProjectPath() const { return currentProject->getPath(); }

	void setProjectPath(const std::string &path) const { currentProject->setPath(path); }

	std::time_t getProjectLastSaveDate() const { return currentProject->getLastSaveDate(); }

	void setProjectLastSaveDate(const std::time_t &date) const { currentProject->setLastSaveDate(date); }

	std::vector<std::shared_ptr<TowerSample>> &getTowers() const { return currentProject->getTowers(); }

	std::vector<std::shared_ptr<EnemySample>> &getEnemies() const { return currentProject->getEnemies(); }

	std::vector<std::shared_ptr<Map>> &getMaps() const { return currentProject->getMaps(); }

	void removeEnemiesFromWaves(std::string enemyName) const;

private:
	std::shared_ptr<Project> currentProject;
	std::shared_ptr<TowerController> towerController;
	std::shared_ptr<EnemyController> enemyController;
	std::shared_ptr<MapController> mapController;
	//     WaveController* waveController;
	//     CampaignController* campaignController;

	/**
	 * Method for initializing all controllers
	 *
	 */
	void loadControls();

	void setEmptyTile();
};


#endif // TOWERDEFENCE_PROJECTCONTROLLER_H
