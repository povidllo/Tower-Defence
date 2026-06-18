#pragma once
#include <cstdint>
#include <memory>

#include "../../../../../editor/include/Entity/AbilitySample.h"

namespace TDEngine {
	namespace Inner {
		class EnginePlayer;
		class Ability : public AbilitySample{
		public:
			explicit Ability(AbilitySample sample)
			: AbilitySample(std::move(sample)) {}
			int currentCharges;
            uint64_t timeAfterSingleRecharge;
            uint64_t timeAfterLastFullCharge;
			std::shared_ptr<MapObject> target;
			std::vector<std::shared_ptr<EnginePlayer>> onwerPlayers;
		};
	} // Inner
} // TDEngine