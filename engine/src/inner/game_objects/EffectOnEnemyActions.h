#pragma once
#include "IActing.h"
#include "entities/EffectOnEnemy.h"
#include <memory>
#include "EnemyActions.h"

namespace TDEngine::Inner {
	class EngineStorage;
	class EnemyActions;

	class EffectOnEnemyActions : public IActing {
	public:
		EffectOnEnemyActions(EffectOnEnemySample sample, std::shared_ptr<EnemyActions> target);
		void act(uint64_t timePassedMillis, std::shared_ptr<EngineStorage> engineStorage) override;
		void end();

		EffectOnEnemy storage;
		std::shared_ptr<EnemyActions> target;
	};
} // namespace TDEngine::Inner