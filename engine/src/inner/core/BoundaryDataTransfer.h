#pragma once

#include <memory>
#include <optional>
#include <mutex>
#include <queue>

#include "FrameData.h"
#include "../player_actions/IPlayerAction.h"
#include "../Zaglushki/ProjectSample.h"

namespace TDEngine {
    namespace Inner {
        class Engine;
        class BoundaryDataTransfer {
        public:
            explicit BoundaryDataTransfer(Engine* engine, std::shared_ptr<ProjectSample> &projectSample);
            std::optional<FrameData> getCurFrame();
            void setNextFrame(FrameData &newNextFrame);
            void addPlayerAction(std::shared_ptr<IPlayerAction> &action);
            std::optional<std::shared_ptr<IPlayerAction>> extractPlayerAction();
        private:
            Engine* engine;
            std::shared_ptr<ProjectSample> currentProject;
            std::optional<FrameData> curFrame;
            std::optional<FrameData> nextFrame;
            std::mutex curFrameMutex;
            std::mutex nextFrameMutex;
            std::queue<std::shared_ptr<IPlayerAction>> playerActions;
            std::mutex playerActionsMutex;
        };
    } // Inner
} // TDEngine