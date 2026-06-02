#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

namespace TDEngine::Inner {

	// Base effect class (stub for editor version)
	class EffectSample {
	public:
		virtual ~EffectSample() = default;

		double duration = 0.0;        // seconds
		double period = 0.0;          // seconds, 0 = instant/one-time
		bool canStack = false;        // allow multiple instances
		std::vector<std::string> onEndEffects; // effect names to apply when this ends
	};

} // namespace TDEngine::Inner