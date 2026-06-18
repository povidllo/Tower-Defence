#pragma once
#include "IActing.h"
#include "MapObject.h"
#include "entities/EffectCreator.h"
#include <memory>
#include <vector>

#include "EffectSample.h"
#include "EnemyActions.h"
#include "TowerActions.h"

namespace TDEngine::Inner {
	class EngineStorage;

	class EffectCreatorActions : public IActing, public MapObject {
	public:
		EffectCreatorActions(std::string sampleName, std::shared_ptr<EngineStorage> engineStorage,
        	std::vector<std::shared_ptr<EnginePlayer>> ownerPlayers,  std::shared_ptr<MapObject> attachedObject);
		std::shared_ptr<MapObject> findClosestTargetEnemy(std::shared_ptr<EngineStorage> engine_storage,
			std::shared_ptr<MapObject> attachedObject);
		void act(uint64_t timePassedMillis, std::shared_ptr<EngineStorage> engineStorage) override;
		void applyEffects(const std::vector<std::string>& effectNames,
						  std::shared_ptr<EngineStorage> engineStorage);
		std::shared_ptr<EffectCreatorSample> findSample(std::string sampleName, std::shared_ptr<EngineStorage> engineStorage);
		std::vector<std::shared_ptr<EnemyActions>> getTargetEnemiesInRadius(std::shared_ptr<EngineStorage> engineStorage);
		std::vector<std::shared_ptr<TowerActions>> getTargetTowersInRadius(std::shared_ptr<EngineStorage> engineStorage);
		EffectCreator storage;
	};
} // namespace TDEngine::Inner