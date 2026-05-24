#include "MapController.h"
#include "ProjectController.h"

namespace {
std::shared_ptr<TowerSample> findProjectTower(ProjectController *projectController, const std::string &towerName) {
	for (const auto &t : projectController->getTowers()) {
		if (t->getName() == towerName) {
			return t;
		}
	}
	return nullptr;
}
} // namespace

MapController::MapController(ProjectController *projectController) : projectController(projectController),
																	currentMap(nullptr) {
}

void MapController::setCurrentMap(const std::string &name) {
	const auto &maps = projectController->getMaps();
	for (auto &map: maps) {
		if (map->getName() == name) {
			currentMap = map;
			return;
		}
	}
}

std::shared_ptr<Map> MapController::getCurrentMap() const { return currentMap; }

void MapController::addMap(const std::string &name, int height, int width) {
	const auto newMap = std::make_shared<Map>(name, height, width);
	currentMap = newMap;
	auto &maps = projectController->getMaps();
	maps.push_back(newMap);
}

bool MapController::removeMap(const std::string &name) const {
	auto &maps = projectController->getMaps();
	for (int i = 0; i < maps.size(); i++) {
		if (maps[i]->getName() == name) {
			maps.erase(maps.begin() + i);
			return true;
		}
	}
	return false;
}

std::vector<std::string> MapController::getMapNames() const {
	const auto &maps = projectController->getMaps();
	std::vector<std::string> names;
	for (auto &map: maps) {
		names.push_back(map->getName());
	}
	return names;
}

bool MapController::mapExists(const std::string &name) const {
	const auto &maps = projectController->getMaps();
	for (const auto &map: maps) {
		if (map->getName() == name) {
			return true;
		}
	}
	return false;
}

void MapController::addWave(const std::string &name) {
	auto &waves = currentMap->getWaves();
	auto newWave = std::make_shared<WaveSample>(name);
	waves.push_back(newWave);
}

bool MapController::removeWave(const std::string &name) {
	if (!currentMap) {
		return false;
	}
	auto &waves = currentMap->getWaves();
	const auto before = waves.size();
	waves.erase(std::remove_if(waves.begin(), waves.end(),
								[&name](const std::shared_ptr<WaveSample> &wave) { return wave->getName() == name; }),
				waves.end());
	if (waves.size() == before) {
		return false;
	}
	currentMap->removeWaveReferences(name);
	return true;
}

void MapController::renameWave(const std::string &oldName, const std::string &newName) {
	if (!currentMap || oldName == newName) {
		return;
	}
	if (auto wave = getWave(oldName)) {
		wave->setName(newName);
		currentMap->renameWaveReferences(oldName, newName);
	}
}

std::vector<std::string> MapController::getTeamNames() const {
	std::vector<std::string> names;
	if (!currentMap) {
		return names;
	}
	for (const auto &team : currentMap->getTeams()) {
		names.push_back(team->getTeamName());
	}
	return names;
}

void MapController::addStartWaveChain() {
	if (!currentMap) {
		return;
	}
	currentMap->getStartWaves().push_back(std::make_shared<WaveChain>());
}

bool MapController::removeStartWaveChain(const size_t index) {
	if (!currentMap || index >= currentMap->getStartWaves().size()) {
		return false;
	}
	auto &chains = currentMap->getStartWaves();
	chains.erase(chains.begin() + static_cast<std::ptrdiff_t>(index));
	return true;
}

std::shared_ptr<WaveChain> MapController::getStartWaveChain(const size_t index) {
	if (!currentMap || index >= currentMap->getStartWaves().size()) {
		return nullptr;
	}
	return currentMap->getStartWaves()[index];
}

size_t MapController::getStartWaveChainCount() const {
	if (!currentMap) {
		return 0;
	}
	return currentMap->getStartWaves().size();
}

std::vector<std::string> MapController::getWavesNames() const {
	if (!currentMap) {
		return std::vector<std::string>();
	}
	const auto &waves = currentMap->getWaves();
	std::vector<std::string> names;
	for (auto &wave: waves) {
		names.push_back(wave->getName());
	}
	return names;
}

bool MapController::waveExists(const std::string &name) const {
	const auto &waves = currentMap->getWaves();
	for (const auto &wave: waves) {
		if (wave->getName() == name) {
			return true;
		}
	}
	return false;
}

std::shared_ptr<WaveSample> MapController::getWave(const std::string &name) const {
	auto &waves = currentMap->getWaves();
	for (const auto &wave: waves) {
		if (wave->getName() == name) {
			return wave;
		}
	}
	return nullptr;
}

bool MapController::addPathPoint(const std::string &waveName, int tx, int ty) {
	if (tx >= currentMap->getWidth() || ty >= currentMap->getHeight() || tx < 0 || ty < 0) {
		return false;
	}
	auto it = getWave(waveName);
	if (it != nullptr) {
		it->addPathPoint(tx, ty);
		return true;
	}
	return false;
}

bool MapController::deletePathPoint(const std::string &waveName) {
	auto it = getWave(waveName);
	if (it != nullptr && !it->getPath().empty()) {
		it->removeLastPathPoint();
		return true;
	}
	return false;
}

std::vector<std::string> MapController::getAvailableEnemies() const {
	auto &enemies = projectController->getEnemies();
	std::vector<std::string> names;
	for (auto &enemy: enemies) {
		names.push_back(enemy->getName());
	}
	return names;
}

std::vector<std::string> MapController::getAvailableTowers() const {
	auto &towers = projectController->getTowers();
	std::vector<std::string> names;
	for (auto &tower: towers) {
		names.push_back(tower->getName());
	}
	return names;
}

std::shared_ptr<WaveSample> MapController::getWave(const std::string &name) {
	auto waves = currentMap->getWaves();
	for (auto &wave: waves) {
		if (wave->getName() == name) {
			return wave;
		}
	}
	return nullptr;
}

bool MapController::addSpot(const std::string &instanceName, const std::string &towerTemplateName, int tx, int ty) {
	const auto tmpl = findProjectTower(projectController, towerTemplateName);
	if (!tmpl) {
		return false;
	}
	auto spot = std::make_shared<TowerSample>(instanceName, tx, ty);
	spot->setTowerTemplateName(towerTemplateName);
	spot->applyTemplate(*tmpl);
	auto &spots = currentMap->getSpots();
	spots.push_back(spot);
	return true;
}

bool MapController::spotExist(const std::string &name) {
	if (!currentMap) {
		return false;
	}
	const auto &spots = currentMap->getSpots();
	for (const auto &spot: spots) {
		if (spot->getName() == name) {
			return true;
		}
	}
	return false;
}

bool MapController::removeSpot(const std::string &name) {
	if (!currentMap || !spotExist(name)) {
		return false;
	}
	auto &spots = currentMap->getSpots();
	for (int i = 0; i < spots.size(); i++) {
		if (spots[i]->getName() == name) {
			spots.erase(spots.begin() + i);
			return true;
		}
	}
	return false;
}

std::shared_ptr<TowerSample> MapController::getSpot(const std::string &name) {
	if (!currentMap) {
		return nullptr;
	}
	auto &spots = currentMap->getSpots();
	for (const auto &spot: spots) {
		if (spot->getName() == name) {
			return spot;
		}
	}
	return nullptr;
}
