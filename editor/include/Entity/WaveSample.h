#ifndef TOWERDEFENCE_WAVESAMPLE_H
#define TOWERDEFENCE_WAVESAMPLE_H

#include <qstring.h>
#include <string>
#include <utility>
#include <vector>
#include "Serializable.h"

using json = nlohmann::json;

class WaveSample : public ISerializable {
public:
	WaveSample(std::string name = "New Wave") : name(std::move(name)) {}

	explicit WaveSample(const json &j) { fromJson(j); }

	json toJson() const override {
		json enemiesArray = json::array();
		for (const auto &[enemyName, count]: enemies) {
			enemiesArray.push_back({{"enemyName", enemyName}, {"count", count}});
		}

		json j = {{"name", name}, {"enemies", enemiesArray}};

		return j;
	}

	void fromJson(const json &j) override {
		name = j.value("name", "Wave");

		enemies.clear();
		if (j.contains("enemies") && j["enemies"].is_array()) {
			for (const auto &item: j["enemies"]) {
				std::string enemyName = item.value("enemyName", "");
				int count = item.value("count", 1);
				if (!enemyName.empty()) {
					enemies.emplace_back(enemyName, count);
				}
			}
		}
	}

	const std::string &getName() const { return name; }
	void setName(const std::string &n) { name = n; }

	const std::vector<std::pair<std::string, int>> &getEnemies() const { return enemies; }
	std::vector<std::pair<std::string, int>> &getEnemies() { return enemies; }

	void addEnemy(const std::string &enemyName, int count = 1) {
		for (auto &pair: enemies) {
			if (pair.first == enemyName) {
				pair.second += count;
				return;
			}
		}
		enemies.emplace_back(enemyName, count);
	}

	void removeEnemyByName(const std::string &enemyName) {
		enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
									 [&enemyName](const auto &p) { return p.first == enemyName; }),
					  enemies.end());
	}

	void clearEnemies() { enemies.clear(); }

	int getTotalEnemyCount() const {
		int total = 0;
		for (const auto &[name, count]: enemies)
			total += count;
		return total;
	}


private:
	std::string name;
	std::vector<std::pair<std::string, int>> enemies;
};

#endif // TOWERDEFENCE_WAVESAMPLE_H
