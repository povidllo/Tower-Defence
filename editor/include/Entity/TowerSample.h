#ifndef TOWERDEFENCE_TOWERSAMPLE_H
#define TOWERDEFENCE_TOWERSAMPLE_H

#include <string>
#include <utility>
#include "Serializable.h"


class TowerSample : protected ISerializable {
public:
    using json = nlohmann::json;

    explicit TowerSample(std::string name)
        : name(std::move(name)) {
    }

    explicit TowerSample(const json &j) {
        TowerSample::fromJson(j);
    }

    json toJson() const override {
        return {
            {"name", name},
            {"damage", damage},
            {"fireRate", fireRate},
            {"towerTexturePath", towerTexturePath},
            // {"projectileTexturePath", projectileTexturePath},
            {"x", x},
            {"y", y}
        };
    }

    void fromJson(const json &j) override {
        name = j.value("name", name);
        damage = j.value("damage", damage);
        fireRate = j.value("fireRate", fireRate);
        towerTexturePath = j.value("towerTexturePath", "");
        // projectileTexturePath = j.value("projectileTexturePath", "");
        x = j.value("x", x);
        y = j.value("y", y);
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

    void setTowerTexturePath(const std::string &texPath) {
        towerTexturePath = texPath;
    }

    std::string getTowerTexturePath() const {
        return towerTexturePath;
    }

private:
    std::string name;
    double damage{0};
    double fireRate{0};
    // std::vector<std::shared_ptr<TowerSample> > nextUpgrade;
    std::string towerTexturePath;
    int x{0};
    int y{0};
};


#endif //TOWERDEFENCE_TOWERSAMPLE_H
