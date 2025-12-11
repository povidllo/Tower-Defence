#ifndef TOWERDEFENCE_TOWERSAMPLE_H
#define TOWERDEFENCE_TOWERSAMPLE_H

#include <string>
#include <utility>
#include "Serializable.h"

class TowerSample : protected ISerializable {
public:
	using json = nlohmann::json;

	explicit TowerSample(std::string name);

	explicit TowerSample(const json &j);

	json toJson() const override;

	void fromJson(const json &j) override;

	[[nodiscard]] std::string getName();

	void setName(const std::string &n);

	[[nodiscard]] double getDamage() const;

	void setDamage(double d);

	[[nodiscard]] double getFireRate() const;

	void setFireRate(double r);

	void setTowerTexturePath(const std::string &texPath);

	std::string getTowerTexturePath() const;

	std::vector<std::string> getUpgradeNames() const;

	void addNextUpgrade(const std::string &name);

	bool removeNextUpgrade(const std::string &name);

private:
	std::string name;
	double damage{0};
	double fireRate{0};
	std::vector<std::string> nextUpgrade;
	std::string towerTexturePath;
	int x{0};
	int y{0};
};

#endif //TOWERDEFENCE_TOWERSAMPLE_H
