#include "EnemySample.h"

#include <algorithm>

namespace {
	EnemySample::json stringVectorToJson(const std::vector<std::string> &values) {
		auto result = EnemySample::json::array();
		for (const auto &value: values) {
			result.push_back(value);
		}
		return result;
	}

	void loadStringVector(const EnemySample::json &j, const char *key, std::vector<std::string> &target) {
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

EnemySample::EnemySample(std::string name) : name(std::move(name)) {
}

EnemySample::EnemySample(const json &j) {
	EnemySample::fromJson(j);
}

EnemySample::json EnemySample::toJson() const {
	return {
		{"name", name},
		{"health", health},
		{"damage", damage},
		{"moneyFallsOut", moneyFallsOut},
		{"moneyFallsOutPercentage", moneyFallsOutPercentage},
		{"speed", speed},
		{"enemyTexturePath", enemyTexturePath},
		{"baseEffectCreators", stringVectorToJson(baseEffectCreators)},
		{"damageTakenEffectCreators", stringVectorToJson(damageTakenEffectCreators)},
		{"damageDealtEffectCreators", stringVectorToJson(damageDealtEffectCreators)}
	};
}

void EnemySample::fromJson(const json &j) {
	name = j.value("name", name);
	health = j.value("health", health);
	damage = j.value("damage", damage);
	moneyFallsOut = j.value("moneyFallsOut", moneyFallsOut);
	moneyFallsOutPercentage = j.value("moneyFallsOutPercentage", moneyFallsOutPercentage);
	speed = j.value("speed", speed);

	enemyTexturePath = j.value("enemyTexturePath", "");
	loadStringVector(j, "baseEffectCreators", baseEffectCreators);
	loadStringVector(j, "damageTakenEffectCreators", damageTakenEffectCreators);
	loadStringVector(j, "damageDealtEffectCreators", damageDealtEffectCreators);
}

std::string EnemySample::getName() const {
	return name;
}

void EnemySample::setName(const std::string &name) {
	this->name = name;
}

double EnemySample::getHealth() const {
	return health;
}

void EnemySample::setHealth(double health) {
	this->health = health;
}

double EnemySample::getDamage() const {
	return damage;
}

void EnemySample::setDamage(double damage) {
	this->damage = damage;
}

std::string EnemySample::getEnemyTexturePath() const {
	return enemyTexturePath;
}

void EnemySample::setEnemyTexturePath(const std::string &enemyTexturePath) {
	this->enemyTexturePath = enemyTexturePath;
}

double EnemySample::getSpeed() const {
	return speed;
}

void EnemySample::setSpeed(double speed) {
	this->speed = speed;
}

double EnemySample::getMoneyFallsOut() const {
	return this->moneyFallsOut;
}

void EnemySample::setMoneyFallsOut(double moneyFallsOut) {
	this->moneyFallsOut = moneyFallsOut;
}

double EnemySample::getMoneyFallsOutPercentage() {
	return this->moneyFallsOutPercentage;
}

void EnemySample::setMoneyFallsOutPercentage(double moneyFallsOutPercentage) {
	this->moneyFallsOutPercentage = moneyFallsOutPercentage;
}

std::vector<std::string> EnemySample::getBaseEffectCreatorNames() const {
	return baseEffectCreators;
}

void EnemySample::addBaseEffectCreator(const std::string &name) {
	addUnique(baseEffectCreators, name);
}

bool EnemySample::removeBaseEffectCreator(const std::string &name) {
	return removeValue(baseEffectCreators, name);
}

std::vector<std::string> EnemySample::getDamageTakenEffectCreatorNames() const {
	return damageTakenEffectCreators;
}

void EnemySample::addDamageTakenEffectCreator(const std::string &name) {
	addUnique(damageTakenEffectCreators, name);
}

bool EnemySample::removeDamageTakenEffectCreator(const std::string &name) {
	return removeValue(damageTakenEffectCreators, name);
}

std::vector<std::string> EnemySample::getDamageDealtEffectCreatorNames() const {
	return damageDealtEffectCreators;
}

void EnemySample::addDamageDealtEffectCreator(const std::string &name) {
	addUnique(damageDealtEffectCreators, name);
}

bool EnemySample::removeDamageDealtEffectCreator(const std::string &name) {
	return removeValue(damageDealtEffectCreators, name);
}

void EnemySample::renameEffectCreatorReference(const std::string &oldName, const std::string &newName) {
	renameValue(baseEffectCreators, oldName, newName);
	renameValue(damageTakenEffectCreators, oldName, newName);
	renameValue(damageDealtEffectCreators, oldName, newName);
}

void EnemySample::removeEffectCreatorReference(const std::string &name) {
	removeValue(baseEffectCreators, name);
	removeValue(damageTakenEffectCreators, name);
	removeValue(damageDealtEffectCreators, name);
}
