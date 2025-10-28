#ifndef EDITOR_PROJECT_H
#define EDITOR_PROJECT_H
#include "Serializable.h"


class Project : protected  ISerializable{
    using json = nlohmann::json;
    std::string name;
    std::string path;
    std::time_t lastSaveDate;
    //для будущего
    // std::vector<CampaignTemplate*> campaigns;
    // std::vector<EnemyTemplate*> enemies;
    // std::vector<TowerTemplate*> towers;
    // std::vector<AbilityTemplate*> abilities;
    // std::vector<Replay*> replays;
    public:
    /**
     * Constructor for new project
     *
     * @param name          project name
     * @param path          project path
     * @param lastSaveDate  project last save time
     */
    Project(const std::string& name, const std::string& path,const std::time_t& lastSaveDate) {
        this->name = name;
        this->lastSaveDate = lastSaveDate;
        this->path = path;
    }

    /**
     * Constructor for existing project
     *
     * @param j json that describes the project
     */
    explicit Project(const json& j) {
        Project::fromJson(j);
    }

    json toJson() const override {
        return {
            {"name", name},
            {"lastSaveDate", lastSaveDate},
            {"path", path}
        };
    }

    void fromJson(const json& j) override {
        name = j.at("name").get<std::string>();
        lastSaveDate = j.at("lastSaveDate").get<std::time_t>();
    }

};


#endif //EDITOR_PROJECT_H