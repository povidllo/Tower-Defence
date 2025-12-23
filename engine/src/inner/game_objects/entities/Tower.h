#pragma once
#include "../../../../../editor/include/Entity/TowerSample.h"
namespace TDEngine {
    namespace Inner {
        class Tower : public TowerSample{
        public:
            explicit Tower(TowerSample sample)
            : TowerSample(std::move(sample)) {}
            std::optional<std::string> setUpgradingTo;
            uint64_t timeAfterLastShot;
        };
    } // Inner
} // TDEngine