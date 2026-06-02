#include "EffectSample.h"

#include <algorithm>

#include "EnemyEffectSample.h"
#include "TowerEffectSample.h"

EffectSample::EffectSample(std::string name, const Kind effectKind)
	: name{std::move(name)}, kind{effectKind} {}

const std::string &EffectSample::getName() const {
	return name;
}

void EffectSample::setName(std::string effectName) {
	name = std::move(effectName);
}

EffectSample::Kind EffectSample::getKind() const {
	return kind;
}

std::string EffectSample::getKindString() const {
	return kind == Kind::Tower ? "tower" : "enemy";
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

void EffectSample::addEffectAfterFinish(const std::string &effectName) {
	if (std::find(effectsAfterFinish.begin(), effectsAfterFinish.end(), effectName) == effectsAfterFinish.end()) {
		effectsAfterFinish.push_back(effectName);
	}
}

bool EffectSample::removeEffectAfterFinish(const std::string &effectName) {
	const auto it = std::remove(effectsAfterFinish.begin(), effectsAfterFinish.end(), effectName);
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

void EffectSample::readCommonFields(const json &j) {
	name = j.value("name", name);
	durationSeconds = j.value("durationSeconds", durationSeconds);
	periodSeconds = j.value("periodSeconds", periodSeconds);
	stackable = j.value("stackable", stackable);
	visualTexturePath = j.value("visualTexturePath", visualTexturePath);

	if (j.contains("effectsAfterFinish") && j["effectsAfterFinish"].is_array()) {
		effectsAfterFinish.clear();
		for (const auto &effectName : j["effectsAfterFinish"]) {
			if (effectName.is_string()) {
				effectsAfterFinish.push_back(effectName.get<std::string>());
			}
		}
	}
}

EffectSample::json EffectSample::writeCommonFields() const {
	json afterFinish = json::array();
	for (const auto &effectName : effectsAfterFinish) {
		afterFinish.push_back(effectName);
	}

	return {
		{"name", name},
		{"kind", getKindString()},
		{"durationSeconds", durationSeconds},
		{"periodSeconds", periodSeconds},
		{"stackable", stackable},
		{"effectsAfterFinish", afterFinish},
		{"visualTexturePath", visualTexturePath},
	};
}

std::shared_ptr<EffectSample> createEffectFromJson(const EffectSample::json &j) {
	const std::string kind = j.value("kind", "");
	const std::string targetType = j.value("targetType", "");

	if (kind == "tower" || targetType == "tower") {
		return std::make_shared<TowerEffectSample>(j);
	}
	if (kind == "enemy" || targetType == "unit" || targetType.empty()) {
		return std::make_shared<EnemyEffectSample>(j);
	}
	return std::make_shared<EnemyEffectSample>(j);
}

std::shared_ptr<EffectSample> createEffect(const std::string &name, const EffectSample::Kind kind) {
	if (kind == EffectSample::Kind::Tower) {
		return std::make_shared<TowerEffectSample>(name);
	}
	return std::make_shared<EnemyEffectSample>(name);
}
