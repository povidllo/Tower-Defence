#include "MapController.h"
#include "ProjectController.h"

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
	auto waves = currentMap->getWaves();
	waves.erase(std::remove_if(waves.begin(), waves.end(),
							   [&name](const std::shared_ptr<WaveSample> &wave) { return wave->getName() == name; }),
				waves.end());
	return true;
}

std::vector<std::string> MapController::getWavesNames() const {
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

std::shared_ptr<WaveSample> MapController::getWave(const std::string &name) {
	auto waves = currentMap->getWaves();
	for (auto &wave: waves) {
		if (wave->getName() == name) {
			return wave;
		}
	}
	return nullptr;
}
