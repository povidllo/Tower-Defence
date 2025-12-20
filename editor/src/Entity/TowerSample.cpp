#include "../../include/Entity/TowerSample.h"

TowerSample::TowerSample(std::string name) : name(std::move(name)) {
}

TowerSample::TowerSample(std::string name, const int x, const int y) : name(std::move(name)), x(x), y(y) {
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
		{"cost", cost},
		{"x", x},
		{"y", y},
		{"projectileSpeed", projectileSpeed},
		{"towerTexturePath", towerTexturePath},
		{"projectileTexturePath", projectileTexturePath},
		{"nextUpgrade", upgradeArray}
	};
}

void TowerSample::fromJson(const json &j) {
	name = j.value("name", name);
	damage = j.value("damage", damage);
	fireRate = j.value("fireRate", fireRate);
	towerTexturePath = j.value("towerTexturePath", towerTexturePath);
	cost = j.value("cost", cost);
	projectileTexturePath = j.value("projectileTexturePath", projectileTexturePath);
	projectileSpeed = j.value("projectileSpeed", projectileSpeed);

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

int TowerSample::getX() const {
	return x;
}

void TowerSample::setProjectileTexturePath(const std::string &path) {
	projectileTexturePath = path;
}

std::string TowerSample::getProjectileTexturePath() const {
	return projectileTexturePath;
}

void TowerSample::setProjectileSpeed(const double speed) {
	projectileSpeed = speed;
}

double TowerSample::getProjectileSpeed() const {
	return projectileSpeed;
}

int TowerSample::getY() const {
	return y;
}

void TowerSample::setX(const int x) {
	this->x = x;
}

void TowerSample::setY(const int y) {
	this->y = y;
}

double TowerSample::getCost() const {
	return cost;
}

void TowerSample::setCost(const double cost) {
	this->cost = cost;
}
