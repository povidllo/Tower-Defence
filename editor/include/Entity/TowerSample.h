#ifndef TOWERDEFENCE_TOWERSAMPLE_H
#define TOWERDEFENCE_TOWERSAMPLE_H

#include <string>
#include <utility>
#include <vector>
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

	std::vector<std::string> getBaseEffectCreatorNames() const;

	void addBaseEffectCreator(const std::string &name);

	bool removeBaseEffectCreator(const std::string &name);

	std::vector<std::string> getAttackEffectCreatorNames() const;

	void addAttackEffectCreator(const std::string &name);

	bool removeAttackEffectCreator(const std::string &name);

	void renameEffectCreatorReference(const std::string &oldName, const std::string &newName);

	void removeEffectCreatorReference(const std::string &name);

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

	/** Non-empty: map spot stores only this template name in JSON; stats come from project tower. */
	[[nodiscard]] const std::string &getTowerTemplateName() const { return towerTemplateName; }

	void setTowerTemplateName(std::string templateName) { towerTemplateName = std::move(templateName); }

	[[nodiscard]] bool isMapSpotReference() const { return !towerTemplateName.empty(); }

	/** Copies combat/visual fields from template; keeps instance name, position, towerTemplateName. */
	void applyTemplate(const TowerSample &src);

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
	std::vector<std::string> baseEffectCreators;
	std::vector<std::string> attackEffectCreators;
	std::string towerTemplateName;
};

#endif //TOWERDEFENCE_TOWERSAMPLE_H
