#pragma once
#include <cstdint>

namespace TDEngine {
    namespace Inner {
        class IActing {
        public:
        virtual void act(uint64_t timePassed) = 0;
        //     virtual void createNew(EngineStorage storage) = 0;
        virtual ~IActing() = default;
        };
    } // Inner
} // TDEngine