#pragma once
#include <complex.h>

#include "IActing.h"
#include "MapObject.h"

namespace TDEngine {
    namespace Inner {
        class TowerActions : public MapObject, public IActing {
        };
    } // Inner
} // TDEngine