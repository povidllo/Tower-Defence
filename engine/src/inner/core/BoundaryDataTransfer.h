#pragma once
#include <memory>
#include <optional>

#include "FrameData.h"

namespace TDEngine {
    namespace Inner {
        class Engine;
        class BoundaryDataTransfer {
        public:
            FrameData getCurFrame();
            void setNextFrame(FrameData nextFrame);
            // void addPlayerAction(IPlayerAction action);
            // std::optional<IPlayerAction> extractPlayerAction();
        private:
            Engine* mEngine;
            // Project currentProject;
            FrameData curFrame;
            FrameData nextFrame;
            // std::vector<IPlayerAction> playerActions;
        };
    } // Inner
} // TDEngine