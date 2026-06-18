#pragma once
#include "IPlayerAction.h"
#include <memory>
#include "../game_objects/TowerActions.h"

namespace TDEngine::Inner {
	class TowerBehaviourChangeAction : public IPlayerAction {
	public:
		TowerBehaviourChangeAction(std::shared_ptr<TowerActions> tower, TowerBehaviourTypes newBehaviour);
		void MakeAction() override;
	private:
		std::shared_ptr<TowerActions> tower;
		TowerBehaviourTypes newBehaviour;
	};
}