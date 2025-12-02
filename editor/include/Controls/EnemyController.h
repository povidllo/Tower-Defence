#ifndef TOWERDEFENCE_ENEMYCONTROLLER_H
#define TOWERDEFENCE_ENEMYCONTROLLER_H
#include <fstream>
#include <memory>
#include "EnemySample.h"
#include "TextureUtils.h"


class ProjectController;

class EnemyController {
public:
	EnemyController(ProjectController *projectController) :
		projectController(projectController), currentEnemy(nullptr) {}

	void setCurrentEnemy(const std::string &name);

	std::shared_ptr<EnemySample> getCurrentEnemy();

	void addEnemy(std::string name);

	bool removeEnemy(const std::string &name) const;

	std::vector<std::string> getEnemyNames() const;

	bool enemyExists(const std::string &name) const;

	void setEnemyTexture(const std::string &path) {
		if (!TextureUtils::isPngBySignature(path)) {
			throw std::invalid_argument("Enemy texture does not have .png format");
		}
		currentEnemy->setEnemyTexturePath(path);
	}

private:
	ProjectController *projectController;
	std::shared_ptr<EnemySample> currentEnemy;
};


#endif // TOWERDEFENCE_ENEMYCONTROLLER_H
