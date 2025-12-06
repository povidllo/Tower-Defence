#include "EnemyController.h"

#include "ProjectController.h"

void EnemyController::setCurrentEnemy(const std::string &name) {
	const auto &enemies = projectController->getEnemies();
	for (auto &enemy: enemies) {
		if (enemy->getName() == name) {
			currentEnemy = enemy;
			return;
		}
	}
}

std::shared_ptr<EnemySample> EnemyController::getCurrentEnemy() { return currentEnemy; }

void EnemyController::addEnemy(std::string name) {
	const auto newEnemy = std::make_shared<EnemySample>(name);
	currentEnemy = newEnemy;
	auto &enemies = projectController->getEnemies();
	enemies.push_back(newEnemy);
}

bool EnemyController::removeEnemy(const std::string &name) const {
	auto &enemies = projectController->getEnemies();
	for (int i = 0; i < enemies.size(); i++) {
		if (enemies[i]->getName() == name) {
			enemies.erase(enemies.begin() + i);
			projectController->removeEnemiesFromWaves(name);
			return true;
		}
	}

	return false;
}

std::vector<std::string> EnemyController::getEnemyNames() const {
	const auto enemies = projectController->getEnemies();
	std::vector<std::string> names;
	for (auto &enemy: enemies) {
		names.push_back(enemy->getName());
	}
	return names;
}

bool EnemyController::enemyExists(const std::string &name) const {
	const auto enemies = projectController->getEnemies();
	for (const auto &enemy: enemies) {
		if (enemy->getName() == name) {
			return true;
		}
	}
	return false;
}
