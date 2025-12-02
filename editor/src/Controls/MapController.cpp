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
