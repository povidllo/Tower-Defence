#ifndef TOWERDEFENCE_EFFECTSAMPLE_H
#define TOWERDEFENCE_EFFECTSAMPLE_H

#include <string>
#include <utility>
#include <vector>

#include "Serializable.h"

class EffectSample : protected ISerializable {
public:
	using json = nlohmann::ordered_json;

	enum class TargetType {
		Unit,
		Tower
	};

	explicit EffectSample(std::string name);

	explicit EffectSample(const json &j);

	json toJson() const override;

	void fromJson(const json &j) override;

	[[nodiscard]] const std::string &getName() const;

	void setName(std::string name);

	[[nodiscard]] TargetType getTargetType() const;

	void setTargetType(TargetType targetType);

	[[nodiscard]] std::string getTargetTypeString() const;

	void setTargetTypeString(const std::string &targetType);

	[[nodiscard]] int getStartHealthImpact() const;

	void setStartHealthImpact(int value);

	[[nodiscard]] int getStartSpeedImpactPercent() const;

	void setStartSpeedImpactPercent(int value);

	[[nodiscard]] int getPeriodicHealthImpact() const;

	void setPeriodicHealthImpact(int value);

	[[nodiscard]] int getPeriodicSpeedImpactPercent() const;

	void setPeriodicSpeedImpactPercent(int value);

	[[nodiscard]] int getStartDamageFlatImpact() const;

	void setStartDamageFlatImpact(int value);

	[[nodiscard]] int getStartDamagePercentImpact() const;

	void setStartDamagePercentImpact(int value);

	[[nodiscard]] int getStartAttackSpeedPercentImpact() const;

	void setStartAttackSpeedPercentImpact(int value);

	[[nodiscard]] int getPeriodicDamageFlatImpact() const;

	void setPeriodicDamageFlatImpact(int value);

	[[nodiscard]] int getPeriodicDamagePercentImpact() const;

	void setPeriodicDamagePercentImpact(int value);

	[[nodiscard]] int getPeriodicAttackSpeedPercentImpact() const;

	void setPeriodicAttackSpeedPercentImpact(int value);

	[[nodiscard]] double getDurationSeconds() const;

	void setDurationSeconds(double value);

	[[nodiscard]] double getPeriodSeconds() const;

	void setPeriodSeconds(double value);

	[[nodiscard]] bool isStackable() const;

	void setStackable(bool value);

	[[nodiscard]] const std::vector<std::string> &getEffectsAfterFinish() const;

	void setEffectsAfterFinish(std::vector<std::string> effects);

	void addEffectAfterFinish(const std::string &name);

	bool removeEffectAfterFinish(const std::string &name);

	[[nodiscard]] const std::string &getVisualTexturePath() const;

	void setVisualTexturePath(std::string path);

private:
	std::string name;
	TargetType targetType{TargetType::Unit};

	int startHealthImpact{0};
	int startSpeedImpactPercent{0};
	int periodicHealthImpact{0};
	int periodicSpeedImpactPercent{0};

	int startDamageFlatImpact{0};
	int startDamagePercentImpact{0};
	int startAttackSpeedPercentImpact{0};
	int periodicDamageFlatImpact{0};
	int periodicDamagePercentImpact{0};
	int periodicAttackSpeedPercentImpact{0};

	double durationSeconds{0};
	double periodSeconds{0};
	bool stackable{false};
	std::vector<std::string> effectsAfterFinish;
	std::string visualTexturePath;
};

#endif // TOWERDEFENCE_EFFECTSAMPLE_H
