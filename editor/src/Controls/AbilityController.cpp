#include "AbilityController.h"

#include <algorithm>

#include "ProjectController.h"

AbilityController::AbilityController(ProjectController *projectController)
	: projectController(projectController), currentAbility(nullptr) {}

void AbilityController::setCurrentAbility(const std::string &name) {
	for (auto &ability: projectController->getAbilities()) {
		if (ability->getName() == name) {
			currentAbility = ability;
			return;
		}
	}
}

std::shared_ptr<AbilitySample> AbilityController::getCurrentAbility() {
	return currentAbility;
}

void AbilityController::addAbility(const std::string &name) {
	auto ability = std::make_shared<AbilitySample>(name);
	currentAbility = ability;
	projectController->getAbilities().push_back(ability);
}

bool AbilityController::removeAbility(const std::string &name) const {
	auto &abilities = projectController->getAbilities();
	const auto it = std::find_if(abilities.begin(), abilities.end(), [&name](const auto &ability) {
		return ability->getName() == name;
	});
	if (it == abilities.end()) {
		return false;
	}

	abilities.erase(it);
	removeReferences(name);
	return true;
}

std::vector<std::string> AbilityController::getAbilityNames() const {
	std::vector<std::string> names;
	for (const auto &ability: projectController->getAbilities()) {
		names.push_back(ability->getName());
	}
	return names;
}

std::vector<std::string> AbilityController::getEffectCreatorNames() const {
	std::vector<std::string> names;
	for (const auto &effectCreator: projectController->getEffectCreators()) {
		names.push_back(effectCreator->getName());
	}
	return names;
}

bool AbilityController::abilityExists(const std::string &name) const {
	for (const auto &ability: projectController->getAbilities()) {
		if (ability->getName() == name) {
			return true;
		}
	}
	return false;
}

void AbilityController::renameReferences(const std::string &oldName, const std::string &newName) const {
	projectController->renameAbilityReferences(oldName, newName);
}

void AbilityController::removeReferences(const std::string &name) const {
	projectController->removeAbilityReferences(name);
}
