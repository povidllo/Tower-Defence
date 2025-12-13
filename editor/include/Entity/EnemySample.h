#ifndef TOWERDEFENCE_ENEMYSAMPLE_H
#define TOWERDEFENCE_ENEMYSAMPLE_H

#include <string>
#include <utility>
#include "Serializable.h"

class EnemySample : protected ISerializable {
public:
	using json = nlohmann::json;

	explicit EnemySample(std::string name);

	explicit EnemySample(const json &j);

	json toJson() const override;

	void fromJson(const json &j) override;

	std::string getName() const;

	void setName(const std::string &name);

	double getHealth() const;

	void setHealth(double health);

	double getDamage() const;

	void setDamage(double damage);

	std::string getEnemyTexturePath() const;

	void setEnemyTexturePath(const std::string &enemyTexturePath);

	double getSpeed() const;

	void setSpeed(double speed);

private:
	std::string name;
	double health{0};
	double damage{0};
	double moneyFallsOut{0};
	double moneyFallsOutPercentage{0};
	double speed{0};
	std::string enemyTexturePath;
};

#endif //TOWERDEFENCE_ENEMYSAMPLE_H
