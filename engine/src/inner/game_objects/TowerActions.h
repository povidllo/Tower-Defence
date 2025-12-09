#pragma once
#include <complex.h>

#include "IActing.h"
#include "MapObject.h"
#include "entities/Tower.h"


namespace TDEngine {
    namespace Inner {
        class TowerActions : public MapObject, public IActing {
        public:
            explicit TowerActions(TowerSample sample);
            void attack();
            void upgradeTower();

            Tower storage;
        };
    } // Inner
} // TDEngine