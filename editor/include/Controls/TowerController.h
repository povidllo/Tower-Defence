#ifndef TOWERDEFENCE_TOWERCONTROLLER_H
#define TOWERDEFENCE_TOWERCONTROLLER_H

#include <qtmetamacros.h>

#include "TowerSample.h"
#include <fstream>


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

    bool removeTower(const std::string &name) const;

    std::vector<std::string> getTowerNames() const;

    bool towerExists(const std::string &name) const;

    void setTowerTexture(const std::string &path) {
        if (!isPngBySignature(path)) {
            throw std::invalid_argument("Tower texture does not have .png format");
        }
        currentTower->setTowerTexturePath(path);
    }

private:
    // std::shared_ptr<Project> currentProject;
    // std::weak_ptr<ProjectController> projectController;
    ProjectController *projectController;
    std::shared_ptr<TowerSample> currentTower;

    bool isPngBySignature(const std::string &path) {
        std::ifstream file(path, std::ios::binary);
        if (!file) return false;

        unsigned char pngSignature[8] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
        unsigned char header[8] = {};

        file.read(reinterpret_cast<char *>(header), 8);

        return std::equal(std::begin(pngSignature), std::end(pngSignature), header);
    }
};


#endif //TOWERDEFENCE_TOWERCONTROLLER_H
