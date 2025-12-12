#include "TowerController.h"
#include "ProjectController.h"

std::shared_ptr<TowerSample> TowerController::getCurrentTower() { return currentTower; }

TowerController::TowerController(ProjectController *projectController) : projectController(projectController),
																		currentTower(nullptr), currentNextUpgrade("") {
}

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
			projectController->removeTowersFromSpots(name);
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

void TowerController::setTowerTexture(const std::string &path) const {
	if (!TextureUtils::isPngBySignature(path)) {
		throw std::invalid_argument("Tower texture does not have .png format");
	}
	currentTower->setTowerTexturePath(path);
}

void TowerController::addNextUpgrade(const std::string &name) {
	if (towerExists(name)) {
		currentTower->addNextUpgrade(name);
	}
}

bool TowerController::removeNextUpgrade(const std::string &name) {
	return currentTower->removeNextUpgrade(name);
}

std::vector<std::string> TowerController::getNextUpgradeNames() const {
	return currentTower->getUpgradeNames();
}

std::string TowerController::getCurrentNextUpgrade() const {
	return currentNextUpgrade;
}

void TowerController::updateUpgradesNameAfterRename(const std::string &oldName, const std::string &newName) {
	auto &towers = projectController->getTowers();
	for (auto &tower: towers) {
		if (tower->removeNextUpgrade(oldName)) {
			tower->addNextUpgrade(newName);
		}
	}
}

void TowerController::updateUpgradesAfterRemoving(const std::string &name) {
	auto &towers = projectController->getTowers();
	for (auto &tower: towers) {
		tower->removeNextUpgrade(name);
	}
}
