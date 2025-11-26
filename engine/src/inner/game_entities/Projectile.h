#pragma once
#include "IActing.h"
#include "MapObject.h"

namespace TDEngine {
    namespace Inner {
        class Projectile : public MapObject, public IActing{
        };
    } // Inner
} // TDEngine