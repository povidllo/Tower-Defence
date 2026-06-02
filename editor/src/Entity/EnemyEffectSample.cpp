#include "EnemyEffectSample.h"

#include <algorithm>

EnemyEffectSample::EnemyEffectSample(std::string name)
	: EffectSample(std::move(name), Kind::Enemy) {}

EnemyEffectSample::EnemyEffectSample(const json &j) : EffectSample("", Kind::Enemy) {
	fromJson(j);
}

EnemyEffectSample::json EnemyEffectSample::toJson() const {
	json j = writeCommonFields();
	j["startHealthImpact"] = startHealthImpact;
	j["startSpeedImpactPercent"] = startSpeedImpactPercent;
	j["periodicHealthImpact"] = periodicHealthImpact;
	j["periodicSpeedImpactPercent"] = periodicSpeedImpactPercent;
	return j;
}

void EnemyEffectSample::fromJson(const json &j) {
	readCommonFields(j);
	startHealthImpact = j.value("startHealthImpact", startHealthImpact);
	setStartSpeedImpactPercent(j.value("startSpeedImpactPercent", startSpeedImpactPercent));
	periodicHealthImpact = j.value("periodicHealthImpact", periodicHealthImpact);
	setPeriodicSpeedImpactPercent(j.value("periodicSpeedImpactPercent", periodicSpeedImpactPercent));
}

int EnemyEffectSample::getStartHealthImpact() const {
	return startHealthImpact;
}

void EnemyEffectSample::setStartHealthImpact(const int value) {
	startHealthImpact = value;
}

int EnemyEffectSample::getStartSpeedImpactPercent() const {
	return startSpeedImpactPercent;
}

void EnemyEffectSample::setStartSpeedImpactPercent(const int value) {
	startSpeedImpactPercent = std::max(-100, value);
}

int EnemyEffectSample::getPeriodicHealthImpact() const {
	return periodicHealthImpact;
}

void EnemyEffectSample::setPeriodicHealthImpact(const int value) {
	periodicHealthImpact = value;
}

int EnemyEffectSample::getPeriodicSpeedImpactPercent() const {
	return periodicSpeedImpactPercent;
}

void EnemyEffectSample::setPeriodicSpeedImpactPercent(const int value) {
	periodicSpeedImpactPercent = std::max(-100, value);
}
