#ifndef TOWERDEFENCE_PROJECTCONTROLLER_H
#define TOWERDEFENCE_PROJECTCONTROLLER_H

#include "AbilityController.h"
#include "BaseController.h"
#include "EffectCreatorController.h"
#include "EffectController.h"
#include "EnemyController.h"
#include "EnemyEditor.h"
#include "MapController.h"
#include "TowerController.h"

/**
 * Control class that manage entity class Project
 */
class ProjectController : public BaseController, std::enable_shared_from_this<ProjectController> {
public:
	using json = nlohmann::ordered_json;

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

	std::shared_ptr<EffectController> getEffectController();

	std::shared_ptr<EffectCreatorController> getEffectCreatorController();

	std::shared_ptr<AbilityController> getAbilityController();

	std::shared_ptr<MapController> getMapController();

	json toJson() const;

	void fromJson(const json &j);

	std::string getProjectName() const;

	void setProjectName(const std::string &name) const;

	std::string getProjectPath() const;

	void setProjectPath(const std::string &path) const;

	std::time_t getProjectLastSaveDate() const;

	void setProjectLastSaveDate(const std::time_t &date) const;

	std::vector<std::shared_ptr<TowerSample> > &getTowers() const;

	std::vector<std::shared_ptr<EnemySample> > &getEnemies() const;

	std::vector<std::shared_ptr<EffectSample> > &getEffects() const;

	std::vector<std::shared_ptr<EffectCreatorSample> > &getEffectCreators() const;

	std::vector<std::shared_ptr<AbilitySample> > &getAbilities() const;

	std::vector<std::shared_ptr<Map> > &getMaps() const { return currentProject->getMaps(); }

	void removeEnemiesFromWaves(std::string enemyName) const;

	void removeTowersFromSpots(const std::string &towerName);

	void renameTowerTemplateOnMapSpots(const std::string &oldName, const std::string &newName) const;

	void renameEffectCreatorReferences(const std::string &oldName, const std::string &newName) const;

	void removeEffectCreatorReferences(const std::string &name) const;

	void renameAbilityReferences(const std::string &oldName, const std::string &newName) const;

	void removeAbilityReferences(const std::string &name) const;

	void rehydrateAllMapSpots() const;

	std::shared_ptr<Project> getCurrentProject() const;

private:
	std::shared_ptr<Project> currentProject;
	std::shared_ptr<TowerController> towerController;
	std::shared_ptr<EnemyController> enemyController;
	std::shared_ptr<EffectController> effectController;
	std::shared_ptr<EffectCreatorController> effectCreatorController;
	std::shared_ptr<AbilityController> abilityController;
	std::shared_ptr<MapController> mapController;

	void loadControls();

	void setEmptyTile();
};

#endif // TOWERDEFENCE_PROJECTCONTROLLER_H
