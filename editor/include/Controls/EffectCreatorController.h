#ifndef TOWERDEFENCE_EFFECTCREATORCONTROLLER_H
#define TOWERDEFENCE_EFFECTCREATORCONTROLLER_H

#include <memory>
#include <string>
#include <vector>

#include "EffectCreatorSample.h"

class ProjectController;

class EffectCreatorController {
public:
	explicit EffectCreatorController(ProjectController *projectController);

	void setCurrentEffectCreator(const std::string &name);

	std::shared_ptr<EffectCreatorSample> getCurrentEffectCreator();

	void addEffectCreator(const std::string &name);

	bool removeEffectCreator(const std::string &name) const;

	[[nodiscard]] std::vector<std::string> getEffectCreatorNames() const;

	[[nodiscard]] std::vector<std::string> getEffectNames() const;

	[[nodiscard]] bool effectCreatorExists(const std::string &name) const;

	void renameReferences(const std::string &oldName, const std::string &newName) const;

	void removeReferences(const std::string &name) const;

private:
	ProjectController *projectController;
	std::shared_ptr<EffectCreatorSample> currentEffectCreator;
};

#endif // TOWERDEFENCE_EFFECTCREATORCONTROLLER_H
