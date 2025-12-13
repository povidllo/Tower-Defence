#include "BoundaryDataTransfer.h"

namespace TDEngine {
    namespace Inner {

        void BoundaryDataTransfer::addPlayerAction(std::shared_ptr<IPlayerAction> &action) {
        }

        std::optional<std::shared_ptr<IPlayerAction>> BoundaryDataTransfer::extractPlayerActionForEngine() {
            if (!playerActionsForEngine.empty()) {
                std::shared_ptr<IPlayerAction> returnAction = playerActionsForEngine.front();
                playerActionsForEngine.pop();
                return returnAction;
            }
            return std::nullopt;
        }
    } // Inner
} // TDEngine