#include "EnemyActions.h"

namespace TDEngine {
    namespace Inner {
        EnemyActions::EnemyActions(EnemySample sample, std::shared_ptr<Wave> wave)
            : MapObject(wave->getPath()[0].first, wave->getPath()[0].second), storage(std::move(sample))
        {

            storage.associatedWave = std::move(wave);
            storage.currentHP = storage.getHealth();
            storage.targetIndex = 0;
            storage.curSpeed = 1;
        }

        void EnemyActions::act(uint64_t timePassedMillis) {
            if (storage.currentHP <= 0) {
                die();
            }

            if (abs(positionCoordinates.first - storage.associatedWave->getPath()[storage.targetIndex].first)
                < 1e-5
                && abs(positionCoordinates.second - storage.associatedWave->getPath()[storage.targetIndex].second)
                < 1e-5) {
                if (storage.targetIndex + 1 == storage.associatedWave->getPath().size()) {
                    attack();
                }
                else {
                    storage.targetIndex++;
                }
            }

            move(timePassedMillis);
        }

        void EnemyActions::move(uint64_t timePassedMillis) {
            double distTraveled = storage.curSpeed * timePassedMillis / 1000.0;
            double dx = storage.associatedWave->getPath()[storage.targetIndex].first - positionCoordinates.first;
            double dy = storage.associatedWave->getPath()[storage.targetIndex].second - positionCoordinates.second;
            double distLeft = sqrt(dx*dx + dy*dy);
            if (distLeft < distTraveled) {
                distTraveled = distLeft;
            }

            double movedX = dx * distTraveled / distLeft;
            double movedY = dy * distTraveled / distLeft;
            positionCoordinates.first += movedX;
            positionCoordinates.second += movedY;
        }

        void EnemyActions::die() {

        }

        void EnemyActions::attack() {

        }


    } // Inner
} // TDEngine