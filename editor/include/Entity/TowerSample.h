#ifndef TOWERDEFENCE_TOWERSAMPLE_H
#define TOWERDEFENCE_TOWERSAMPLE_H
#include <string>
#include <vector>
#include "Serializable.h"


class TowerSample : protected ISerializable {
public:
    using json = nlohmann::json;

    TowerSample(const std::string &name)
        : name(name),
          damage(0),
          fireRate(0) {
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
        damage = j.value("damage", 0.0);
        fireRate = j.value("fireRate", 1.0);
        towerTexturePath = j.value("towerTexturePath", "");
        // projectileTexturePath = j.value("projectileTexturePath", "");
        x = j.value("x", 0.0);
        y = j.value("y", 0.0);
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
    double damage;
    double fireRate;
    // std::vector<std::shared_ptr<TowerSample> > nextUpgrade;
    std::string towerTexturePath;
    int x;
    int y;
};


#endif //TOWERDEFENCE_TOWERSAMPLE_H
