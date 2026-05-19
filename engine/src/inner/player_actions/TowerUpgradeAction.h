#pragma once
#include <TowerSample.h>

#include "IPlayerAction.h"
#include "../game_objects/TowerActions.h"

namespace TDEngine {
    namespace Inner {
        class TowerUpgradeAction : public IPlayerAction {
        public:
            TowerUpgradeAction(std::string upgradeTo, std::shared_ptr<TowerActions> tower,
            	std::shared_ptr<EnginePlayer> player);
            void MakeAction() override;
            std::string upgradeTo;
            std::shared_ptr<TowerActions> tower;
        	std::shared_ptr<EnginePlayer> player;
        };
    } // Inner
} // TDEngine