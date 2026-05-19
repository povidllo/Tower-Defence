#include "Engine.h"

#include <memory>
namespace TDEngine {
    namespace Inner {
        Engine::Engine(std::shared_ptr<Project> project)
            : storage(std::make_shared<EngineStorage>(project)),
            tickGen(std::chrono::steady_clock::now())
        {
        }

        std::shared_ptr<GameStatus> Engine::gameStep(std::shared_ptr<IPlayerAction> action) {
                if (action != nullptr) {
	                action->MakeAction();
                }
                tickGen.tick(storage);
                storage->cleanMap();
        		checkForVictory();
				return storage->curGameStatus;
        }

        std::shared_ptr<GameStatus> Engine::startGame(const std::string& mapName) {
        	for (const auto& map : storage->curProject->getMaps()) {
        		if (map->getName() == mapName) {
        			storage->curMap = map;
        			initMap();
					return storage->curGameStatus;
        		}
        	}
        	throw std::invalid_argument("Map not found");
        }

    	void Engine::initMap() {
        	storage->curWave = 0;
	        // storage->activeAbilities.clear();
        	storage->activeEnemies.clear();
        	storage->activeProjectiles.clear();
        	storage->activeTowers.clear();
        	storage->activeWaves.clear();
        	storage->activeEnemyEffects.clear();
        	storage->activeEffectCreators.clear();
        	storage->curGameStatus->mapObjects.clear();
        	storage->curGameStatus->teams.clear();

        	for (const auto& team : storage->curMap->getTeams()) {
        		EngineTeam engineTeam(*team);
        		for (const auto& player : team->getPlayers()) {
        			EnginePlayer enginePlayer(player);
        			enginePlayer.currentCurrency = enginePlayer.getStartCurrency();
        			enginePlayer.currentHp = enginePlayer.getHp();
        			enginePlayer.status = EnginePlayer::PLAYING;
        			enginePlayer.team = std::make_shared<EngineTeam>(engineTeam);
        			engineTeam.teamPlayers.push_back(std::make_shared<EnginePlayer>(enginePlayer));
        		}
        		storage->curGameStatus->teams.push_back(std::make_shared<EngineTeam>(engineTeam));
        	}

        	for (const auto& tower : storage->curMap->getSpots()) {
        		storage->addTower(std::make_shared<TowerActions>(
        			TowerActions(*tower, {tower->getX(), tower->getY()}, getAllPlayers())));
        	}
        	if (!storage->curMap->getWaves().empty()) {
        		storage->addWave(std::make_shared<WaveActions>(WaveActions(*(storage->curMap->getWaves()[0]))));
        	}
        }

        void Engine::checkForVictory() {
			if (storage->activeWaves.size() == 0 &&
				storage->activeEnemies.size() == 0) {
				for (auto player : getAllPlayers()) {
					player->status = EnginePlayer::WON;
				}
			}
        }

    	std::vector<std::shared_ptr<EnginePlayer>> Engine::getAllPlayers() {
        	return storage->getAllPlayers();
		}





    } // Inner
} // TDEngine
