#ifndef TOWERDEFENCE_PROJECTCONTROLLER_H
#define TOWERDEFENCE_PROJECTCONTROLLER_H

#include "BaseController.h"
#include "TowerController.h"

/**
 * Control class that manage entity class Project
 */
class ProjectController : public BaseController, std::enable_shared_from_this<ProjectController> {
public:
    using json = nlohmann::json;
    /**
     * Constructor for loading existing project from json file
     *
     * @param path path to json file
     */
    ProjectController(const std::string &path);

    /**
     * Creating new project at the specified path
     *
     * @param path path where we will create a new project
     * @param name project's name
     */
    ProjectController(const std::string &path, const std::string &name);

    std::shared_ptr<TowerController> getTowerController();

    json toJson() const {
        return currentProject->toJson();
    }

    void fromJson(const json &j) {
        currentProject->fromJson(j);
    }

    std::string getProjectName() const {
        return currentProject->getName();
    }

    void setProjectName(const std::string &name) const {
        currentProject->setName(name);
    }

    std::string getProjectPath() const {
        return currentProject->getPath();
    }

    void setProjectPath(const std::string &path) const {
        currentProject->setPath(path);
    }

    std::time_t getProjectLastSaveDate() const {
        return currentProject->getLastSaveDate();
    }

    void setProjectLastSaveDate(const std::time_t &date) const {
        currentProject->setLastSaveDate(date);
    }

    std::vector<std::shared_ptr<TowerSample> > &getTowers() const {
        return currentProject->getTowers();
    }

private:
    std::shared_ptr<Project> currentProject;
    std::shared_ptr<TowerController> towerController;
    //     MapController* mapController;
    //     TowerController* towerController;
    //     EnemyController* enemyController;
    //     WaveController* waveController;
    //     CampaignController* campaignController;

    /**
     * Method for initializing all controllers
     *
     */
    void loadControls();
};


#endif //TOWERDEFENCE_PROJECTCONTROLLER_H
