#ifndef TOWERDEFENCE_TOWEREFFECTSAMPLE_H
#define TOWERDEFENCE_TOWEREFFECTSAMPLE_H

#include "EffectSample.h"

class TowerEffectSample : public EffectSample {
public:
	explicit TowerEffectSample(std::string name);

	explicit TowerEffectSample(const json &j);

	json toJson() const override;

	void fromJson(const json &j) override;

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

private:
	int startDamageFlatImpact{0};
	int startDamagePercentImpact{0};
	int startAttackSpeedPercentImpact{0};
	int periodicDamageFlatImpact{0};
	int periodicDamagePercentImpact{0};
	int periodicAttackSpeedPercentImpact{0};
};

#endif // TOWERDEFENCE_TOWEREFFECTSAMPLE_H
