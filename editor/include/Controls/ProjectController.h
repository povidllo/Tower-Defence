#ifndef TOWERDEFENCE_PROJECTCONTROLLER_H
#define TOWERDEFENCE_PROJECTCONTROLLER_H

#include "BaseController.h"

/**
 * Control class that manage entity class Project
 */
class ProjectController : public BaseController{
public:

    /**
     * Constructor for loading existing project from json file
     *
     * @param path path to json file
     */
    ProjectController(const std::string& path);

    /**
     * Creating new project at the specified path
     *
     * @param path path where we will create a new project
     * @param name project's name
     */
    ProjectController(const std::string& path, const std::string& name);

private:
    std::shared_ptr<Project> currentProject;
//     MapController* mapController;
//     TowerController* towerController;
//     EnemyController* enemyController;
//     WaveController* waveController;
//     CampaignController* campaignController;


};


#endif //TOWERDEFENCE_PROJECTCONTROLLER_H