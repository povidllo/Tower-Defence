#pragma once
#include "IActing.h"
#include "MapObject.h"

namespace TDEngine {
    namespace Inner {
        class Projectile : public MapObject, public IActing{
        public:
            Projectile(double moveSpeed, double damage);
        private:
            void move(uint64_t timePassed);
            void hit();

            double moveSpeed;
            double damage;
        };
    } // Inner
} // TDEngine