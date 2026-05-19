#include "EffectCreatorSample.h"

#include <algorithm>
#include <utility>

namespace {
	nlohmann::ordered_json stringVectorToJson(const std::vector<std::string> &values) {
		auto result = nlohmann::ordered_json::array();
		for (const auto &value: values) {
			result.push_back(value);
		}
		return result;
	}

	std::vector<std::string> stringVectorFromJson(const nlohmann::ordered_json &j) {
		std::vector<std::string> result;
		if (!j.is_array()) {
			return result;
		}
		for (const auto &value: j) {
			if (value.is_string()) {
				result.push_back(value.get<std::string>());
			}
		}
		return result;
	}

	void addUnique(std::vector<std::string> &values, const std::string &name) {
		if (std::find(values.begin(), values.end(), name) == values.end()) {
			values.push_back(name);
		}
	}

	bool removeValue(std::vector<std::string> &values, const std::string &name) {
		const auto it = std::remove(values.begin(), values.end(), name);
		if (it == values.end()) {
			return false;
		}
		values.erase(it, values.end());
		return true;
	}
}

EffectCreatorSample::EffectCreatorSample(std::string name) : name(std::move(name)) {}

EffectCreatorSample::EffectCreatorSample(const json &j) {
	fromJson(j);
}

EffectCreatorSample::json EffectCreatorSample::toJson() const {
	return {
		{"name", name},
		{"startEffects", stringVectorToJson(startEffects)},
		{"periodicEffects", stringVectorToJson(periodicEffects)},
		{"radius", radius},
		{"durationSeconds", durationSeconds},
		{"periodSeconds", periodSeconds},
		{"targetRelation", targetRelation},
		{"attachmentPreference", attachmentPreference},
	};
}

void EffectCreatorSample::fromJson(const json &j) {
	name = j.value("name", name);
	if (j.contains("startEffects")) {
		startEffects = stringVectorFromJson(j["startEffects"]);
	}
	if (j.contains("periodicEffects")) {
		periodicEffects = stringVectorFromJson(j["periodicEffects"]);
	}
	setRadius(j.value("radius", radius));
	setDurationSeconds(j.value("durationSeconds", durationSeconds));
	setPeriodSeconds(j.value("periodSeconds", periodSeconds));
	setTargetRelation(j.value("targetRelation", targetRelation));
	setAttachmentPreference(j.value("attachmentPreference", attachmentPreference));
}

const std::string &EffectCreatorSample::getName() const {
	return name;
}

void EffectCreatorSample::setName(std::string name) {
	this->name = std::move(name);
}

const std::vector<std::string> &EffectCreatorSample::getStartEffects() const {
	return startEffects;
}

void EffectCreatorSample::setStartEffects(std::vector<std::string> effects) {
	startEffects = std::move(effects);
}

void EffectCreatorSample::addStartEffect(const std::string &name) {
	addUnique(startEffects, name);
}

bool EffectCreatorSample::removeStartEffect(const std::string &name) {
	return removeValue(startEffects, name);
}

const std::vector<std::string> &EffectCreatorSample::getPeriodicEffects() const {
	return periodicEffects;
}

void EffectCreatorSample::setPeriodicEffects(std::vector<std::string> effects) {
	periodicEffects = std::move(effects);
}

void EffectCreatorSample::addPeriodicEffect(const std::string &name) {
	addUnique(periodicEffects, name);
}

bool EffectCreatorSample::removePeriodicEffect(const std::string &name) {
	return removeValue(periodicEffects, name);
}

double EffectCreatorSample::getRadius() const {
	return radius;
}

void EffectCreatorSample::setRadius(const double value) {
	radius = std::max(0.0, value);
}

double EffectCreatorSample::getDurationSeconds() const {
	return durationSeconds;
}

void EffectCreatorSample::setDurationSeconds(const double value) {
	durationSeconds = std::max(0.0, value);
}

double EffectCreatorSample::getPeriodSeconds() const {
	return periodSeconds;
}

void EffectCreatorSample::setPeriodSeconds(const double value) {
	periodSeconds = std::max(0.0, value);
}

const std::string &EffectCreatorSample::getTargetRelation() const {
	return targetRelation;
}

void EffectCreatorSample::setTargetRelation(std::string value) {
	if (value != "allies" && value != "neutral") {
		value = "enemies";
	}
	targetRelation = std::move(value);
}

const std::string &EffectCreatorSample::getAttachmentPreference() const {
	return attachmentPreference;
}

void EffectCreatorSample::setAttachmentPreference(std::string value) {
	if (value != "object") {
		value = "point";
	}
	attachmentPreference = std::move(value);
}
