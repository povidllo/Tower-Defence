#ifndef TOWERDEFENCE_ENEMYCONTROLLER_H
#define TOWERDEFENCE_ENEMYCONTROLLER_H
#include <fstream>
#include <memory>
#include "EnemySample.h"
#include "TextureUtils.h"

class ProjectController;

class EnemyController {
public:
	EnemyController(ProjectController *projectController);

	void setCurrentEnemy(const std::string &name);

	std::shared_ptr<EnemySample> getCurrentEnemy();

	void addEnemy(std::string name);

	bool removeEnemy(const std::string &name) const;

	std::vector<std::string> getEnemyNames() const;

	bool enemyExists(const std::string &name) const;

	void setEnemyTexture(const std::string &path) const;

private:
	ProjectController *projectController;

	std::shared_ptr<EnemySample> currentEnemy;
};

#endif // TOWERDEFENCE_ENEMYCONTROLLER_H
