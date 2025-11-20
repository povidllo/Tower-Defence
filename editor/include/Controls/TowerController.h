#ifndef TOWERDEFENCE_TOWERCONTROLLER_H
#define TOWERDEFENCE_TOWERCONTROLLER_H

#include <qtmetamacros.h>

#include "TowerSample.h"


class ProjectController;


class TowerController {
public:
    explicit TowerController(ProjectController *projectController)
        : projectController(projectController), currentTower(nullptr) {
    }

    // explicit TowerController(const std::shared_ptr<ProjectController> &projectController)
    //     : projectController(std::move(projectController)), currentTower(nullptr) {
    // }

    void setCurrentTower(const std::string &name);

    std::shared_ptr<TowerSample> getCurrentTower();

    // void changeCurrentTower(const std::string &name);


    void addTower(std::string name);

    bool removeTower(const std::string &name);

    std::vector<std::string> getTowerNames() const;

    bool towerExists(const std::string &name) const;

private:
    // std::shared_ptr<Project> currentProject;
    // std::weak_ptr<ProjectController> projectController;
    ProjectController *projectController;
    std::shared_ptr<TowerSample> currentTower;
};


#endif //TOWERDEFENCE_TOWERCONTROLLER_H
