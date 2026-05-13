// #include "AbilityActions.h"
// #include "../core/EngineStorage.h"
//
// namespace TDEngine::Inner {
//
// 	AbilityActions::AbilityActions(AbilitySample sample, std::pair<double,double> castPosition,
// 								   std::shared_ptr<MapObject> targetObject)
// 		: storage(std::move(sample)), castPos(castPosition), target(targetObject) {
// 		currentCharges = storage.maxCharges;
// 	}
//
// 	void AbilityActions::cast(std::shared_ptr<EngineStorage> engineStorage) {
// 		if (currentCharges <= 0) return;
// 		if (globalCooldownRemaining > 0.0) return;
// 		if (chargeCooldownRemaining > 0.0) return;
//
// 		// Apply each EffectCreator from storage.effectsOnCast
// 		for (auto& ecSample : storage.effectsOnCast) {
// 			std::shared_ptr<MapObject> attachObj = nullptr;
// 			if (ecSample.attachment == EffectAttachment::MapObject && target != nullptr) {
// 				attachObj = target;
// 			}
// 			std::pair<double,double> pos = (ecSample.attachment == EffectAttachment::Point) ? castPos : target->positionCoordinates;
// 			auto creator = std::make_shared<EffectCreatorActions>(ecSample, pos, attachObj);
// 			engineStorage->activeEffectCreators.push_back(creator);
// 			engineStorage->curGameStatus->mapObjects.push_back(creator);
// 		}
//
// 		currentCharges--;
// 		chargeCooldownRemaining = storage.chargeCooldown;
// 		globalCooldownRemaining = storage.globalCooldown;
// 	}
//
// } // namespace TDEngine::Inner