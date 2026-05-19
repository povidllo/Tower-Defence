#include "EffectCreatorController.h"

#include <algorithm>

#include "ProjectController.h"

EffectCreatorController::EffectCreatorController(ProjectController *projectController)
	: projectController(projectController), currentEffectCreator(nullptr) {}

void EffectCreatorController::setCurrentEffectCreator(const std::string &name) {
	for (auto &effectCreator: projectController->getEffectCreators()) {
		if (effectCreator->getName() == name) {
			currentEffectCreator = effectCreator;
			return;
		}
	}
}

std::shared_ptr<EffectCreatorSample> EffectCreatorController::getCurrentEffectCreator() {
	return currentEffectCreator;
}

void EffectCreatorController::addEffectCreator(const std::string &name) {
	auto effectCreator = std::make_shared<EffectCreatorSample>(name);
	currentEffectCreator = effectCreator;
	projectController->getEffectCreators().push_back(effectCreator);
}

bool EffectCreatorController::removeEffectCreator(const std::string &name) const {
	auto &effectCreators = projectController->getEffectCreators();
	const auto it = std::find_if(effectCreators.begin(), effectCreators.end(), [&name](const auto &effectCreator) {
		return effectCreator->getName() == name;
	});
	if (it == effectCreators.end()) {
		return false;
	}

	effectCreators.erase(it);
	removeReferences(name);
	return true;
}

std::vector<std::string> EffectCreatorController::getEffectCreatorNames() const {
	std::vector<std::string> names;
	for (const auto &effectCreator: projectController->getEffectCreators()) {
		names.push_back(effectCreator->getName());
	}
	return names;
}

std::vector<std::string> EffectCreatorController::getEffectNames() const {
	std::vector<std::string> names;
	for (const auto &effect: projectController->getEffects()) {
		names.push_back(effect->getName());
	}
	return names;
}

bool EffectCreatorController::effectCreatorExists(const std::string &name) const {
	for (const auto &effectCreator: projectController->getEffectCreators()) {
		if (effectCreator->getName() == name) {
			return true;
		}
	}
	return false;
}

void EffectCreatorController::renameReferences(const std::string &oldName, const std::string &newName) const {
	projectController->renameEffectCreatorReferences(oldName, newName);
}

void EffectCreatorController::removeReferences(const std::string &name) const {
	projectController->removeEffectCreatorReferences(name);
}
