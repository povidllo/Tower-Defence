#include "EngineStorage.h"

namespace TDEngine {
    namespace Inner {
        //Эту функцию необходимо реализовать с использованием нормального mapSample
        EngineStorage::EngineStorage(std::shared_ptr<Project> project)
            : curGameStatus(std::make_shared<GameStatus>(GameStatus())), curMap(nullptr),
    		curProject(project), curWave(0)
        {
        }

        std::vector<std::shared_ptr<IActing>> EngineStorage::getEverythingActing() {
            std::vector<std::shared_ptr<IActing>> actings;

            actings.insert(actings.end(), activeProjectiles.begin(), activeProjectiles.end());
            // actings.insert(actings.end(), activeAbilities.begin(), activeAbilities.end());
            actings.insert(actings.end(), activeTowers.begin(), activeTowers.end());
            actings.insert(actings.end(), activeEnemies.begin(), activeEnemies.end());
            actings.insert(actings.end(), activeWaves.begin(), activeWaves.end());
            actings.insert(actings.end(), activeEffectCreators.begin(), activeEffectCreators.end());
            actings.insert(actings.end(), activeEnemyEffects.begin(), activeEnemyEffects.end());

            return actings;
        }

    	std::vector<std::shared_ptr<EnginePlayer>> EngineStorage::getAllPlayers() {
	        std::vector<std::shared_ptr<EnginePlayer>> players;
        	for (auto team : curGameStatus->teams) {
				for (auto player : team->teamPlayers) {
					players.push_back(player);
				}
        	}
        	return players;
        }

    	void EngineStorage::reloadMapPlayers() {
        	curGameStatus->teams.clear();

        	std::cout << "[INFO] Loading players" << std::endl;
        	if (curMap->getTeams().size() == 0) {
        		Team teamSample(std::string("team 0"));
        		curMap->getTeams().push_back(std::make_shared<Team>(teamSample));
        		std::cout << "[INFO] Created team " << teamSample.getTeamName() << std::endl;
        	}
        	if (curMap->getTeams()[0]->getPlayers().size() == 0) {
        		auto team = curMap->getTeams()[0];
        		team->addPlayer("player 0");
        		Player& playerSample = team->getPlayers()[0];
        		playerSample.setHp(curMap->getHp());
        		playerSample.setStartCurrency(curMap->getStartCurrency());
        		std::cout << "[INFO] Created player " << team->getPlayers()[0].getPlayerName() << std::endl;
        	}
        	for (const auto& team : curMap->getTeams()) {
        		EngineTeam engineTeam(*team);
        		for (const auto& player : team->getPlayers()) {
        			EnginePlayer enginePlayer(player);
        			enginePlayer.currentCurrency = enginePlayer.getStartCurrency();
        			enginePlayer.currentHp = enginePlayer.getHp();
        			enginePlayer.status = EnginePlayer::PLAYING;
        			enginePlayer.team = std::make_shared<EngineTeam>(engineTeam);
        			engineTeam.teamPlayers.push_back(std::make_shared<EnginePlayer>(enginePlayer));
        		}
        		curGameStatus->teams.push_back(std::make_shared<EngineTeam>(engineTeam));
        	}

        	std::cout << "[INFO] Loaded players amount: " << getAllPlayers().size() << std::endl;
        	std::cout << "[INFO] Loaded players: : ";
        	for (auto player : getAllPlayers()) {
        			std::cout <<  player->getPlayerName() << " ";
        	}
        		std::cout << std::endl;
        }

        void EngineStorage::cleanMap() {
            for (int i = 0; i < activeProjectiles.size(); i++) {
            	auto projectilePtr = activeProjectiles[i];
                if (!projectilePtr->target->storage.isAlive || !projectilePtr->isActive) {
                    removeProjectile(projectilePtr);
                }
            }
        	for (int i = 0; i < activeEnemies.size(); i++) {
        		auto enemyPtr = activeEnemies[i];
        		if (!enemyPtr->storage.isAlive) {
        			removeEnemy(enemyPtr);
        		}
        	}
        	for (int i = 0; i < activeWaves.size(); i++) {
        		auto wavePtr = activeWaves[i];
        		if (wavePtr->storage.spawningIndex == wavePtr->storage.getEnemies().size()) {
        			removeWave(wavePtr);
        		}
        	}
        	for (int i = 0; i < activeEnemyEffects.size(); i++) {
        		auto effectPtr = activeEnemyEffects[i];
        		if (effectPtr->storage.isFinished) {
        			removeEffectOnEnemy(effectPtr);
        		}
        	}
        	for (int i = 0; i < activeEffectCreators.size(); i++) {
        		auto effectCreatorPtr = activeEffectCreators[i];
        		if (effectCreatorPtr->storage.isFinished) {
        			removeEffectCreator(effectCreatorPtr);
        		}
        	}

        }

        void EngineStorage::addProjectile(const std::shared_ptr<Projectile> &projectile) {
            activeProjectiles.push_back(projectile);
            curGameStatus->mapObjects.push_back(projectile);

        }
        void EngineStorage::removeProjectile(const std::shared_ptr<Projectile> &projectile) {
            auto projectileIt =
                std::find(activeProjectiles.begin(), activeProjectiles.end(), projectile);
            if (projectileIt != activeProjectiles.end()) {
                activeProjectiles.erase(projectileIt);
            }

            std::shared_ptr<MapObject> mapObj = std::dynamic_pointer_cast<MapObject>(projectile);
            auto mapIt = std::find(curGameStatus->mapObjects.begin(), curGameStatus->mapObjects.end(), mapObj);
            if (mapIt != curGameStatus->mapObjects.end()) {
                curGameStatus->mapObjects.erase(mapIt);
            }

        }

        void EngineStorage::addTower(const std::shared_ptr<TowerActions> &tower) {
            activeTowers.push_back(tower);
            curGameStatus->mapObjects.push_back(tower);

        }
        void EngineStorage::removeTower(const std::shared_ptr<TowerActions> &tower) {
            auto towerIt = std::find(activeTowers.begin(), activeTowers.end(), tower);
            if (towerIt != activeTowers.end()) {
                activeTowers.erase(towerIt);
            }

            std::shared_ptr<MapObject> mapObj = std::dynamic_pointer_cast<MapObject>(tower);
            auto mapIt = std::find(curGameStatus->mapObjects.begin(), curGameStatus->mapObjects.end(), mapObj);
            if (mapIt != curGameStatus->mapObjects.end()) {
                curGameStatus->mapObjects.erase(mapIt);
            }

        }

    	void EngineStorage::addEnemy(const std::shared_ptr<EnemyActions> &enemy) {
        	activeEnemies.push_back(enemy);
        	curGameStatus->mapObjects.push_back(enemy);
        }

    	void EngineStorage::removeEnemy(const std::shared_ptr<EnemyActions> &enemy) {
        	auto enemyIt = std::find(activeEnemies.begin(), activeEnemies.end(), enemy);
        	if (enemyIt != activeEnemies.end()) {
        		activeEnemies.erase(enemyIt);
        	}

        	std::shared_ptr<MapObject> mapObj = std::dynamic_pointer_cast<MapObject>(enemy);
        	auto mapIt = std::find(curGameStatus->mapObjects.begin(), curGameStatus->mapObjects.end(), mapObj);
        	if (mapIt != curGameStatus->mapObjects.end()) {
        		curGameStatus->mapObjects.erase(mapIt);
        	}
        }
    	void EngineStorage::addWave(const std::shared_ptr<WaveActions> &wave) {
        	activeWaves.push_back(wave);
        }

    	void EngineStorage::removeWave(const std::shared_ptr<WaveActions> &wave) {
        	auto waveIt = std::find(activeWaves.begin(), activeWaves.end(), wave);
        	if (waveIt != activeWaves.end()) {
        		activeWaves.erase(waveIt);
        	}
        }

    	void EngineStorage::addEffectOnEnemy(const std::shared_ptr<EffectOnEnemyActions> &effect) {
        	activeEnemyEffects.push_back(effect);
        }

    	void EngineStorage::removeEffectOnEnemy(const std::shared_ptr<EffectOnEnemyActions> &effect) {
        	auto effectIt = std::find(activeEnemyEffects.begin(), activeEnemyEffects.end(), effect);
        	if (effectIt != activeEnemyEffects.end()) {
        		activeEnemyEffects.erase(effectIt);
        	}
        }

    	void EngineStorage::addEffectCreator(const std::shared_ptr<EffectCreatorActions> &effectCreator) {
        	activeEffectCreators.push_back(effectCreator);
        	curGameStatus->mapObjects.push_back(effectCreator);
        }

    	void EngineStorage::removeEffectCreator(const std::shared_ptr<EffectCreatorActions> &effectCreator) {
        	auto effectCreatorIt = std::find(activeEffectCreators.begin(), activeEffectCreators.end(), effectCreator);
        	if (effectCreatorIt != activeEffectCreators.end()) {
        		activeEffectCreators.erase(effectCreatorIt);
        	}

        	std::shared_ptr<MapObject> mapObj = std::dynamic_pointer_cast<MapObject>(effectCreator);
        	auto mapIt = std::find(curGameStatus->mapObjects.begin(), curGameStatus->mapObjects.end(), mapObj);
        	if (mapIt != curGameStatus->mapObjects.end()) {
        		curGameStatus->mapObjects.erase(mapIt);
        	}
        }
    } // Inner
} // TDEngine