#ifndef TOWERDEFENCE_ABILITYSAMPLE_H
#define TOWERDEFENCE_ABILITYSAMPLE_H

#include <string>
#include <vector>

#include "Serializable.h"

class AbilitySample : protected ISerializable {
public:
	using json = nlohmann::ordered_json;

	explicit AbilitySample(std::string name);

	explicit AbilitySample(const json &j);

	json toJson() const override;

	void fromJson(const json &j) override;

	[[nodiscard]] const std::string &getName() const;

	void setName(std::string name);

	[[nodiscard]] const std::vector<std::string> &getEffectCreatorsOnCast() const;

	void setEffectCreatorsOnCast(std::vector<std::string> effectCreators);

	void addEffectCreatorOnCast(const std::string &name);

	bool removeEffectCreatorOnCast(const std::string &name);

	void renameEffectCreatorReference(const std::string &oldName, const std::string &newName);

	void removeEffectCreatorReference(const std::string &name);

	[[nodiscard]] const std::string &getTargetSelection() const;

	void setTargetSelection(std::string value);

	[[nodiscard]] int getChargesCount() const;

	void setChargesCount(int value);

	[[nodiscard]] double getChargeCooldownSeconds() const;

	void setChargeCooldownSeconds(double value);

	[[nodiscard]] double getFullCooldownSeconds() const;

	void setFullCooldownSeconds(double value);

private:
	std::string name;
	std::vector<std::string> effectCreatorsOnCast;
	std::string targetSelection{"none"};
	int chargesCount{1};
	double chargeCooldownSeconds{0};
	double fullCooldownSeconds{0};
};

#endif // TOWERDEFENCE_ABILITYSAMPLE_H
