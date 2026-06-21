#pragma once
#include "../../../../../editor/include/Entity/TowerSample.h"
#include "../../../../../editor//include/Entity/EffectCreatorSample.h"
namespace TDEngine {
    namespace Inner {
		class TowerActions;
		enum class TowerBehaviourTypes { //Виды поведения башен при выборе целей атаки
			//Соответственно самый ближний/дальний враги, враги с наименьшим/наибольшим количеством хп, враги с наибольшим числом соседей
			Closest,
			Farthest,
			LowestHP,
			HighestHP
		 };
        class Tower : public TowerSample{
        public:
        	explicit Tower(TowerSample sample)
        	: TowerSample(std::move(sample)) {}
        	explicit Tower(std::vector<std::string> upgrades)
			: TowerSample(std::string("")) {
        		for (auto &upgrade : upgrades) {
        			addNextUpgrade(upgrade);
        		}
        	}
            std::optional<std::string> setUpgradingTo;
        	std::shared_ptr<EnginePlayer> setUpgradingByPlayer;
            uint64_t timeAfterLastShot;
        	std::vector<std::shared_ptr<EnginePlayer>> ownerPlayers;
        	double curDamage;
        	double curFireRate;
        	double curHp;
        	bool initialActionsDone;
        	TowerBehaviourTypes behaviourType;
        	std::shared_ptr<TowerSample> originSample;
        	std::vector<std::shared_ptr<EnginePlayer>> originOwnerPlayers;
        	std::shared_ptr<TowerActions> self;
        };
    } // Inner
} // TDEngine