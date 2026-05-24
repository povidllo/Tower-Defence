#ifndef TOWERDEFENCE_ENEMYEFFECTSAMPLE_H
#define TOWERDEFENCE_ENEMYEFFECTSAMPLE_H

#include "EffectSample.h"

class EnemyEffectSample : public EffectSample {
public:
	explicit EnemyEffectSample(std::string name);

	explicit EnemyEffectSample(const json &j);

	json toJson() const override;

	void fromJson(const json &j) override;

	[[nodiscard]] int getStartHealthImpact() const;

	void setStartHealthImpact(int value);

	[[nodiscard]] int getStartSpeedImpactPercent() const;

	void setStartSpeedImpactPercent(int value);

	[[nodiscard]] int getPeriodicHealthImpact() const;

	void setPeriodicHealthImpact(int value);

	[[nodiscard]] int getPeriodicSpeedImpactPercent() const;

	void setPeriodicSpeedImpactPercent(int value);

private:
	int startHealthImpact{0};
	int startSpeedImpactPercent{0};
	int periodicHealthImpact{0};
	int periodicSpeedImpactPercent{0};
};

#endif // TOWERDEFENCE_ENEMYEFFECTSAMPLE_H
