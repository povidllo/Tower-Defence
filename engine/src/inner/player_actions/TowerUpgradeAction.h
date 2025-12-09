#pragma once
#include <TowerSample.h>

#include "IPlayerAction.h"
#include "../game_objects/TowerActions.h"

namespace TDEngine {
    namespace Inner {
        class TowerUpgradeAction : public IPlayerAction {
        public:
            TowerUpgradeAction(TowerSample upgradeTo, std::shared_ptr<TowerActions> tower);
            void MakeAction() override;
            TowerSample upgradeTo;
            std::shared_ptr<TowerActions> tower;
        };
    } // Inner
} // TDEngine