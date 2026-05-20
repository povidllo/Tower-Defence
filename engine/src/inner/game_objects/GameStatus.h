#pragma once
#include <cstdint>
#include "MapObject.h"
#include "entities/EngineTeam.h"

namespace TDEngine {
	namespace Inner {
		class GameStatus {
		public:
			GameStatus() {}
			std::vector<std::shared_ptr<MapObject>> mapObjects;
			std::vector<std::shared_ptr<EngineTeam>> teams;
		};
	} // namespace Inner
} // namespace TDEngine

