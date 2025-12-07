#ifndef TOWERDEFENCE_BASECONTROLLER_H
#define TOWERDEFENCE_BASECONTROLLER_H

#include "Project.h"

/**
 * Base class for each controller
 */
class BaseController {
protected:
	std::shared_ptr<Project> currentProject = nullptr;
	bool modified = false;

public:
	void SetCurrentProject(const std::shared_ptr<Project> &project);

	std::shared_ptr<Project> GetCurrentProject() const {
		return currentProject;
	}

	bool IsModified() const {
		return modified;
	};

	void SetModified(const bool &value) {
		modified = value;
	};
};

inline void BaseController::SetCurrentProject(const std::shared_ptr<Project> &project) {
	currentProject = project;
}

#endif //TOWERDEFENCE_BASECONTROLLER_H
