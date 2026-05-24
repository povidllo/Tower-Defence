#include "TowerEffectSample.h"

TowerEffectSample::TowerEffectSample(std::string name)
	: EffectSample(std::move(name), Kind::Tower) {}

TowerEffectSample::TowerEffectSample(const json &j) : EffectSample("", Kind::Tower) {
	fromJson(j);
}

TowerEffectSample::json TowerEffectSample::toJson() const {
	json j = writeCommonFields();
	j["startDamageFlatImpact"] = startDamageFlatImpact;
	j["startDamagePercentImpact"] = startDamagePercentImpact;
	j["startAttackSpeedPercentImpact"] = startAttackSpeedPercentImpact;
	j["periodicDamageFlatImpact"] = periodicDamageFlatImpact;
	j["periodicDamagePercentImpact"] = periodicDamagePercentImpact;
	j["periodicAttackSpeedPercentImpact"] = periodicAttackSpeedPercentImpact;
	return j;
}

void TowerEffectSample::fromJson(const json &j) {
	readCommonFields(j);
	startDamageFlatImpact = j.value("startDamageFlatImpact", startDamageFlatImpact);
	startDamagePercentImpact = j.value("startDamagePercentImpact", startDamagePercentImpact);
	startAttackSpeedPercentImpact = j.value("startAttackSpeedPercentImpact", startAttackSpeedPercentImpact);
	periodicDamageFlatImpact = j.value("periodicDamageFlatImpact", periodicDamageFlatImpact);
	periodicDamagePercentImpact = j.value("periodicDamagePercentImpact", periodicDamagePercentImpact);
	periodicAttackSpeedPercentImpact = j.value("periodicAttackSpeedPercentImpact", periodicAttackSpeedPercentImpact);
}

int TowerEffectSample::getStartDamageFlatImpact() const {
	return startDamageFlatImpact;
}

void TowerEffectSample::setStartDamageFlatImpact(const int value) {
	startDamageFlatImpact = value;
}

int TowerEffectSample::getStartDamagePercentImpact() const {
	return startDamagePercentImpact;
}

void TowerEffectSample::setStartDamagePercentImpact(const int value) {
	startDamagePercentImpact = value;
}

int TowerEffectSample::getStartAttackSpeedPercentImpact() const {
	return startAttackSpeedPercentImpact;
}

void TowerEffectSample::setStartAttackSpeedPercentImpact(const int value) {
	startAttackSpeedPercentImpact = value;
}

int TowerEffectSample::getPeriodicDamageFlatImpact() const {
	return periodicDamageFlatImpact;
}

void TowerEffectSample::setPeriodicDamageFlatImpact(const int value) {
	periodicDamageFlatImpact = value;
}

int TowerEffectSample::getPeriodicDamagePercentImpact() const {
	return periodicDamagePercentImpact;
}

void TowerEffectSample::setPeriodicDamagePercentImpact(const int value) {
	periodicDamagePercentImpact = value;
}

int TowerEffectSample::getPeriodicAttackSpeedPercentImpact() const {
	return periodicAttackSpeedPercentImpact;
}

void TowerEffectSample::setPeriodicAttackSpeedPercentImpact(const int value) {
	periodicAttackSpeedPercentImpact = value;
}
