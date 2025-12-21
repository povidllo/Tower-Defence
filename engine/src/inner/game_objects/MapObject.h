#pragma once
#include <iostream>
#include <math.h>
#include <string>
#include <utility>
#include <memory>

namespace TDEngine {
    namespace Inner {
    	enum class MapObjectTypes {
    		Tower,
    		Enemy,
    		Projectile
    	};
        class MapObject {
        public:
        	MapObject(std::string texturePath, double x, double y, MapObjectTypes type) {
                positionCoordinates = {x, y};
            	this->texturePath = std::move(texturePath);
        		this->type = type;
            }
        	MapObject(std::string texturePath, std::pair<double, double> position, MapObjectTypes type) {
                positionCoordinates = position;
            	this->texturePath = std::move(texturePath);
        		this->type = type;
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
        		if (distLeft < 1e-8) {
        			return;
        		}
                if (distLeft < distTraveled) {
                    distTraveled = distLeft;
                }

                double movedX = dx * distTraveled / distLeft;
                double movedY = dy * distTraveled / distLeft;
                positionCoordinates.first += movedX;
                positionCoordinates.second += movedY;
        		std::cout << "object moved to " << positionCoordinates.first << " " << positionCoordinates.second <<
        			"and was moving to " << target.first << " " << target.second << std::endl;
            }
        	std::string texturePath;
			MapObjectTypes type;
            std::pair<double, double> positionCoordinates;
        };
    } // Inner
} // TDEngine