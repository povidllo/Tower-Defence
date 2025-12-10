#include "../../include/Entity/TowerSample.h"

TowerSample::TowerSample(std::string name) : name(std::move(name)) {
}

TowerSample::TowerSample(const json &j) {
	TowerSample::fromJson(j);
}

TowerSample::json TowerSample::toJson() const {
	json upgradeArray = json::array();
	for (const auto &name: nextUpgrade) {
		upgradeArray.push_back(name);
	}
	return {
		{"name", name},
		{"damage", damage},
		{"fireRate", fireRate},
		{"towerTexturePath", towerTexturePath},
		{"nextUpgrade", upgradeArray},
		{"x", x},
		{"y", y}
	};
}

void TowerSample::fromJson(const json &j) {
	name = j.value("name", name);
	damage = j.value("damage", damage);
	fireRate = j.value("fireRate", fireRate);
	towerTexturePath = j.value("towerTexturePath", "");

	if (j.contains("nextUpgrade") && j["nextUpgrade"].is_array()) {
		nextUpgrade.clear();
		for (const auto &item: j["nextUpgrade"]) {
			if (item.is_string()) {
				nextUpgrade.push_back(item.get<std::string>());
			}
		}
	}

	x = j.value("x", x);
	y = j.value("y", y);
}

std::string TowerSample::getName() {
	return name;
}

void TowerSample::setName(const std::string &n) {
	name = n;
}

double TowerSample::getDamage() const {
	return damage;
}

void TowerSample::setDamage(double d) {
	damage = d;
}

double TowerSample::getFireRate() const {
	return fireRate;
}

void TowerSample::setFireRate(double r) {
	fireRate = r;
}

void TowerSample::setTowerTexturePath(const std::string &texPath) {
	towerTexturePath = texPath;
}

std::string TowerSample::getTowerTexturePath() const {
	return towerTexturePath;
}

std::vector<std::string> TowerSample::getUpgradeNames() const {
	return nextUpgrade;
}

void TowerSample::addNextUpgrade(const std::string &name) {
	nextUpgrade.push_back(name);
}

bool TowerSample::removeNextUpgrade(const std::string &name) {
	auto it = std::remove(nextUpgrade.begin(), nextUpgrade.end(), name);
	if (it != nextUpgrade.end()) {
		nextUpgrade.erase(it, nextUpgrade.end());
		return true;
	}
	return false;
}
