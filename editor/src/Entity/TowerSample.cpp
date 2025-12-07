#include "../../include/Entity/TowerSample.h"

TowerSample::TowerSample(std::string name) : name(std::move(name)) {
}

TowerSample::TowerSample(const json &j) {
	TowerSample::fromJson(j);
}

TowerSample::json TowerSample::toJson() const {
	return {
		{"name", name},
		{"damage", damage},
		{"fireRate", fireRate},
		{"towerTexturePath", towerTexturePath},
		// {"projectileTexturePath", projectileTexturePath},
		{"x", x},
		{"y", y}
	};
}

void TowerSample::fromJson(const json &j) {
	name = j.value("name", name);
	damage = j.value("damage", damage);
	fireRate = j.value("fireRate", fireRate);
	towerTexturePath = j.value("towerTexturePath", "");
	// projectileTexturePath = j.value("projectileTexturePath", "");
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
