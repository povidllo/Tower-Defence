#pragma once
#include "IActing.h"
#include <memory>

#include "MapObject.h"
#include "entities/Ability.h"

namespace TDEngine::Inner {
	class EngineStorage;

	class AbilityActions : public IActing{
	public:
		AbilityActions(AbilitySample sample);
		void act(uint64_t timePassedMillis, std::shared_ptr<EngineStorage> engineStorage) override;
		void cast(std::shared_ptr<EngineStorage> engineStorage, std::shared_ptr<MapObject> target);

		Ability storage;
	};
} // namespace TDEngine::Inner