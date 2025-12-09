#pragma once
#include <utility>

namespace TDEngine {
    namespace Inner {
        class MapObject {
            public:
            explicit MapObject(double x, double y) {
                positionCoordinates = {x, y};
            }
            std::pair<double, double> positionCoordinates;
        };
    } // Inner
} // TDEngine