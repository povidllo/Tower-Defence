#include "Projectile.h"

namespace TDEngine {
    namespace Inner {
        Projectile::Projectile(double moveSpeed, double damage,
            std::shared_ptr<EnemyActions> target, std::pair<double, double> positionCoordinates,
            std::string texturePath)
                : MapObject(texturePath, positionCoordinates, MapObjectTypes::Projectile),
                    moveSpeed(moveSpeed), damage(damage), target(target) {}

        void Projectile::act(uint64_t timePassedMillis, std::shared_ptr<EngineStorage> engineStorage) {
            moveTo(target->positionCoordinates, moveSpeed, timePassedMillis);
            if (getDistanceTo(target) < 1e-5) {
                hit();
            }
        }

        void Projectile::hit() {
            target->storage.currentHP -= damage;
        }
    } // Inner
} // TDEngine