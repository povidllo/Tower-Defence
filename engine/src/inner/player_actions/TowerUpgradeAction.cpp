#include "TowerUpgradeAction.h"

namespace TDEngine {
    namespace Inner {
        TowerUpgradeAction::TowerUpgradeAction(TowerSample upgradeTo, std::shared_ptr<TowerActions> tower)
            : upgradeTo(upgradeTo), tower(tower) {}

        void TowerUpgradeAction::MakeAction() {
            tower->storage.setUpgradingTo = upgradeTo;
        }

    } // Inner
} // TDEngine