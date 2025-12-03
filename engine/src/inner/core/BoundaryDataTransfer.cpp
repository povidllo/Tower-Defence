#include "BoundaryDataTransfer.h"

#include <utility>

namespace TDEngine {
    namespace Inner {
        BoundaryDataTransfer::BoundaryDataTransfer(Engine *engine, std::shared_ptr<ProjectSample> &projectSample)
            : engine(engine), currentProject(projectSample)
        {}
        std::optional<FrameData> BoundaryDataTransfer::getCurFrame() {
            std::lock_guard<std::mutex> lockCur(curFrameMutex);
            std::lock_guard<std::mutex> lockNext(nextFrameMutex);
            auto returnFrame = std::exchange(curFrame, nextFrame);
            nextFrame = std::nullopt;
            return returnFrame;
        }

        void BoundaryDataTransfer::setNextFrame(FrameData &newNextFrame) {
            std::lock_guard<std::mutex> lockNext(nextFrameMutex);
            nextFrame = std::move(newNextFrame);
        }

        void BoundaryDataTransfer::addPlayerAction(std::shared_ptr<IPlayerAction> &action) {
            std::lock_guard<std::mutex> lock(playerActionsMutex);
            playerActions.push(action);
        }

        std::optional<std::shared_ptr<IPlayerAction>> BoundaryDataTransfer::extractPlayerAction() {
            std::lock_guard<std::mutex> lock(playerActionsMutex);
            if (!playerActions.empty()) {
                std::shared_ptr<IPlayerAction> returnAction = playerActions.front();
                playerActions.pop();
                return returnAction;
            }
            return std::nullopt;
        }
    } // Inner
} // TDEngine