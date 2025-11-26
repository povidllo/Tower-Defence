#ifndef TOWERDEFENCE_ENEMYSAMPLE_H
#define TOWERDEFENCE_ENEMYSAMPLE_H

#include <string>
#include <utility>
#include "Serializable.h"

class EnemySample : protected ISerializable {
public:
    using json = nlohmann::json;

    explicit EnemySample(std::string name)
        : name(std::move(name)) {
    }

    explicit EnemySample(const json &j) {
        EnemySample::fromJson(j);
    }

    json toJson() const override {
        return {
            {"name", name},
            {"health", health},
            {"damage", damage},
            {"enemyTexturePath", enemyTexturePath},
        };
    }

    void fromJson(const json &j) override {
        name = j.value("name", name);
        health = j.value("health", health);
        damage = j.value("damage", damage);
        enemyTexturePath = j.value("enemyTexturePath", "");
    }


    std::string getName() const {
        return name;
    }

    void setName(const std::string &name) {
        this->name = name;
    }

    double getHealth() const {
        return health;
    }

    void setHealth(double health) {
        this->health = health;
    }

    double getDamage() const {
        return damage;
    }

    void setDamage(double damage) {
        this->damage = damage;
    }

    std::string getEnemyTexturePath() const {
        return enemyTexturePath;
    }

    void setEnemyTexturePath(const std::string &enemyTexturePath) {
        this->enemyTexturePath = enemyTexturePath;
    }

private:
    std::string name;
    double health{0};
    double damage{0};
    std::string enemyTexturePath;
};


#endif //TOWERDEFENCE_ENEMYSAMPLE_H
