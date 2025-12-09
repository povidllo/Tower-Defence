#pragma once
#include <cstdint>
#include <memory>

namespace TDEngine {
    namespace Inner {
        class EngineStorage;
        class IActing {
        public:
        virtual void act(uint64_t timePassedMillis, std::shared_ptr<EngineStorage> engineStorage) = 0;
        //     virtual void createNew(EngineStorage storage) = 0;
        virtual ~IActing() = default;
        };
    } // Inner
} // TDEngine