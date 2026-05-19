#include "EffectSample.h"

#include <algorithm>

EffectSample::EffectSample(std::string name) : name(std::move(name)) {}

EffectSample::EffectSample(const json &j) {
	fromJson(j);
}

EffectSample::json EffectSample::toJson() const {
	json afterFinish = json::array();
	for (const auto &effectName: effectsAfterFinish) {
		afterFinish.push_back(effectName);
	}

	json j = {
		{"name", name},
		{"targetType", getTargetTypeString()},
		{"durationSeconds", durationSeconds},
		{"periodSeconds", periodSeconds},
		{"stackable", stackable},
		{"effectsAfterFinish", afterFinish},
		{"visualTexturePath", visualTexturePath},
	};

	if (targetType == TargetType::Unit) {
		j["startHealthImpact"] = startHealthImpact;
		j["startSpeedImpactPercent"] = startSpeedImpactPercent;
		j["periodicHealthImpact"] = periodicHealthImpact;
		j["periodicSpeedImpactPercent"] = periodicSpeedImpactPercent;
	} else {
		j["startDamageFlatImpact"] = startDamageFlatImpact;
		j["startDamagePercentImpact"] = startDamagePercentImpact;
		j["startAttackSpeedPercentImpact"] = startAttackSpeedPercentImpact;
		j["periodicDamageFlatImpact"] = periodicDamageFlatImpact;
		j["periodicDamagePercentImpact"] = periodicDamagePercentImpact;
		j["periodicAttackSpeedPercentImpact"] = periodicAttackSpeedPercentImpact;
	}

	return j;
}

void EffectSample::fromJson(const json &j) {
	name = j.value("name", name);
	setTargetTypeString(j.value("targetType", getTargetTypeString()));

	durationSeconds = j.value("durationSeconds", durationSeconds);
	periodSeconds = j.value("periodSeconds", periodSeconds);
	stackable = j.value("stackable", stackable);
	visualTexturePath = j.value("visualTexturePath", visualTexturePath);

	startHealthImpact = j.value("startHealthImpact", startHealthImpact);
	setStartSpeedImpactPercent(j.value("startSpeedImpactPercent", startSpeedImpactPercent));
	periodicHealthImpact = j.value("periodicHealthImpact", periodicHealthImpact);
	setPeriodicSpeedImpactPercent(j.value("periodicSpeedImpactPercent", periodicSpeedImpactPercent));

	startDamageFlatImpact = j.value("startDamageFlatImpact", startDamageFlatImpact);
	startDamagePercentImpact = j.value("startDamagePercentImpact", startDamagePercentImpact);
	startAttackSpeedPercentImpact = j.value("startAttackSpeedPercentImpact", startAttackSpeedPercentImpact);
	periodicDamageFlatImpact = j.value("periodicDamageFlatImpact", periodicDamageFlatImpact);
	periodicDamagePercentImpact = j.value("periodicDamagePercentImpact", periodicDamagePercentImpact);
	periodicAttackSpeedPercentImpact = j.value("periodicAttackSpeedPercentImpact", periodicAttackSpeedPercentImpact);

	if (j.contains("effectsAfterFinish") && j["effectsAfterFinish"].is_array()) {
		effectsAfterFinish.clear();
		for (const auto &effectName: j["effectsAfterFinish"]) {
			if (effectName.is_string()) {
				effectsAfterFinish.push_back(effectName.get<std::string>());
			}
		}
	}
}

const std::string &EffectSample::getName() const {
	return name;
}

void EffectSample::setName(std::string name) {
	this->name = std::move(name);
}

EffectSample::TargetType EffectSample::getTargetType() const {
	return targetType;
}

void EffectSample::setTargetType(const TargetType targetType) {
	this->targetType = targetType;
}

std::string EffectSample::getTargetTypeString() const {
	return targetType == TargetType::Unit ? "unit" : "tower";
}

void EffectSample::setTargetTypeString(const std::string &targetType) {
	this->targetType = targetType == "tower" ? TargetType::Tower : TargetType::Unit;
}

int EffectSample::getStartHealthImpact() const {
	return startHealthImpact;
}

void EffectSample::setStartHealthImpact(const int value) {
	startHealthImpact = value;
}

int EffectSample::getStartSpeedImpactPercent() const {
	return startSpeedImpactPercent;
}

void EffectSample::setStartSpeedImpactPercent(const int value) {
	startSpeedImpactPercent = std::max(-100, value);
}

int EffectSample::getPeriodicHealthImpact() const {
	return periodicHealthImpact;
}

void EffectSample::setPeriodicHealthImpact(const int value) {
	periodicHealthImpact = value;
}

int EffectSample::getPeriodicSpeedImpactPercent() const {
	return periodicSpeedImpactPercent;
}

void EffectSample::setPeriodicSpeedImpactPercent(const int value) {
	periodicSpeedImpactPercent = std::max(-100, value);
}

int EffectSample::getStartDamageFlatImpact() const {
	return startDamageFlatImpact;
}

void EffectSample::setStartDamageFlatImpact(const int value) {
	startDamageFlatImpact = value;
}

int EffectSample::getStartDamagePercentImpact() const {
	return startDamagePercentImpact;
}

void EffectSample::setStartDamagePercentImpact(const int value) {
	startDamagePercentImpact = value;
}

int EffectSample::getStartAttackSpeedPercentImpact() const {
	return startAttackSpeedPercentImpact;
}

void EffectSample::setStartAttackSpeedPercentImpact(const int value) {
	startAttackSpeedPercentImpact = value;
}

int EffectSample::getPeriodicDamageFlatImpact() const {
	return periodicDamageFlatImpact;
}

void EffectSample::setPeriodicDamageFlatImpact(const int value) {
	periodicDamageFlatImpact = value;
}

int EffectSample::getPeriodicDamagePercentImpact() const {
	return periodicDamagePercentImpact;
}

void EffectSample::setPeriodicDamagePercentImpact(const int value) {
	periodicDamagePercentImpact = value;
}

int EffectSample::getPeriodicAttackSpeedPercentImpact() const {
	return periodicAttackSpeedPercentImpact;
}

void EffectSample::setPeriodicAttackSpeedPercentImpact(const int value) {
	periodicAttackSpeedPercentImpact = value;
}

double EffectSample::getDurationSeconds() const {
	return durationSeconds;
}

void EffectSample::setDurationSeconds(const double value) {
	durationSeconds = value;
}

double EffectSample::getPeriodSeconds() const {
	return periodSeconds;
}

void EffectSample::setPeriodSeconds(const double value) {
	periodSeconds = value;
}

bool EffectSample::isStackable() const {
	return stackable;
}

void EffectSample::setStackable(const bool value) {
	stackable = value;
}

const std::vector<std::string> &EffectSample::getEffectsAfterFinish() const {
	return effectsAfterFinish;
}

void EffectSample::setEffectsAfterFinish(std::vector<std::string> effects) {
	effectsAfterFinish = std::move(effects);
}

void EffectSample::addEffectAfterFinish(const std::string &name) {
	if (std::find(effectsAfterFinish.begin(), effectsAfterFinish.end(), name) == effectsAfterFinish.end()) {
		effectsAfterFinish.push_back(name);
	}
}

bool EffectSample::removeEffectAfterFinish(const std::string &name) {
	const auto it = std::remove(effectsAfterFinish.begin(), effectsAfterFinish.end(), name);
	if (it == effectsAfterFinish.end()) {
		return false;
	}
	effectsAfterFinish.erase(it, effectsAfterFinish.end());
	return true;
}

const std::string &EffectSample::getVisualTexturePath() const {
	return visualTexturePath;
}

void EffectSample::setVisualTexturePath(std::string path) {
	visualTexturePath = std::move(path);
}
