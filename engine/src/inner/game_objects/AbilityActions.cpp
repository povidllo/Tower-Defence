#include "AbilityActions.h"
#include "../core/EngineStorage.h"

namespace TDEngine::Inner {

	AbilityActions::AbilityActions(AbilitySample sample)
		: storage(std::move(sample)) {
		storage.currentCharges = storage.getChargesCount();
		storage.timeAfterLastFullCharge = 0;
		storage.timeAfterSingleRecharge = 0;
		storage.target = nullptr;
	}

	void AbilityActions::act(uint64_t timePassedMillis, std::shared_ptr<EngineStorage> engineStorage) {
		if (storage.currentCharges < storage.getChargesCount()) {
			storage.timeAfterLastFullCharge += timePassedMillis;
			storage.timeAfterSingleRecharge += timePassedMillis;
            uint64_t chargeCooldown = ceil(storage.getChargeCooldownSeconds() * 1000);
            uint64_t fullCooldown = ceil(storage.getFullCooldownSeconds() * 1000);
			if (storage.timeAfterLastFullCharge >= fullCooldown) {
				storage.currentCharges = storage.getChargesCount();
				storage.timeAfterLastFullCharge = 0;
				storage.timeAfterSingleRecharge = 0;
			}
			else if (storage.timeAfterSingleRecharge >= chargeCooldown) {
				storage.currentCharges++;
				storage.timeAfterSingleRecharge = 0;
				if (storage.currentCharges == storage.getChargesCount()) {
					storage.timeAfterLastFullCharge = 0;
				}
			}
		}
		if (storage.target != nullptr && storage.currentCharges > 0) {
			cast(engineStorage, storage.target);
			storage.target = nullptr;
		}
	}
	void AbilityActions::cast(std::shared_ptr<EngineStorage> engineStorage, std::shared_ptr<MapObject> target) {
		for (std::string effectCreatorName : storage.getEffectCreatorsOnCast()) {
			auto newEffectCreator = std::make_shared<EffectCreatorActions>(effectCreatorName, engineStorage, target);
			engineStorage->addEffectCreator(newEffectCreator);
		}

		storage.currentCharges--;
	}
} // namespace TDEngine::Inner