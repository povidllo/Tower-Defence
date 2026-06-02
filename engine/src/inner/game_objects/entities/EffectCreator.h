#pragma once
#include <memory>
#include "../samples/EffectCreatorSample.h"
#include "../MapObject.h"
namespace TDEngine {
	namespace Inner {
		class EffectCreator : public EffectCreatorSample{
		public:
			explicit EffectCreator(EffectCreatorSample sample)
			: EffectCreatorSample(std::move(sample)) {}

			std::shared_ptr<MapObject> attachedObject;
			bool isFinished = false;
			double elapsedTime = 0.0;
			double timeSinceLastPeriod = 0.0;
		};
	} // Inner
} // TDEngine