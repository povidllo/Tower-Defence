#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "EffectOnEnemySample.h"
#include "EffectOnTowerSample.h"

namespace TDEngine::Inner {

enum class EffectTargetType { Enemies, Allies, Neutral };
enum class EffectAttachment { Point, MapObject };

class EffectCreatorSample {
public:
    std::vector<EffectOnEnemySample> initialEnemyEffects;
    // std::vector<EffectOnTowerSample> initialTowerEffects;
    std::vector<EffectOnEnemySample> periodicEnemyEffects;
    // std::vector<EffectOnTowerSample> periodicTowerEffects;
    double radius = 0.0;          // 0 = single target
    double duration = 0.0;        // 0 = permanent until object dies? But will be handled.
    double period = 0.0;          // how often to apply periodic effects
    EffectTargetType targetType = EffectTargetType::Enemies;
    EffectAttachment attachment = EffectAttachment::Point;
};

} // namespace TDEngine::Inner