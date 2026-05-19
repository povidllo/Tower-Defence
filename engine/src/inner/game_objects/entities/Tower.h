#pragma once
#include "../../../../../editor/include/Entity/TowerSample.h"
#include "../samples//EffectCreatorSample.h"
namespace TDEngine {
    namespace Inner {
        class Tower : public TowerSample{
        public:
            explicit Tower(TowerSample sample)
            : TowerSample(std::move(sample)) {}
            std::optional<std::string> setUpgradingTo;
        	std::optional<std::shared_ptr<EnginePlayer>> setUpgradingByPlayer;
            uint64_t timeAfterLastShot;
        	std::vector<EffectCreatorSample> effectCreatorsOnHit;
        	std::vector<std::shared_ptr<EnginePlayer>> ownerPlayers;
        };
    } // Inner
} // TDEngine