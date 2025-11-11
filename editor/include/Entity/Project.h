#ifndef EDITOR_PROJECT_H
#define EDITOR_PROJECT_H
#include "Serializable.h"
#include "TowerSample.h"


class Project : protected ISerializable {
public:
    using json = nlohmann::json;

    /**
     * Constructor for new project
     *
     * @param name          project name
     * @param path          project path
     * @param lastSaveDate  project last save time
     */
    Project(const std::string &name, const std::string &path, const std::time_t &lastSaveDate) {
        this->name = name;
        this->lastSaveDate = lastSaveDate;
        this->path = path;
    }

    /**
     * Constructor for existing project
     *
     * @param j json that describes the project
     */
    explicit Project(const json &j) {
        Project::fromJson(j);
    }

    json toJson() const override {
        return {
            {"name", name},
            {"lastSaveDate", lastSaveDate},
            {"path", path}
        };
    }

    void fromJson(const json &j) override {
        name = j.at("name").get<std::string>();
        lastSaveDate = j.at("lastSaveDate").get<std::time_t>();
    }

    [[nodiscard]] std::string getName() const;

    void setName(const std::string &name);

    [[nodiscard]] std::string getPath() const;

    void setPath(const std::string &path);

    [[nodiscard]] std::time_t getLastSaveDate() const;

    void setLastSaveDate(std::time_t last_save_date);

    [[nodiscard]] std::unordered_map<std::string, std::shared_ptr<TowerSample> > getTowers() const;

    std::shared_ptr<TowerSample> getTower(std::string &name) const;

    void addTower(const std::shared_ptr<TowerSample> &tower);

    bool existsTower(const std::string &name) const;

private:
    std::string name;
    std::string path;
    std::time_t lastSaveDate;
    //для будущего
    // std::vector<CampaignTemplate*> campaigns;
    // std::vector<EnemyTemplate*> enemies;
    std::unordered_map<std::string, std::shared_ptr<TowerSample> > towers;
    // std::vector<AbilityTemplate*> abilities;
    // std::vector<Replay*> replays;
};


#endif //EDITOR_PROJECT_H
