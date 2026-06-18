#pragma once
#include "IPlayerAction.h"
#include <memory>
#include "../game_objects/entities/EnginePlayer.h"

namespace TDEngine {
	namespace Inner {
		class AbilityUseAction : public IPlayerAction {
		public:
			AbilityUseAction(std::shared_ptr<EnginePlayer> player, int abilityIndex, std::shared_ptr<MapObject> target);
			void MakeAction() override;
		private:
			std::shared_ptr<EnginePlayer> player;
			int abilityIndex;
			std::shared_ptr<MapObject> target;
		};
	}
}