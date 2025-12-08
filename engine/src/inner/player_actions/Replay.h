#pragma once
#include <cstdint>
#include <map>
#include <memory>

#include "IPlayerAction.h"

namespace TDEngine {
    namespace Inner {
        class Replay {
            public:
            std::map<uint64_t, std::shared_ptr<IPlayerAction>> actionsSequenceByTime;
            std::string playedMapName;
            std::string replayName;
            uint64_t playingTime;
        };
    } // Inner
} // TDEngine