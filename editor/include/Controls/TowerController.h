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

private:
	ProjectController *projectController;
	std::shared_ptr<TowerSample> currentTower;
};

#endif // TOWERDEFENCE_TOWERCONTROLLER_H
