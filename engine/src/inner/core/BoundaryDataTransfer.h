#pragma once

#include <memory>
#include <optional>
#include <mutex>
#include <queue>

#include "FrameData.h"
#include "../player_actions/IPlayerAction.h"
#include "../../../../editor/include/Entity/Project.h"

namespace TDEngine {
    namespace Inner {
        class Engine;
        class BoundaryDataTransfer {
        public:
            void addPlayerAction(std::shared_ptr<IPlayerAction> &action);
            std::optional<std::shared_ptr<IPlayerAction>> extractPlayerActionForEngine();
            std::queue<std::shared_ptr<IPlayerAction>> playerActionsForEngine;
        	std::queue<std::shared_ptr<IPlayerAction>> playerActionsForRenderer;
        };
    } // Inner
} // TDEngine