#ifndef TOWERDEFENCE_TOWERSAMPLE_H
#define TOWERDEFENCE_TOWERSAMPLE_H
#include <string>
#include <vector>
#include "Serializable.h"


class TowerSample : protected ISerializable {
public:
    using json = nlohmann::json;

    TowerSample(const std::string &name, double damage = 0, double fire_rate = 0)
        : name(name),
          damage(damage),
          fireRate(fire_rate) {
    }

    explicit TowerSample(const json &j) {
        TowerSample::fromJson(j);
    }

    json toJson() const override {
        return {
            {"name", name},
            {"damage", damage},
            {"fireRate", fireRate},
        };
    }

    void fromJson(const json &j) override {
        name = j.at("name").get<std::string>();
        damage = j.at("damage").get<double>();
        fireRate = j.at("fireRate").get<double>();
    }

    [[nodiscard]] std::string getName() {
        return name;
    }

    void setName(const std::string &n) {
        name = n;
    }

    [[nodiscard]] double getDamage() const {
        return damage;
    }

    void setDamage(double d) {
        damage = d;
    }

    [[nodiscard]] double getFireRate() const {
        return fireRate;
    }

    void setFireRate(double r) {
        fireRate = r;
    }

private:
    std::string name;
    double damage;
    double fireRate;
    std::vector<std::shared_ptr<TowerSample> > nextUpgrade;
};


#endif //TOWERDEFENCE_TOWERSAMPLE_H
