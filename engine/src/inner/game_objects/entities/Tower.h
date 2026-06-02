#pragma once
#include "../../../../../editor/include/Entity/TowerSample.h"
#include "../samples//EffectCreatorSample.h"
namespace TDEngine {
    namespace Inner {
        class Tower : public TowerSample{
        public:
        	explicit Tower(TowerSample sample)
        	: TowerSample(std::move(sample)) {}
        	explicit Tower(std::vector<std::string> upgrades)
			: TowerSample(std::string("")) {
        		for (auto &upgrade : upgrades) {
        			addNextUpgrade(upgrade);
        		}
        	}
            std::optional<std::string> setUpgradingTo;
        	std::shared_ptr<EnginePlayer> setUpgradingByPlayer;
            uint64_t timeAfterLastShot;
        	std::vector<EffectCreatorSample> effectCreatorsOnHit;
        	std::vector<std::shared_ptr<EnginePlayer>> ownerPlayers;
        };
    } // Inner
} // TDEngine