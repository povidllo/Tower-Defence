#include "AbilityActions.h"
#include "../core/EngineStorage.h"

namespace TDEngine::Inner {

	AbilityActions::AbilityActions(AbilitySample sample, std::shared_ptr<EnginePlayer> player)
		: storage(std::move(sample)) {
		storage.currentCharges = storage.getChargesCount();
		storage.timeAfterLastFullCharge = 0;
		storage.timeAfterSingleRecharge = 0;
		storage.target = nullptr;
		storage.onwerPlayers.push_back(player);
	}

	void AbilityActions::act(uint64_t timePassedMillis, std::shared_ptr<EngineStorage> engineStorage) {
		if (storage.currentCharges < storage.getChargesCount()) {
			// std::cout << "[INFO] ability acting cd" << std::endl;
            uint64_t chargeCooldown = ceil(storage.getChargeCooldownSeconds() * 1000);
			uint64_t fullCooldown = ceil(storage.getFullCooldownSeconds() * 1000);
			if (fullCooldown > 0 ) storage.timeAfterLastFullCharge += timePassedMillis;
			if (chargeCooldown > 0 ) storage.timeAfterSingleRecharge += timePassedMillis;

			// std::cout << "[INFO] Cd action data: " << chargeCooldown << " " << fullCooldown << " "
			//	 << storage.timeAfterLastFullCharge << " " << storage.timeAfterSingleRecharge << " "  << std::endl;
			if (fullCooldown == 0 && chargeCooldown == 0) {
				// std::cout << "[INFO] ability recharged by 0" << std::endl;
				storage.currentCharges = storage.getChargesCount();
			}
			if (fullCooldown > 0 && storage.timeAfterLastFullCharge >= fullCooldown) {
				// std::cout << "[INFO] ability recharged by fullcd" << std::endl;
				storage.currentCharges = storage.getChargesCount();
				storage.timeAfterLastFullCharge = 0;
				storage.timeAfterSingleRecharge = 0;
			}
			else if (chargeCooldown > 0 && storage.timeAfterSingleRecharge >= chargeCooldown) {
				// std::cout << "[INFO] ability recharged by chargecd" << std::endl;
				storage.currentCharges++;
				storage.timeAfterSingleRecharge = 0;
				if (storage.currentCharges == storage.getChargesCount()) {
					storage.timeAfterLastFullCharge = 0;
				}
			}
		}
		if (storage.target != nullptr && storage.currentCharges > 0) {
			std::cout << "[INFO] ability casting " << storage.getName() << std::endl;
			cast(engineStorage, storage.target);
			storage.target = nullptr;
		}
	}
	void AbilityActions::cast(std::shared_ptr<EngineStorage> engineStorage, std::shared_ptr<MapObject> target) {
		for (std::string effectCreatorName : storage.getEffectCreatorsOnCast()) {
			auto newEffectCreator = std::make_shared<EffectCreatorActions>(effectCreatorName, engineStorage, storage.onwerPlayers, target);
			engineStorage->addEffectCreator(newEffectCreator);
		}

		storage.currentCharges--;
	}

	uint64_t AbilityActions::getClosestCooldown() {
		uint64_t chargeCooldown = ceil(storage.getChargeCooldownSeconds() * 1000);
		uint64_t fullCooldown = ceil(storage.getFullCooldownSeconds() * 1000);
		uint64_t chargeCooldownLeft = (chargeCooldown >= storage.timeAfterSingleRecharge ? chargeCooldown - storage.timeAfterSingleRecharge : 0);
		uint64_t fullCooldownLeft = (fullCooldown >= storage.timeAfterLastFullCharge ? fullCooldown - storage.timeAfterLastFullCharge : 0);
		if (chargeCooldown == 0 && fullCooldown == 0) {
			return 0;
		}
		else if (chargeCooldown == 0) {
			return fullCooldownLeft;
		}
		else if (fullCooldown == 0) {
			return chargeCooldown;
		}
		else {
			return (fullCooldownLeft <= chargeCooldownLeft ? fullCooldownLeft : chargeCooldownLeft);
		}
	}
} // namespace TDEngine::Inner