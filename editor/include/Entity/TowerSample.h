#ifndef TOWERDEFENCE_TOWERSAMPLE_H
#define TOWERDEFENCE_TOWERSAMPLE_H

#include <string>
#include <utility>
#include "Serializable.h"

class TowerSample : protected ISerializable {
public:
	using json = nlohmann::ordered_json;

	explicit TowerSample(std::string name);

	explicit TowerSample(std::string name, int x, int y);

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

	int getX() const;

	void setProjectileTexturePath(const std::string &path);;

	std::string getProjectileTexturePath() const;

	void setProjectileSpeed(const double speed);

	double getProjectileSpeed() const;

	int getY() const;;

	void setX(const int x);

	void setY(const int y);

	double getCost() const;

	void setCost(const double cost);

	double getFireDistance() const;

	void setFireDistance(const double fireDistance);

private:
	std::string name;
	double damage{0};
	double fireRate{0};
	double cost{0};
	double x{0};
	double y{0};
	double projectileSpeed{0};
	double fireDistance{0};
	std::string towerTexturePath;
	std::string projectileTexturePath;
	std::vector<std::string> nextUpgrade;
};

#endif //TOWERDEFENCE_TOWERSAMPLE_H
