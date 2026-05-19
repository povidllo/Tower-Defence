#include "TowerUpgradeAction.h"

namespace TDEngine {
    namespace Inner {
        TowerUpgradeAction::TowerUpgradeAction(std::string upgradeTo, std::shared_ptr<TowerActions> tower,
        	std::shared_ptr<EnginePlayer> player)
            : upgradeTo(upgradeTo), tower(tower), player(player) {}

        void TowerUpgradeAction::MakeAction() {
            tower->storage.setUpgradingTo = upgradeTo;
        }

    } // Inner
} // TDEngine