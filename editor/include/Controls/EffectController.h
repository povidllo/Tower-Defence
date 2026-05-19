#ifndef TOWERDEFENCE_EFFECTCONTROLLER_H
#define TOWERDEFENCE_EFFECTCONTROLLER_H

#include <memory>
#include <string>
#include <vector>

#include "EffectSample.h"

class ProjectController;

class EffectController {
public:
	explicit EffectController(ProjectController *projectController);

	void setCurrentEffect(const std::string &name);

	std::shared_ptr<EffectSample> getCurrentEffect();

	void addEffect(const std::string &name);

	bool removeEffect(const std::string &name) const;

	[[nodiscard]] std::vector<std::string> getEffectNames() const;

	[[nodiscard]] bool effectExists(const std::string &name) const;

	void renameReferences(const std::string &oldName, const std::string &newName) const;

	void removeReferences(const std::string &name) const;

private:
	ProjectController *projectController;
	std::shared_ptr<EffectSample> currentEffect;
};

#endif // TOWERDEFENCE_EFFECTCONTROLLER_H
