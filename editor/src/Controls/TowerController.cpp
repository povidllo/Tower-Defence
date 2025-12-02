#include "TowerController.h"
#include "ProjectController.h"

std::shared_ptr<TowerSample> TowerController::getCurrentTower() { return currentTower; }

void TowerController::setCurrentTower(const std::string &name) {
	const auto &towers = projectController->getTowers();
	for (auto &tower: towers) {
		if (tower->getName() == name) {
			currentTower = tower;
			return;
		}
	}
}

void TowerController::addTower(std::string name) {
	const auto newTower = std::make_shared<TowerSample>(name);
	currentTower = newTower;
	auto &towers = projectController->getTowers();
	towers.push_back(newTower);
}

bool TowerController::removeTower(const std::string &name) const {
	auto &towers = projectController->getTowers();
	for (int i = 0; i < towers.size(); i++) {
		if (towers[i]->getName() == name) {
			towers.erase(towers.begin() + i);
			return true;
		}
	}
	return false;
}

std::vector<std::string> TowerController::getTowerNames() const {
	const auto &towers = projectController->getTowers();
	std::vector<std::string> names;
	for (auto &tower: towers) {
		names.push_back(tower->getName());
	}
	return names;
}

bool TowerController::towerExists(const std::string &name) const {
	const auto &towers = projectController->getTowers();
	for (const auto &tower: towers) {
		if (tower->getName() == name) {
			return true;
		}
	}
	return false;
}
