#include "EffectController.h"

#include <algorithm>

#include "ProjectController.h"

EffectController::EffectController(ProjectController *projectController)
	: projectController(projectController), currentEffect(nullptr) {}

void EffectController::setCurrentEffect(const std::string &name) {
	for (auto &effect: projectController->getEffects()) {
		if (effect->getName() == name) {
			currentEffect = effect;
			return;
		}
	}
}

std::shared_ptr<EffectSample> EffectController::getCurrentEffect() {
	return currentEffect;
}

void EffectController::addEffect(const std::string &name, const EffectSample::Kind kind) {
	auto effect = createEffect(name, kind);
	currentEffect = effect;
	projectController->getEffects().push_back(effect);
}

bool EffectController::removeEffect(const std::string &name) const {
	auto &effects = projectController->getEffects();
	const auto it = std::find_if(effects.begin(), effects.end(), [&name](const auto &effect) {
		return effect->getName() == name;
	});
	if (it == effects.end()) {
		return false;
	}

	effects.erase(it);
	removeReferences(name);
	return true;
}

std::vector<std::string> EffectController::getEffectNames() const {
	std::vector<std::string> names;
	for (const auto &effect: projectController->getEffects()) {
		names.push_back(effect->getName());
	}
	return names;
}

bool EffectController::effectExists(const std::string &name) const {
	for (const auto &effect: projectController->getEffects()) {
		if (effect->getName() == name) {
			return true;
		}
	}
	return false;
}

void EffectController::renameReferences(const std::string &oldName, const std::string &newName) const {
	for (const auto &effect: projectController->getEffects()) {
		auto afterFinish = effect->getEffectsAfterFinish();
		bool changed = false;
		for (auto &name: afterFinish) {
			if (name == oldName) {
				name = newName;
				changed = true;
			}
		}
		if (changed) {
			effect->setEffectsAfterFinish(afterFinish);
		}
	}
	for (const auto &effectCreator: projectController->getEffectCreators()) {
		auto startEffects = effectCreator->getStartEffects();
		auto periodicEffects = effectCreator->getPeriodicEffects();
		bool changed = false;
		for (auto &name: startEffects) {
			if (name == oldName) {
				name = newName;
				changed = true;
			}
		}
		for (auto &name: periodicEffects) {
			if (name == oldName) {
				name = newName;
				changed = true;
			}
		}
		if (changed) {
			effectCreator->setStartEffects(startEffects);
			effectCreator->setPeriodicEffects(periodicEffects);
		}
	}
}

void EffectController::removeReferences(const std::string &name) const {
	for (const auto &effect: projectController->getEffects()) {
		effect->removeEffectAfterFinish(name);
	}
	for (const auto &effectCreator: projectController->getEffectCreators()) {
		effectCreator->removeStartEffect(name);
		effectCreator->removePeriodicEffect(name);
	}
}
