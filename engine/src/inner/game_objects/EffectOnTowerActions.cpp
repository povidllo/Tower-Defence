// #include "EffectOnTowerActions.h"
// #include "TowerActions.h"
// #include "../core/EngineStorage.h"
//
// namespace TDEngine::Inner {
//
// EffectOnTowerActions::EffectOnTowerActions(EffectOnTowerSample sample, std::shared_ptr<TowerActions> target)
//     : storage(std::move(sample)), target(std::move(target)) {
//     originalDamage = target->storage.getDamage();
//     originalFireRate = target->storage.getFireRate();
// }
//
// void EffectOnTowerActions::act(uint64_t timePassedMillis, std::shared_ptr<EngineStorage> engineStorage) {
//     if (isFinished){ //|| !target->storage.isAlive()) {
//         //isFinished = true;
//         return;
//     }
//
//     double dt = timePassedMillis / 1000.0;
//     elapsedTime += dt;
//
//     if (!initialApplied) {
//         double dmgAbs = storage.initialDamageAbsolute;
//         double dmgPerc = storage.initialDamagePercent;
//         double newDamage = originalDamage + dmgAbs;
//         newDamage += originalDamage * dmgPerc;
//         target->storage.setDamage(newDamage);
//
//         double newFireRate = originalFireRate * (1.0 + storage.initialAttackSpeedPercent / 100.0);
//         target->storage.setFireRate(newFireRate);
//
//         initialApplied = true;
//     }
//
//     if (storage.period > 0.0) {
//         timeSinceLastPeriod += dt;
//         while (timeSinceLastPeriod >= storage.period) {
//             double dmgAbs = storage.periodicDamageAbsolute;
//             double dmgPerc = storage.periodicDamagePercent;
//             double newDamage = target->storage.getDamage() + dmgAbs;
//             newDamage += target->storage.getDamage() * dmgPerc;
//             target->storage.setDamage(newDamage);
//
//             double newFireRate = target->storage.getFireRate() * (1.0 + storage.periodicAttackSpeedPercent / 100.0);
//             target->storage.setFireRate(newFireRate);
//
//             timeSinceLastPeriod -= storage.period;
//         }
//     }
//
//     if (storage.duration > 0.0 && elapsedTime >= storage.duration) {
//         // Revert to original stats (or remove effect completely)
//         // For simplicity, we set back to original, but in real stacking scenario need more logic.
//         target->storage.setDamage(originalDamage);
//         target->storage.setFireRate(originalFireRate);
//         // Apply on-end effects similarly
//         isFinished = true;
//     }
// }
//
// } // namespace TDEngine::Inner