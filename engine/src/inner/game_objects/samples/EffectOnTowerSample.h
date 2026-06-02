// #pragma once
// #include "EffectSample.h"
//
// namespace TDEngine::Inner {
//
// 	class EffectOnTowerSample : public EffectSample {
// 	public:
// 		double initialDamageAbsolute = 0.0;
// 		double initialDamagePercent = 0.0;   // e.g., 0.2 = +20%
// 		double initialAttackSpeedPercent = 0.0;
// 		double periodicDamageAbsolute = 0.0;
// 		double periodicDamagePercent = 0.0;
// 		double periodicAttackSpeedPercent = 0.0;
//
// 		nlohmann::json toJson() const override {
// 			return {
// 	            {"duration", duration},
// 				{"period", period},
// 				{"canStack", canStack},
// 				{"onEndEffects", onEndEffects},
// 				{"initialDamageAbsolute", initialDamageAbsolute},
// 				{"initialDamagePercent", initialDamagePercent},
// 				{"initialAttackSpeedPercent", initialAttackSpeedPercent},
// 				{"periodicDamageAbsolute", periodicDamageAbsolute},
// 				{"periodicDamagePercent", periodicDamagePercent},
// 				{"periodicAttackSpeedPercent", periodicAttackSpeedPercent}
// 			};
// 		}
//
// 		void fromJson(const nlohmann::json& j) override {
// 			duration = j.value("duration", 0.0);
// 			period = j.value("period", 0.0);
// 			canStack = j.value("canStack", false);
// 			if (j.contains("onEndEffects"))
// 				onEndEffects = j["onEndEffects"].get<std::vector<std::string>>();
// 			initialDamageAbsolute = j.value("initialDamageAbsolute", 0.0);
// 			initialDamagePercent = j.value("initialDamagePercent", 0.0);
// 			initialAttackSpeedPercent = j.value("initialAttackSpeedPercent", 0.0);
// 			periodicDamageAbsolute = j.value("periodicDamageAbsolute", 0.0);
// 			periodicDamagePercent = j.value("periodicDamagePercent", 0.0);
// 			periodicAttackSpeedPercent = j.value("periodicAttackSpeedPercent", 0.0);
// 		}
// 	};
//
// } // namespace TDEngine::Inner