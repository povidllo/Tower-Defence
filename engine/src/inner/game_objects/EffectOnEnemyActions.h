#pragma once
#include <memory>
#include "EnemyActions.h"
#include "IActing.h"
#include "entities/EffectOnEnemy.h"

namespace TDEngine::Inner {
	class EngineStorage;
	class EnemyActions;

	class EffectOnEnemyActions : public IActing, public MapObject{
	public:
		EffectOnEnemyActions(EnemyEffectSample sample, std::shared_ptr<EnemyActions> target);
		void act(uint64_t timePassedMillis, std::shared_ptr<EngineStorage> engineStorage) override;
		void end(std::shared_ptr<EngineStorage> engineStorage);
		void applyEffects(const std::vector<std::string>& effectNames,
							  std::shared_ptr<EngineStorage> engineStorage);

		EffectOnEnemy storage;
	};
} // namespace TDEngine::Inner