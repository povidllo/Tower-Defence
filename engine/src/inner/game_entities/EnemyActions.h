#pragma once
#include "IActing.h"
#include "MapObject.h"

namespace TDEngine {
    namespace Inner {
        class EnemyActions : public MapObject, public  IActing {
        };
    } // Inner
} // TDEngine