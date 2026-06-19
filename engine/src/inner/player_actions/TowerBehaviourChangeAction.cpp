#include "TowerBehaviourChangeAction.h"

namespace TDEngine::Inner {
	TowerBehaviourChangeAction::TowerBehaviourChangeAction(std::shared_ptr<TowerActions> tower, TowerBehaviourTypes newBehaviour)
		: tower(tower), newBehaviour(newBehaviour) {}

	void TowerBehaviourChangeAction::MakeAction() {
		if (tower) {
			tower->storage.behaviourType = newBehaviour;
		}
	}
}