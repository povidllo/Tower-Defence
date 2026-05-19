#ifndef TOWERDEFENCE_ABILITYCONTROLLER_H
#define TOWERDEFENCE_ABILITYCONTROLLER_H

#include <memory>
#include <string>
#include <vector>

#include "AbilitySample.h"

class ProjectController;

class AbilityController {
public:
	explicit AbilityController(ProjectController *projectController);

	void setCurrentAbility(const std::string &name);

	std::shared_ptr<AbilitySample> getCurrentAbility();

	void addAbility(const std::string &name);

	bool removeAbility(const std::string &name) const;

	[[nodiscard]] std::vector<std::string> getAbilityNames() const;

	[[nodiscard]] std::vector<std::string> getEffectCreatorNames() const;

	[[nodiscard]] bool abilityExists(const std::string &name) const;

	void renameReferences(const std::string &oldName, const std::string &newName) const;

	void removeReferences(const std::string &name) const;

private:
	ProjectController *projectController;
	std::shared_ptr<AbilitySample> currentAbility;
};

#endif // TOWERDEFENCE_ABILITYCONTROLLER_H
