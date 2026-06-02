// #pragma once
// #include "IActing.h" // maybe not needed, but for player abilities
// #include "samples/AbilitySample.h"
// #include "EffectCreatorActions.h"
// #include <memory>
//
// namespace TDEngine::Inner {
// 	class EngineStorage;
//
// 	class AbilityActions {
// 	public:
// 		AbilityActions(AbilitySample sample, std::pair<double,double> castPosition,
// 					   std::shared_ptr<MapObject> targetObject = nullptr);
// 		void cast(std::shared_ptr<EngineStorage> engineStorage);
//
// 		AbilitySample storage;
// 		int currentCharges;
// 		double chargeCooldownRemaining = 0.0;
// 		double globalCooldownRemaining = 0.0;
//
// 	private:
// 		std::pair<double,double> castPos;
// 		std::shared_ptr<MapObject> target;
// 	};
// } // namespace TDEngine::Inner