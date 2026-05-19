#ifndef TOWERDEFENCE_ENEMYSAMPLE_H
#define TOWERDEFENCE_ENEMYSAMPLE_H

#include <string>
#include <utility>
#include <vector>
#include "Serializable.h"

class EnemySample : protected ISerializable {
public:
	using json = nlohmann::ordered_json;

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

	double getMoneyFallsOut() const;

	void setMoneyFallsOut(double moneyFallsOut);

	double getMoneyFallsOutPercentage();

	void setMoneyFallsOutPercentage(double moneyFallsOutPercentage);

	std::vector<std::string> getBaseEffectCreatorNames() const;

	void addBaseEffectCreator(const std::string &name);

	bool removeBaseEffectCreator(const std::string &name);

	std::vector<std::string> getDamageTakenEffectCreatorNames() const;

	void addDamageTakenEffectCreator(const std::string &name);

	bool removeDamageTakenEffectCreator(const std::string &name);

	std::vector<std::string> getDamageDealtEffectCreatorNames() const;

	void addDamageDealtEffectCreator(const std::string &name);

	bool removeDamageDealtEffectCreator(const std::string &name);

	void renameEffectCreatorReference(const std::string &oldName, const std::string &newName);

	void removeEffectCreatorReference(const std::string &name);

private:
	std::string name;
	double health{0};
	double damage{0};
	double moneyFallsOut{0};
	double moneyFallsOutPercentage{0};
	double speed{0};
	std::string enemyTexturePath;
	std::vector<std::string> baseEffectCreators;
	std::vector<std::string> damageTakenEffectCreators;
	std::vector<std::string> damageDealtEffectCreators;
};

#endif //TOWERDEFENCE_ENEMYSAMPLE_H
