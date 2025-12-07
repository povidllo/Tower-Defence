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
		{"enemyTexturePath", enemyTexturePath},
	};
}

void EnemySample::fromJson(const json &j) {
	name = j.value("name", name);
	health = j.value("health", health);
	damage = j.value("damage", damage);
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
