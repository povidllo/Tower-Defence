#include "../../include/Entity/WaveSample.h"

WaveSample::WaveSample(std::string name) : name(std::move(name)) {
}

WaveSample::WaveSample(const json &j) { fromJson(j); }

json WaveSample::toJson() const {
	json enemiesArray = json::array();
	for (const auto &[enemyName, count]: enemies) {
		enemiesArray.push_back({{"enemyName", enemyName}, {"count", count}});
	}

	json pathArray = json::array();
	for (const auto &[x, y]: path) {
		pathArray.push_back({{"x", x}, {"y", y}});
	}

	json j = {
		{"name", name}, {"enemies", enemiesArray}, {"path", pathArray}, {"timeForWave", timeForWave},
		{"enemySpawnInterval", enemySpawnInterval}
	};

	return j;
}

void WaveSample::fromJson(const json &j) {
	name = j.value("name", "Wave");
	timeForWave = j.value("timeForWave", 0);
	enemySpawnInterval = j.value("enemySpawnInterval", 0);


	if (j.contains("enemies") && j["enemies"].is_array()) {
		enemies.clear();
		for (const auto &item: j["enemies"]) {
			std::string enemyName = item.value("enemyName", "");
			int count = item.value("count", 1);
			if (!enemyName.empty()) {
				enemies.emplace_back(enemyName, count);
			}
		}
	}

	if (j.contains("path") && j["path"].is_array()) {
		for (const auto &item: j["path"]) {
			int x = item.value("x", 0);
			int y = item.value("y", 0);
			path.emplace_back(x, y);
		}
	}
}

const std::string &WaveSample::getName() const { return name; }

void WaveSample::setName(const std::string &n) { name = n; }

const std::vector<std::pair<std::string, int> > &WaveSample::getEnemies() const { return enemies; }

std::vector<std::pair<std::string, int> > &WaveSample::getEnemies() { return enemies; }

void WaveSample::addEnemy(const std::string &enemyName, int count) { enemies.emplace_back(enemyName, count); }

void WaveSample::removeEnemyByName(const std::string &enemyName) {
	enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
								[&enemyName](const auto &p) { return p.first == enemyName; }),
				enemies.end());
}

void WaveSample::clearEnemies() { enemies.clear(); }

int WaveSample::getTotalEnemyCount() const {
	int total = 0;
	for (const auto &[name, count]: enemies)
		total += count;
	return total;
}

std::vector<std::pair<int, int> > &WaveSample::getPath() { return path; }

void WaveSample::addPathPoint(int tileX, int tileY) { path.emplace_back(tileX, tileY); }

void WaveSample::removeLastPathPoint() {
	if (!path.empty()) {
		path.pop_back();
	}
}

void WaveSample::clearPath() { path.clear(); }

double WaveSample::getTimeForWave() const {
	return timeForWave;
}

void WaveSample::setTimeForWave(double timeForWave) {
	this->timeForWave = timeForWave;
}

double WaveSample::getEnemySpawnInterval() {
	return enemySpawnInterval;
}

void WaveSample::setEnemySpawnInterval(double spawnInterval) {
	this->enemySpawnInterval = spawnInterval;
}
