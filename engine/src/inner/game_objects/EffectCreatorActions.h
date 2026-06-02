#pragma once
#include "IActing.h"
#include "MapObject.h"
#include "entities/EffectCreator.h"
#include <memory>
#include <vector>

#include "EnemyActions.h"

namespace TDEngine::Inner {
	class EngineStorage;

	class EffectCreatorActions : public IActing, public MapObject {
	public:
		EffectCreatorActions(EffectCreatorSample sample, std::shared_ptr<MapObject> attachedObject);
		void act(uint64_t timePassedMillis, std::shared_ptr<EngineStorage> engineStorage) override;
		void applyEffects(const std::vector<EffectOnEnemySample>& enemyEffects,
						  std::shared_ptr<EngineStorage> engineStorage);
		std::vector<std::shared_ptr<EnemyActions>> getTargetEnemiesInRadius(std::shared_ptr<EngineStorage> engineStorage);
		EffectCreator storage;
	};
} // namespace TDEngine::Inner