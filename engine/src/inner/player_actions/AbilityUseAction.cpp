#include "AbilityUseAction.h"
#include "../game_objects/AbilityActions.h"

namespace TDEngine {
	namespace Inner {
		AbilityUseAction::AbilityUseAction(std::shared_ptr<EnginePlayer> player, int abilityIndex, std::shared_ptr<MapObject> target)
			: player(player), abilityIndex(abilityIndex), target(target) {}

		void AbilityUseAction::MakeAction() {
			if (!player || abilityIndex < 0 || abilityIndex >= static_cast<int>(player->abilities.size())) {
				return;
			}
			auto ability = player->abilities[abilityIndex];
			if (ability->storage.currentCharges <= 0) {
				return;
			}
			ability->storage.target = target;
		}
	}
}