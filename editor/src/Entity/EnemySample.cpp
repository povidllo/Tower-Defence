#include "EnemySample.h"

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
		{"enemyTexturePath", enemyTexturePath}
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
