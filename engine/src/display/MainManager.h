#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <memory>
#include <queue>
#include <vector>

#include "../inner/core/Engine.h"
#include "../inner/game_objects/MapObject.h"
#include "../inner/player_actions/TowerUpgradeAction.h"
#include "Project.h"
#include "RendererGame.h"

namespace TDEngine::Inner {

	struct ClientInterpSample {
		double x = 0;
		double y = 0;
		double targetX = 0;
		double targetY = 0;
	};

	enum class AppState { MENU, NETWORK_MENU, GAME, GAME_OVER };
	enum class NetworkRole { NONE, HOST, CLIENT };

	struct NetworkClient {
		std::unique_ptr<sf::TcpSocket> socket;
		int playerIndex = 0;
	};

	class MainManager {
	public:
		MainManager(Project &proj, unsigned int width, unsigned int height);

		void run();

	private:
		void processEvents();
		void update(sf::Time dt);
		void render();

		void handleMenuClick(int mouseX, int mouseY);
		void handleGameClick(int mouseX, int mouseY);

		void handleGameOverClick();

		void initMenu();
		void initNetworkMenu();
		void startGameLevel(const std::string &mapName);
		void startNetworkHost(const std::string &mapName);
		void startNetworkClient();
		std::string getMapBackgroundImgPath(const std::string &mapName);
		std::shared_ptr<Map> getMapByName(const std::string &mapName);

		void updateNetwork();
		void updateHostNetwork();
		void updateClientNetwork();
		void stopNetwork();
		void sendSnapshotToClients();
		void sendStartToClient(NetworkClient &client);
		void processClientPacket(NetworkClient &client, sf::Packet &packet);
		void processServerPacket(sf::Packet &packet);
		void updateClientInterpolation(sf::Time dt);
		sf::Packet buildSnapshotPacket() const;
		void sendUpgradeRequest(double x, double y, const std::string &upgradeName, int playerIndex);
		void applyUpgradeAt(double x, double y, const std::string &upgradeName, int playerIndex);
		std::shared_ptr<TowerActions> findTowerAt(double x, double y);
		std::vector<std::string> getUpgradeNamesForTower(const std::shared_ptr<MapObject> &tower) const;
		bool canPlayerUseTower(std::shared_ptr<EnginePlayer> player, const std::shared_ptr<TowerActions> &tower);
		void rebuildNetworkButtonsHover(int mouseX, int mouseY);
		void rebuildNetworkMenu();
		void updateNetworkSettingsForSelectedMap();
		std::shared_ptr<EnginePlayer> getLocalPlayer();
		std::vector<std::shared_ptr<EnginePlayer>> getAllPlayers();

		sf::RenderWindow window;
		Project &project;
		Engine engine;
		RendererGame renderer;
		AppState state;

		std::shared_ptr<GameStatus> gameStatus;
		sf::Sprite backgroundSprite;
		std::shared_ptr<TowerUpgradeAction> playerAction;
		std::shared_ptr<MapObject> selectedTower;
		std::vector<UpgradeOption> currentUpgradeOptions;

		bool wasVictory = false;

		std::vector<MenuButton> menuButtons;
		std::vector<MenuButton> networkButtons;

		NetworkRole networkRole = NetworkRole::NONE;
		std::unique_ptr<sf::TcpListener> listener;
		std::vector<NetworkClient> clients;
		std::unique_ptr<sf::TcpSocket> serverSocket;
		int localPlayerIndex = 0;
		unsigned short networkPort = 53000;
		std::string networkMapName;
		std::string joinAddress = "127.0.0.1";
		std::string selectedNetworkMapName;
		std::string editingNetworkField;
		std::string networkStatus;
		sf::Clock snapshotClock;
		std::vector<ClientInterpSample> clientEnemyInterp;
	};
} // namespace TDEngine::Inner
