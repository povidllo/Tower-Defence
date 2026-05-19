#include "../../include/Entity/TowerSample.h"

#include <algorithm>

namespace {
	TowerSample::json stringVectorToJson(const std::vector<std::string> &values) {
		auto result = TowerSample::json::array();
		for (const auto &value: values) {
			result.push_back(value);
		}
		return result;
	}

	void loadStringVector(const TowerSample::json &j, const char *key, std::vector<std::string> &target) {
		if (!j.contains(key) || !j[key].is_array()) {
			return;
		}
		target.clear();
		for (const auto &item: j[key]) {
			if (item.is_string()) {
				target.push_back(item.get<std::string>());
			}
		}
	}

	void addUnique(std::vector<std::string> &values, const std::string &name) {
		if (std::find(values.begin(), values.end(), name) == values.end()) {
			values.push_back(name);
		}
	}

	bool removeValue(std::vector<std::string> &values, const std::string &name) {
		auto it = std::remove(values.begin(), values.end(), name);
		if (it == values.end()) {
			return false;
		}
		values.erase(it, values.end());
		return true;
	}

	void renameValue(std::vector<std::string> &values, const std::string &oldName, const std::string &newName) {
		for (auto &value: values) {
			if (value == oldName) {
				value = newName;
			}
		}
	}
}

TowerSample::TowerSample(std::string name) : name(std::move(name)) {
}

TowerSample::TowerSample(std::string name, const int x, const int y) : name(std::move(name)), x(x), y(y) {}

TowerSample::TowerSample(const json &j) { TowerSample::fromJson(j); }

TowerSample::json TowerSample::toJson() const {
	if (!towerTemplateName.empty()) {
		return {
			{"name", name},
			{"tower", towerTemplateName},
			{"x", x},
			{"y", y},
		};
	}
	return {{"name", name},
			{"damage", damage},
			{"fireRate", fireRate},
			{"cost", cost},
			{"x", x},
			{"y", y},
			{"projectileSpeed", projectileSpeed},
			{"fireDistance", fireDistance},
			{"towerTexturePath", towerTexturePath},
			{"projectileTexturePath", projectileTexturePath},
			{"nextUpgrade", stringVectorToJson(nextUpgrade)},
			{"baseEffectCreators", stringVectorToJson(baseEffectCreators)},
			{"attackEffectCreators", stringVectorToJson(attackEffectCreators)}};
}

void TowerSample::fromJson(const json &j) {
	const bool refMode = j.contains("tower") && j["tower"].is_string() &&
						 !(j.contains("damage") && j["damage"].is_number());
	if (refMode) {
		name = j.value("name", name);
		towerTemplateName = j["tower"].get<std::string>();
		x = j.value("x", x);
		y = j.value("y", y);
		return;
	}

	towerTemplateName.clear();
	name = j.value("name", name);
	damage = j.value("damage", damage);
	fireRate = j.value("fireRate", fireRate);
	towerTexturePath = j.value("towerTexturePath", towerTexturePath);
	cost = j.value("cost", cost);
	projectileTexturePath = j.value("projectileTexturePath", projectileTexturePath);
	projectileSpeed = j.value("projectileSpeed", projectileSpeed);
	fireDistance = j.value("fireDistance", fireDistance);

	loadStringVector(j, "nextUpgrade", nextUpgrade);
	loadStringVector(j, "baseEffectCreators", baseEffectCreators);
	loadStringVector(j, "attackEffectCreators", attackEffectCreators);

	x = j.value("x", x);
	y = j.value("y", y);
}

std::string TowerSample::getName() { return name; }

void TowerSample::setName(const std::string &n) { name = n; }

double TowerSample::getDamage() const { return damage; }

void TowerSample::setDamage(double d) { damage = d; }

double TowerSample::getFireRate() const { return fireRate; }

void TowerSample::setFireRate(double r) { fireRate = r; }

void TowerSample::setTowerTexturePath(const std::string &texPath) { towerTexturePath = texPath; }

std::string TowerSample::getTowerTexturePath() const { return towerTexturePath; }

std::vector<std::string> TowerSample::getUpgradeNames() const { return nextUpgrade; }

std::vector<std::string> TowerSample::getBaseEffectCreatorNames() const { return baseEffectCreators; }

void TowerSample::addBaseEffectCreator(const std::string &name) { addUnique(baseEffectCreators, name); }

bool TowerSample::removeBaseEffectCreator(const std::string &name) {
	return removeValue(baseEffectCreators, name);
}

std::vector<std::string> TowerSample::getAttackEffectCreatorNames() const { return attackEffectCreators; }

void TowerSample::addAttackEffectCreator(const std::string &name) { addUnique(attackEffectCreators, name); }

bool TowerSample::removeAttackEffectCreator(const std::string &name) {
	return removeValue(attackEffectCreators, name);
}

void TowerSample::renameEffectCreatorReference(const std::string &oldName, const std::string &newName) {
	renameValue(baseEffectCreators, oldName, newName);
	renameValue(attackEffectCreators, oldName, newName);
}

void TowerSample::removeEffectCreatorReference(const std::string &name) {
	removeValue(baseEffectCreators, name);
	removeValue(attackEffectCreators, name);
}

void TowerSample::addNextUpgrade(const std::string &name) { nextUpgrade.push_back(name); }

bool TowerSample::removeNextUpgrade(const std::string &name) {
	auto it = std::remove(nextUpgrade.begin(), nextUpgrade.end(), name);
	if (it != nextUpgrade.end()) {
		nextUpgrade.erase(it, nextUpgrade.end());
		return true;
	}
	return false;
}

int TowerSample::getX() const { return x; }

void TowerSample::setProjectileTexturePath(const std::string &path) { projectileTexturePath = path; }

std::string TowerSample::getProjectileTexturePath() const { return projectileTexturePath; }

void TowerSample::setProjectileSpeed(const double speed) { projectileSpeed = speed; }

double TowerSample::getProjectileSpeed() const { return projectileSpeed; }

int TowerSample::getY() const { return y; }

void TowerSample::setX(const int x) { this->x = x; }

void TowerSample::setY(const int y) { this->y = y; }

double TowerSample::getCost() const { return cost; }

void TowerSample::setCost(const double cost) { this->cost = cost; }
double TowerSample::getFireDistance() const { return fireDistance; }
void TowerSample::setFireDistance(const double fireDistance) { this->fireDistance = fireDistance; }

void TowerSample::applyTemplate(const TowerSample &src) {
	damage = src.getDamage();
	fireRate = src.getFireRate();
	cost = src.getCost();
	projectileSpeed = src.getProjectileSpeed();
	fireDistance = src.getFireDistance();
	towerTexturePath = src.getTowerTexturePath();
	projectileTexturePath = src.getProjectileTexturePath();
	nextUpgrade = src.getUpgradeNames();
	baseEffectCreators = src.getBaseEffectCreatorNames();
	attackEffectCreators = src.getAttackEffectCreatorNames();
}
