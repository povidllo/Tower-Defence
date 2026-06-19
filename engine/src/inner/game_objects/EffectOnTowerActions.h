#pragma once
#include "IActing.h"
#include <memory>

#include "entities/EffectOnTower.h"

namespace TDEngine::Inner {
	class EngineStorage;
	class TowerActions;

	class EffectOnTowerActions : public IActing, public MapObject {
	public:
		EffectOnTowerActions(TowerEffectSample sample,
        	std::vector<std::shared_ptr<EnginePlayer>> ownerPlayers, std::shared_ptr<TowerActions> target);
		void act(uint64_t timePassedMillis, std::shared_ptr<EngineStorage> engineStorage) override;
		void end(std::shared_ptr<EngineStorage> engineStorage);
		void applyEffects(const std::vector<std::string>& effectNames,
							  std::shared_ptr<EngineStorage> engineStorage);

		EffectOnTower storage;
	};
} // namespace TDEngine::Inner