#pragma once
#include "../../../../../editor/include/Entity/TowerSample.h"
namespace TDEngine {
    namespace Inner {
        class Tower : public TowerSample{
        public:
            std::optional<TowerSample> setUpgradingTo;
        };
    } // Inner
} // TDEngine