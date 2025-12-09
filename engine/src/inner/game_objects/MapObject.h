#pragma once
#include <math.h>
#include <utility>
#include <bits/valarray_after.h>

namespace TDEngine {
    namespace Inner {
        class MapObject {
        public:
            explicit MapObject(double x, double y) {
                positionCoordinates = {x, y};
            }
            explicit MapObject(std::pair<double, double> position) {
                positionCoordinates = position;
            }
            double getDistanceTo(std::shared_ptr<MapObject> const& other) const {
                double dx = positionCoordinates.first - other->positionCoordinates.first;
                double dy = positionCoordinates.second - other->positionCoordinates.second;
                return sqrt(dx*dx + dy*dy);
            }
            double getDistanceTo(std::pair<double, double> point) const {
                double dx = positionCoordinates.first - point.first;
                double dy = positionCoordinates.second - point.second;
                return sqrt(dx*dx + dy*dy);
            }
            void moveTo(std::pair<double, double> target, double speed, uint64_t timeMillis) {
                double distTraveled = speed * timeMillis / 1000.0;
                double dx = target.first - positionCoordinates.first;
                double dy = target.second - positionCoordinates.second;
                double distLeft = sqrt(dx*dx + dy*dy);
                if (distLeft < distTraveled) {
                    distTraveled = distLeft;
                }

                double movedX = dx * distTraveled / distLeft;
                double movedY = dy * distTraveled / distLeft;
                positionCoordinates.first += movedX;
                positionCoordinates.second += movedY;
            }
            std::pair<double, double> positionCoordinates;
        };
    } // Inner
} // TDEngine