#ifndef TOWERDEFENCE_TOWERCONTROLLER_H
#define TOWERDEFENCE_TOWERCONTROLLER_H

#include <qtmetamacros.h>

#include <fstream>

#include "TextureUtils.h"
#include "TowerSample.h"

class ProjectController;

class TowerController {
public:
	explicit TowerController(ProjectController *projectController);

	void setCurrentTower(const std::string &name);

	std::shared_ptr<TowerSample> getCurrentTower();

	void addTower(std::string name);

	bool removeTower(const std::string &name) const;

	std::vector<std::string> getTowerNames() const;

	bool towerExists(const std::string &name) const;

	void setTowerTexture(const std::string &path) const;

	void addNextUpgrade(const std::string &name);

	bool removeNextUpgrade(const std::string &name);

	std::vector<std::string> getNextUpgradeNames() const;

	std::string getCurrentNextUpgrade() const;

	void updateUpgradesNameAfterRename(const std::string &oldName, const std::string &newName);

	void updateUpgradesAfterRemoving(const std::string &name);

private:
	ProjectController *projectController;
	std::shared_ptr<TowerSample> currentTower;
	std::string currentNextUpgrade;
};

#endif // TOWERDEFENCE_TOWERCONTROLLER_H
