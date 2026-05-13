#pragma once
#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include "EffectCreatorSample.h"

namespace TDEngine::Inner {

	enum class AbilityTargetType { NoTarget, Point, Tower, Enemy };

	class AbilitySample {
	public:
		std::string name;
		std::vector<EffectCreatorSample> effectsOnCast;
		AbilityTargetType targetType = AbilityTargetType::NoTarget;
		int maxCharges = 1;
		double chargeCooldown = 0.0;   // seconds
		double globalCooldown = 0.0;   // seconds

		nlohmann::json toJson() const {
			nlohmann::json j;
			j["name"] = name;
			j["targetType"] = static_cast<int>(targetType);
			j["maxCharges"] = maxCharges;
			j["chargeCooldown"] = chargeCooldown;
			j["globalCooldown"] = globalCooldown;
			nlohmann::json effArr = nlohmann::json::array();
			for (const auto& ec : effectsOnCast) effArr.push_back(ec.toJson());
			j["effectsOnCast"] = effArr;
			return j;
		}

		void fromJson(const nlohmann::json& j) {
			name = j.value("name", "");
			targetType = static_cast<AbilityTargetType>(j.value("targetType", 0));
			maxCharges = j.value("maxCharges", 1);
			chargeCooldown = j.value("chargeCooldown", 0.0);
			globalCooldown = j.value("globalCooldown", 0.0);
			if (j.contains("effectsOnCast")) {
				for (auto& item : j["effectsOnCast"]) {
					EffectCreatorSample ec;
					ec.fromJson(item);
					effectsOnCast.push_back(ec);
				}
			}
		}
	};

} // namespace TDEngine::Inner