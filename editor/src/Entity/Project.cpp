#include "Project.h"

#include "TextureManager.h"

Project::Project(const std::string &name, const std::string &path, const std::time_t &lastSaveDate) {
	this->name = name;
	this->lastSaveDate = lastSaveDate;
	this->path = path;
}

Project::Project(const json &j) { Project::fromJson(j); }

// Project::json Project::toJson() const { return {{"name", name}, {"lastSaveDate", lastSaveDate}, {"path", path}}; }
//
// void Project::fromJson(const json &j) {
// 	name = j.at("name").get<std::string>();
// 	lastSaveDate = j.at("lastSaveDate").get<std::time_t>();
// }

Project::json Project::toJson() const {
	json j;

	j["name"] = name;
	j["path"] = path;
	j["lastSaveDate"] = lastSaveDate;

	json mapsArray = json::array();
	for (const auto &map: maps) {
		mapsArray.push_back(map->toJson());
	}
	j["maps"] = mapsArray;

	json towersArray = json::array();
	for (const auto &tower: towers) {
		towersArray.push_back(tower->toJson());
	}
	j["towers"] = towersArray;

	json enemiesArray = json::array();
	for (const auto &enemy: enemies) {
		enemiesArray.push_back(enemy->toJson());
	}
	j["enemies"] = enemiesArray;

	j["textures"] = TextureManager::instance().toJson();

	return j;
}

void Project::fromJson(const json &j) {
	name = j.value("name", "Untitled Project");
	path = j.value("path", "");
	lastSaveDate = j.value("lastSaveDate", std::time(nullptr));

	maps.clear();
	towers.clear();
	enemies.clear();

	if (j.contains("maps")) {
		for (const auto &mapJson: j["maps"]) {
			auto map = std::make_shared<Map>(mapJson);
			maps.push_back(map);
		}
	}

	if (j.contains("towers")) {
		for (const auto &towerJson: j["towers"]) {
			auto tower = std::make_shared<TowerSample>(towerJson);
			towers.push_back(tower);
		}
	}

	if (j.contains("enemies")) {
		for (const auto &enemyJson: j["enemies"]) {
			auto enemy = std::make_shared<EnemySample>(enemyJson);
			enemies.push_back(enemy);
		}
	}

	if (j.contains("textures")) {
		TextureManager::instance().fromJson(j["textures"]);
	}
}

std::string Project::getName() const { return name; }

void Project::setName(const std::string &name) { this->name = name; }

std::string Project::getPath() const { return path; }

void Project::setPath(const std::string &path) { this->path = path; }

std::time_t Project::getLastSaveDate() const { return lastSaveDate; }

void Project::setLastSaveDate(std::time_t last_save_date) { lastSaveDate = last_save_date; }

std::vector<std::shared_ptr<TowerSample>> &Project::getTowers() { return towers; }

std::vector<std::shared_ptr<EnemySample>> &Project::getEnemies() { return enemies; }

std::vector<std::shared_ptr<Map>> &Project::getMaps() { return maps; }
