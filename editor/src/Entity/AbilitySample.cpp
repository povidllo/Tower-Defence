#include "AbilitySample.h"

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

AbilitySample::AbilitySample(std::string name) : name(std::move(name)) {}

AbilitySample::AbilitySample(const json &j) {
	fromJson(j);
}

AbilitySample::json AbilitySample::toJson() const {
	return {
		{"name", name},
		{"effectCreatorsOnCast", stringVectorToJson(effectCreatorsOnCast)},
		{"targetSelection", targetSelection},
		{"chargesCount", chargesCount},
		{"chargeCooldownSeconds", chargeCooldownSeconds},
		{"fullCooldownSeconds", fullCooldownSeconds},
	};
}

void AbilitySample::fromJson(const json &j) {
	name = j.value("name", name);
	if (j.contains("effectCreatorsOnCast")) {
		effectCreatorsOnCast = stringVectorFromJson(j["effectCreatorsOnCast"]);
	}
	setTargetSelection(j.value("targetSelection", targetSelection));
	setChargesCount(j.value("chargesCount", chargesCount));
	setChargeCooldownSeconds(j.value("chargeCooldownSeconds", chargeCooldownSeconds));
	setFullCooldownSeconds(j.value("fullCooldownSeconds", fullCooldownSeconds));
}

const std::string &AbilitySample::getName() const {
	return name;
}

void AbilitySample::setName(std::string name) {
	this->name = std::move(name);
}

const std::vector<std::string> &AbilitySample::getEffectCreatorsOnCast() const {
	return effectCreatorsOnCast;
}

void AbilitySample::setEffectCreatorsOnCast(std::vector<std::string> effectCreators) {
	effectCreatorsOnCast = std::move(effectCreators);
}

void AbilitySample::addEffectCreatorOnCast(const std::string &name) {
	addUnique(effectCreatorsOnCast, name);
}

bool AbilitySample::removeEffectCreatorOnCast(const std::string &name) {
	return removeValue(effectCreatorsOnCast, name);
}

void AbilitySample::renameEffectCreatorReference(const std::string &oldName, const std::string &newName) {
	for (auto &name: effectCreatorsOnCast) {
		if (name == oldName) {
			name = newName;
		}
	}
}

void AbilitySample::removeEffectCreatorReference(const std::string &name) {
	removeValue(effectCreatorsOnCast, name);
}

const std::string &AbilitySample::getTargetSelection() const {
	return targetSelection;
}

void AbilitySample::setTargetSelection(std::string value) {
	if (value != "point" && value != "tower" && value != "unit") {
		value = "none";
	}
	targetSelection = std::move(value);
}

int AbilitySample::getChargesCount() const {
	return chargesCount;
}

void AbilitySample::setChargesCount(const int value) {
	chargesCount = std::max(1, value);
}

double AbilitySample::getChargeCooldownSeconds() const {
	return chargeCooldownSeconds;
}

void AbilitySample::setChargeCooldownSeconds(const double value) {
	chargeCooldownSeconds = std::max(0.0, value);
}

double AbilitySample::getFullCooldownSeconds() const {
	return fullCooldownSeconds;
}

void AbilitySample::setFullCooldownSeconds(const double value) {
	fullCooldownSeconds = std::max(0.0, value);
}
