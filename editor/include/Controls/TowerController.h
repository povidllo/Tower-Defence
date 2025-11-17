#ifndef TOWERDEFENCE_TOWERCONTROLLER_H
#define TOWERDEFENCE_TOWERCONTROLLER_H

#include <qtmetamacros.h>

#include "Project.h"
#include "TowerSample.h"

class TowerController {
public:
    explicit TowerController(const std::shared_ptr<Project> &project) : currentProject(project), currentTower(nullptr) {
    }

    void setCurrentTower(std::string name) {
        currentTower = currentProject->getTower(name);
    }

    std::shared_ptr<TowerSample> getCurrentTower() {
        return currentTower;
    }

    void changeCurrentTower(std::string name) {
        currentTower = currentProject->getTower(name);
    }


    void addTower(std::string name) {
        const auto newTower = std::make_shared<TowerSample>(name);
        currentTower = newTower;
        currentProject->addTower(newTower);
    }

    std::vector<std::string> getTowerNames() const {
        auto towers = currentProject->getTowers();
        std::vector<std::string> names;
        for (auto &[fst, snd]: towers) {
            names.push_back(fst);
        }
        return names;
    }

    bool towerExists(const std::string &name) const {
        return currentProject->existsTower(name);
    }

    std::shared_ptr<Project> getCurrentProject() const {
        return currentProject;
    }

private:
    std::shared_ptr<Project> currentProject;
    std::shared_ptr<TowerSample> currentTower;
};


#endif //TOWERDEFENCE_TOWERCONTROLLER_H
