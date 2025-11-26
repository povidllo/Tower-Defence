#ifndef EDITOR_PROJECT_H
#define EDITOR_PROJECT_H
#include "EnemySample.h"
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
    Project(const std::string &name, const std::string &path, const std::time_t &lastSaveDate);

    /**
     * Constructor for existing project
     *
     * @param j json that describes the project
     */
    explicit Project(const json &j);

    json toJson() const override;

    void fromJson(const json &j) override;

    [[nodiscard]] std::string getName() const;

    void setName(const std::string &name);

    [[nodiscard]] std::string getPath() const;

    void setPath(const std::string &path);

    [[nodiscard]] std::time_t getLastSaveDate() const;

    void setLastSaveDate(std::time_t last_save_date);

    [[nodiscard]] std::vector<std::shared_ptr<TowerSample> > &getTowers();

    [[nodiscard]] std::vector<std::shared_ptr<EnemySample> > &getEnemies();

private:
    std::string name;
    std::string path;
    std::time_t lastSaveDate;
    //для будущего
    // std::vector<CampaignTemplate*> campaigns;
    std::vector<std::shared_ptr<EnemySample> > enemies;
    std::vector<std::shared_ptr<TowerSample> > towers;
    // std::vector<AbilityTemplate*> abilities;
    // std::vector<Replay*> replays;
};


#endif //EDITOR_PROJECT_H
