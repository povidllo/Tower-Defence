#pragma once
#include <memory>

#include "../../editor/include/Entity/EffectCreatorSample.h"
#include "../MapObject.h"
#include "EnginePlayer.h"
namespace TDEngine {
	namespace Inner {
		class EffectCreator : public EffectCreatorSample{
		public:
			explicit EffectCreator(EffectCreatorSample sample)
			: EffectCreatorSample(std::move(sample)) {}

			std::shared_ptr<MapObject> attachedObject;
			bool isFinished = false;
			bool initialApplied = false;
			double elapsedTime = 0.0;
			double timeSinceLastPeriod = 0.0;
        	std::vector<std::shared_ptr<EnginePlayer>> ownerPlayers;
		};
	} // Inner
} // TDEngine