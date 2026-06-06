#include "MainManager.h"
#include "NetworkUtil.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>

namespace TDEngine::Inner {
	namespace {
		constexpr unsigned short DEFAULT_LAN_PORT = 5501;
		constexpr sf::Uint8 PACKET_JOIN = 1;
		constexpr sf::Uint8 PACKET_WELCOME = 2;
		constexpr sf::Uint8 PACKET_START = 3;
		constexpr sf::Uint8 PACKET_UPGRADE = 4;
		constexpr sf::Uint8 PACKET_SNAPSHOT = 5;
		constexpr int SNAPSHOT_INTERVAL_MS = 50;
		constexpr float CLIENT_INTERP_SPEED = 14.f;

		bool isEphemeralMapObject(MapObjectTypes type) {
			return type == MapObjectTypes::Projectile || type == MapObjectTypes::EffectCreator;
		}

		int clampInt(int value, int minValue, int maxValue) {
			return std::max(minValue, std::min(value, maxValue));
		}
	}

	MainManager::MainManager(Project &proj, unsigned int width, unsigned int height) :
		project(proj), engine(std::make_shared<Project>(project)),
		window(sf::VideoMode(width, height), "Tower Defence", sf::Style::Titlebar | sf::Style::Close), renderer(window),
		state(AppState::MENU) {
		window.setFramerateLimit(60);
		initMenu();
	}

	void MainManager::initMenu() {
		menuButtons.clear();
		auto maps = project.getMaps();

		float btnWidth = 300.0f;
		float btnHeight = 60.0f;
		float startY = 170.0f;
		float spacing = 25.0f;
		float centerX = (window.getSize().x - btnWidth) / 2.0f;

		menuButtons.push_back({"LAN GAME", sf::FloatRect(centerX, startY, btnWidth, btnHeight), false});

		int index = 1;
		for (const auto &map: maps) {
			std::string name = map->getName();
			sf::FloatRect rect(centerX, startY + index * (btnHeight + spacing), btnWidth, btnHeight);
			menuButtons.push_back({name, rect, false});
			index++;
		}
	}

	void MainManager::initNetworkMenu() {
		networkButtons.clear();

		float btnWidth = 360.0f;
		float btnHeight = 50.0f;
		float startY = 145.0f;
		float spacing = 12.0f;
		float centerX = (window.getSize().x - btnWidth) / 2.0f;

		int index = 0;
		for (const auto &map: project.getMaps()) {
			if (!map->isOnlineEnabled()) {
				continue;
			}
			const std::string prefix = map->getName() == selectedNetworkMapName ? "MAP: " : "SELECT MAP: ";
			networkButtons.push_back({prefix + map->getName(),
									  sf::FloatRect(centerX, startY + index * (btnHeight + spacing), btnWidth,
													btnHeight),
									  false});
			index++;
		}

		networkButtons.push_back({"HOST SELECTED MAP",
								  sf::FloatRect(centerX, startY + index * (btnHeight + spacing), btnWidth, btnHeight),
								  false});
		index++;
		networkButtons.push_back({"JOIN " + joinAddress + ":" + std::to_string(networkPort),
								  sf::FloatRect(centerX, startY + index * (btnHeight + spacing), btnWidth, btnHeight),
								  false});
		index++;
		networkButtons.push_back({(editingNetworkField == "ip" ? "EDIT IP*: " : "EDIT IP: ") + joinAddress,
								  sf::FloatRect(centerX, startY + index * (btnHeight + spacing), btnWidth, btnHeight),
								  false});
		index++;
		networkButtons.push_back({(editingNetworkField == "port" ? "PORT*: " : "PORT: ") + std::to_string(networkPort),
								  sf::FloatRect(centerX, startY + index * (btnHeight + spacing), btnWidth, btnHeight),
								  false});
		index++;
		networkButtons.push_back({"BACK", sf::FloatRect(centerX, startY + index * (btnHeight + spacing), btnWidth,
														btnHeight),
								  false});
	}

	void MainManager::startGameLevel(const std::string &mapName) {
		stopNetwork();
		std::cout << "[INFO] Loading map: " << mapName << std::endl;
		gameStatus = engine.startGame(mapName, false);
		std::string bgPath = getMapBackgroundImgPath(mapName);
		backgroundSprite.setTexture(renderer.getTexture(bgPath));
		selectedTower = nullptr;
		currentUpgradeOptions.clear();
		playerAction = nullptr;
		wasVictory = false;
		state = AppState::GAME;
	}

	void MainManager::startNetworkHost(const std::string &mapName) {
		if (mapName.empty()) {
			networkStatus = "Select online map first";
			state = AppState::NETWORK_MENU;
			return;
		}
		stopNetwork();
		networkRole = NetworkRole::HOST;
		localPlayerIndex = 0;
		networkMapName = mapName;
		listener = std::make_unique<sf::TcpListener>();
		if (!listenTcpReuse(*listener, networkPort)) {
			networkStatus = "Cannot host on port " + std::to_string(networkPort);
			networkRole = NetworkRole::NONE;
			listener.reset();
			return;
		}
		listener->setBlocking(false);
		networkStatus = "Hosting " + mapName + " on port " + std::to_string(networkPort);

		std::cout << "[INFO] Hosting LAN game: " << mapName << std::endl;
		gameStatus = engine.startGame(mapName, true);
		std::string bgPath = getMapBackgroundImgPath(mapName);
		backgroundSprite.setTexture(renderer.getTexture(bgPath));
		selectedTower = nullptr;
		currentUpgradeOptions.clear();
		playerAction = nullptr;
		wasVictory = false;
		state = AppState::GAME;
	}

	void MainManager::startNetworkClient() {
		stopNetwork();
		networkRole = NetworkRole::CLIENT;
		serverSocket = std::make_unique<sf::TcpSocket>();
		serverSocket->setBlocking(true);
		networkStatus = "Connecting to " + joinAddress + ":" + std::to_string(networkPort) + "...";
		if (serverSocket->connect(joinAddress, networkPort, sf::seconds(3.0f)) != sf::Socket::Done) {
			networkStatus = "Connection failed: " + joinAddress + ":" + std::to_string(networkPort);
			serverSocket.reset();
			networkRole = NetworkRole::NONE;
			state = AppState::NETWORK_MENU;
			return;
		}
		serverSocket->setBlocking(false);
		sf::Packet joinPacket;
		joinPacket << PACKET_JOIN;
		serverSocket->send(joinPacket);
		gameStatus = std::make_shared<GameStatus>();
		clientEnemyInterp.clear();
		selectedTower = nullptr;
		currentUpgradeOptions.clear();
		networkStatus = "Connected. Waiting for host...";
		state = AppState::NETWORK_MENU;
	}

	std::string MainManager::getMapBackgroundImgPath(const std::string &mapName) {
		for (const auto &map: project.getMaps()) {
			if (map->getName() == mapName) {
				return map->getFinalMapImagePath();
			}
		}
		return "";
	}

	std::shared_ptr<Map> MainManager::getMapByName(const std::string &mapName) {
		for (const auto &map: project.getMaps()) {
			if (map->getName() == mapName) {
				return map;
			}
		}
		return nullptr;
	}

	void MainManager::run() {
		sf::Clock clock;
		while (window.isOpen()) {
			sf::Time dt = clock.restart();
			processEvents();
			update(dt);
			render();
		}
	}

	void MainManager::processEvents() {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				stopNetwork();
				window.close();
			} else if (event.type == sf::Event::MouseButtonPressed) {
        		std::cout << "[INFO] Processing event: MouseButtonPressed" << std::endl;
				if (event.mouseButton.button == sf::Mouse::Left) {
					if (state == AppState::MENU) {
						handleMenuClick(event.mouseButton.x, event.mouseButton.y);
					} else if (state == AppState::NETWORK_MENU) {
						sf::Vector2f worldPos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
						for (const auto &btn: networkButtons) {
							if (!btn.bounds.contains(worldPos)) {
								continue;
							}
							if (btn.text.rfind("MAP: ", 0) == 0) {
								selectedNetworkMapName = btn.text.substr(5);
								updateNetworkSettingsForSelectedMap();
								initNetworkMenu();
							} else if (btn.text.rfind("SELECT MAP: ", 0) == 0) {
								selectedNetworkMapName = btn.text.substr(12);
								updateNetworkSettingsForSelectedMap();
								initNetworkMenu();
							} else if (btn.text == "HOST SELECTED MAP") {
								startNetworkHost(selectedNetworkMapName);
							} else if (btn.text.rfind("JOIN ", 0) == 0) {
								startNetworkClient();
							} else if (btn.text.rfind("EDIT IP", 0) == 0) {
								editingNetworkField = editingNetworkField == "ip" ? "" : "ip";
								initNetworkMenu();
							} else if (btn.text.rfind("PORT", 0) == 0) {
								editingNetworkField = editingNetworkField == "port" ? "" : "port";
								initNetworkMenu();
							} else if (btn.text == "BACK") {
								stopNetwork();
								state = AppState::MENU;
							}
							return;
						}
					} else if (state == AppState::GAME) {
						handleGameClick(event.mouseButton.x, event.mouseButton.y);
					} else if (state == AppState::GAME_OVER) {
						handleGameOverClick();
					}
				}
			} else if (event.type == sf::Event::TextEntered && state == AppState::NETWORK_MENU) {
				if (event.text.unicode == '\b') {
					if (editingNetworkField == "ip" && !joinAddress.empty()) {
						joinAddress.pop_back();
					} else if (editingNetworkField == "port") {
						std::string portText = std::to_string(networkPort);
						if (!portText.empty()) {
							portText.pop_back();
						}
						networkPort =
								static_cast<unsigned short>(portText.empty() ? DEFAULT_LAN_PORT : std::stoi(portText));
					}
				} else if ((event.text.unicode >= '0' && event.text.unicode <= '9') || event.text.unicode == '.') {
					if (editingNetworkField == "ip" && joinAddress.size() < 15) {
						joinAddress.push_back(static_cast<char>(event.text.unicode));
					} else if (editingNetworkField == "port" && event.text.unicode != '.') {
						std::string portText = std::to_string(networkPort);
						if (portText == std::to_string(DEFAULT_LAN_PORT)) {
							portText.clear();
						}
						if (portText.size() < 5) {
							portText.push_back(static_cast<char>(event.text.unicode));
							const int parsedPort = clampInt(std::stoi(portText), 1, 65535);
							networkPort = static_cast<unsigned short>(parsedPort);
						}
					}
				}
				initNetworkMenu();
			} else if (event.type == sf::Event::MouseMoved) {
				if (state == AppState::MENU) {
					sf::Vector2f mousePos = window.mapPixelToCoords({event.mouseMove.x, event.mouseMove.y});
					for (auto &btn: menuButtons) {
						btn.isHovered = btn.bounds.contains(mousePos);
					}
				} else if (state == AppState::NETWORK_MENU) {
					rebuildNetworkButtonsHover(event.mouseMove.x, event.mouseMove.y);
				}
			}
		}
	}

	void MainManager::handleMenuClick(int mouseX, int mouseY) {
		sf::Vector2f worldPos = window.mapPixelToCoords({mouseX, mouseY});
		for (const auto &btn: menuButtons) {
			if (btn.bounds.contains(worldPos)) {
				if (btn.text == "LAN GAME") {
					rebuildNetworkMenu();
					initNetworkMenu();
					state = AppState::NETWORK_MENU;
					return;
				}
				startGameLevel(btn.text);
				return;
			}
		}
	}

	void MainManager::handleGameOverClick() {
		stopNetwork();
		state = AppState::MENU;
		gameStatus = nullptr;
	}

	void MainManager::handleGameClick(int mouseX, int mouseY) {
		sf::Vector2f worldPos = window.mapPixelToCoords({mouseX, mouseY});

		// 1. Проверка кликов по UI (Правая панель - Sidebar)
		if (worldPos.x >= RendererGame::UI_SIDEBAR_X) {
			if (selectedTower) {
				for (const auto &option: currentUpgradeOptions) {
					if (option.bounds.contains(worldPos)) {
        				std::cout << "[INFO] Processing event: tower upgrade" << std::endl;
						if (networkRole == NetworkRole::CLIENT) {
							sendUpgradeRequest(selectedTower->positionCoordinates.first,
											   selectedTower->positionCoordinates.second, option.name, localPlayerIndex);
						} else {
							applyUpgradeAt(selectedTower->positionCoordinates.first,
										   selectedTower->positionCoordinates.second, option.name, localPlayerIndex);
						}
						selectedTower = nullptr;
						currentUpgradeOptions.clear();
						return;
					}
				}
			}
			return;
		}

		// Если клик по верхней панели
		if (worldPos.y <= RendererGame::UI_TOP_BAR_HEIGHT) {
			return;
		}

		// 2. Проверка кликов по карте
		sf::Vector2u bgSize =
				backgroundSprite.getTexture() ? backgroundSprite.getTexture()->getSize() : sf::Vector2u(0, 0);
		sf::Vector2f mapOffset = RendererGame::getMapOffset(window.getSize(), bgSize);

		float localX = worldPos.x - mapOffset.x;
		float localY = worldPos.y - mapOffset.y;

		bool clickedOnTower = false;

		if (localX >= 0 && localY >= 0 && localX <= bgSize.x && localY <= bgSize.y) {
			for (const auto &obj: gameStatus->mapObjects) {
				if (obj->type != MapObjectTypes::Tower)
					continue;

				float objX = static_cast<float>(obj->positionCoordinates.first) * RendererGame::TILE_SIZE;
				float objY = static_cast<float>(obj->positionCoordinates.second) * RendererGame::TILE_SIZE;

				sf::FloatRect objBounds(objX, objY, RendererGame::TILE_SIZE, RendererGame::TILE_SIZE);

				if (objBounds.contains(localX, localY)) {
					const auto tower = std::static_pointer_cast<TowerActions>(obj);
					if (!canPlayerUseTower(getLocalPlayer(), tower)) {
						selectedTower = nullptr;
						currentUpgradeOptions.clear();
						return;
					}
					selectedTower = obj;
					clickedOnTower = true;
					currentUpgradeOptions.clear();

					int index = 0;

					// --- ИСПРАВЛЕНИЕ ЛОГИКИ РАЗМЕЩЕНИЯ КНОПОК ---
					// Вычисляем ширину доступной области внутри сайдбара
					float sidebarInnerWidth =
							window.getSize().x - RendererGame::UI_SIDEBAR_X - (2 * RendererGame::UI_PADDING);

					// Делаем кнопку почти во всю ширину сайдбара (с небольшими отступами)
					float btnMarginX = 10.0f;
					float btnWidth = sidebarInnerWidth - (2 * btnMarginX);
					float btnHeight = 50.0f; // Фиксированная высота кнопки-карточки

					float startX = RendererGame::UI_SIDEBAR_X + RendererGame::UI_PADDING + btnMarginX;
					float startY = 80.0f; // Отступ сверху (под заголовком)

					for (const auto &upgName: getUpgradeNamesForTower(obj)) {
						for (const auto &towerConfig: project.getTowers()) {
							if (towerConfig->getName() == upgName) {
								float btnY = startY + (index * (btnHeight + 10.0f)); // 10.0f - отступ между кнопками

								currentUpgradeOptions.push_back(
										{&renderer.getTexture(towerConfig->getTowerTexturePath()), upgName,
										 sf::FloatRect(startX, btnY, btnWidth, btnHeight)});
								index++;
								break;
							}
						}
					}
					return;
				}
			}
		}

		if (!clickedOnTower) {
			selectedTower = nullptr;
			currentUpgradeOptions.clear();
		}
	}

	void MainManager::update(sf::Time dt) {
		updateNetwork();

		if (state != AppState::GAME)
			return;

		auto localPlayer = getLocalPlayer();
		if (!localPlayer) {
			return;
		}
		if (networkRole == NetworkRole::CLIENT) {
			updateClientInterpolation(dt);
			if (gameStatus && (localPlayer->status == EnginePlayer::LOST || localPlayer->status == EnginePlayer::WON)) {
				wasVictory = (localPlayer->status == EnginePlayer::WON);
				state = AppState::GAME_OVER;
			}
			return;
		}

		static sf::Time timeAccumulator = sf::Time::Zero;
		timeAccumulator += dt;
		const sf::Time TIME_PER_FRAME = sf::seconds(1.f / 60.f);

		if (gameStatus && (localPlayer->status == EnginePlayer::LOST || localPlayer->status == EnginePlayer::WON)) {
			wasVictory = (localPlayer->status == EnginePlayer::WON);
			state = AppState::GAME_OVER;
			if (networkRole == NetworkRole::HOST) {
				sendSnapshotToClients();
			}
			return;
		}

		while (timeAccumulator > TIME_PER_FRAME) {
			if (gameStatus) {
				gameStatus = engine.gameStep(playerAction);
			}
			playerAction = nullptr;
			timeAccumulator -= TIME_PER_FRAME;
		}

		if (networkRole == NetworkRole::HOST &&
			snapshotClock.getElapsedTime() >= sf::milliseconds(SNAPSHOT_INTERVAL_MS)) {
			sendSnapshotToClients();
			snapshotClock.restart();
		}
	}

	void MainManager::render() {
		if (state == AppState::MENU) {
			renderer.renderMenu(menuButtons);
			window.display();
		} else if (state == AppState::NETWORK_MENU) {
			renderer.renderMenu(networkButtons, "LAN GAME", "IP: " + joinAddress + "    " + networkStatus);
			window.display();
		} else if (state == AppState::GAME) {
			window.clear(sf::Color(20, 20, 25));
			renderer.renderScene(gameStatus, backgroundSprite);
			renderer.renderUI(gameStatus, currentUpgradeOptions, getLocalPlayer());
			window.display();
		} else if (state == AppState::GAME_OVER) {
			window.clear(sf::Color(20, 20, 25));
			renderer.renderScene(gameStatus, backgroundSprite);
			renderer.renderUI(gameStatus, currentUpgradeOptions, getLocalPlayer());
			renderer.renderGameOver(wasVictory);
			window.display();
		}
	}

	void MainManager::updateNetwork() {
		if (networkRole == NetworkRole::HOST) {
			updateHostNetwork();
		} else if (networkRole == NetworkRole::CLIENT) {
			updateClientNetwork();
		}
	}

	void MainManager::updateHostNetwork() {
		if (!listener) {
			return;
		}

		while (true) {
			auto socket = std::make_unique<sf::TcpSocket>();
			socket->setBlocking(false);
			const sf::Socket::Status status = listener->accept(*socket);
			if (status == sf::Socket::NotReady) {
				break;
			}
			if (status != sf::Socket::Done) {
				break;
			}

			const auto map = getMapByName(networkMapName);
			const int maxPlayers = map ? std::max(1, map->getMaxPlayers()) : 1;
			if (static_cast<int>(clients.size()) + 1 >= maxPlayers) {
				socket->disconnect();
				continue;
			}

			NetworkClient client;
			client.playerIndex = static_cast<int>(clients.size()) + 1;
			client.socket = std::move(socket);
			sendStartToClient(client);
			clients.push_back(std::move(client));
			networkStatus = "Clients: " + std::to_string(clients.size());
		}

		for (auto it = clients.begin(); it != clients.end();) {
			sf::Packet packet;
			const sf::Socket::Status status = it->socket->receive(packet);
			if (status == sf::Socket::Done) {
				processClientPacket(*it, packet);
				++it;
			} else if (status == sf::Socket::Disconnected || status == sf::Socket::Error) {
				it = clients.erase(it);
				networkStatus = "Clients: " + std::to_string(clients.size());
			} else {
				++it;
			}
		}
	}

	void MainManager::updateClientNetwork() {
		if (!serverSocket) {
			return;
		}

		while (true) {
			sf::Packet packet;
			const sf::Socket::Status status = serverSocket->receive(packet);
			if (status == sf::Socket::Done) {
				processServerPacket(packet);
			} else if (status == sf::Socket::Disconnected || status == sf::Socket::Error) {
				networkStatus = "Disconnected from host";
				stopNetwork();
				state = AppState::NETWORK_MENU;
				initNetworkMenu();
				break;
			} else {
				break;
			}
		}
	}

	void MainManager::stopNetwork() {
		for (auto &client : clients) {
			if (client.socket) {
				client.socket->disconnect();
			}
		}
		clients.clear();

		if (listener) {
			listener->close();
		}
		listener.reset();

		if (serverSocket) {
			serverSocket->disconnect();
		}
		serverSocket.reset();

		clientEnemyInterp.clear();
		networkRole = NetworkRole::NONE;
		localPlayerIndex = 0;
		networkMapName.clear();
	}

	void MainManager::sendStartToClient(NetworkClient &client) {
		if (!client.socket) {
			return;
		}
		sf::Packet welcome;
		welcome << PACKET_WELCOME << static_cast<sf::Int32>(client.playerIndex);
		client.socket->send(welcome);

		sf::Packet start;
		start << PACKET_START << networkMapName;
		client.socket->send(start);
	}

	sf::Packet MainManager::buildSnapshotPacket() const {
		sf::Packet packet;
		packet << PACKET_SNAPSHOT;

		packet << static_cast<sf::Uint32>(gameStatus->teams.size());
		for (const auto &team : gameStatus->teams) {
			packet << static_cast<sf::Uint32>(team->teamPlayers.size());
			for (const auto &player : team->teamPlayers) {
				packet << static_cast<sf::Uint32>(player->currentHp)
				       << static_cast<sf::Uint32>(player->currentCurrency) << player->getPlayerName()
				       << static_cast<sf::Int32>(player->status);
			}
		}

		std::vector<std::shared_ptr<TowerActions>> towers;
		std::vector<std::shared_ptr<EnemyActions>> enemies;
		std::vector<std::shared_ptr<MapObject>> staticObjects;

		for (const auto &obj : gameStatus->mapObjects) {
			if (obj->type == MapObjectTypes::Tower) {
				towers.push_back(std::static_pointer_cast<TowerActions>(obj));
			} else if (obj->type == MapObjectTypes::Enemy) {
				enemies.push_back(std::static_pointer_cast<EnemyActions>(obj));
			} else if (!isEphemeralMapObject(obj->type)) {
				staticObjects.push_back(obj);
			}
		}

		packet << static_cast<sf::Uint32>(staticObjects.size());
		for (const auto &obj : staticObjects) {
			packet << static_cast<sf::Int32>(obj->type) << obj->texturePath << obj->positionCoordinates.first
			       << obj->positionCoordinates.second;
		}

		packet << static_cast<sf::Uint32>(towers.size());
		for (const auto &tower : towers) {
			packet << tower->texturePath;
			packet << tower->positionCoordinates.first << tower->positionCoordinates.second;
			const auto upgrades = tower->storage.getUpgradeNames();
			packet << static_cast<sf::Uint32>(upgrades.size());
			for (const auto &name : upgrades) {
				packet << name;
			}
			const auto owners = tower->storage.ownerPlayers;
			packet << static_cast<sf::Uint32>(owners.size());
			for (const auto &ownerPlayer : owners) {
				packet << ownerPlayer->getPlayerName();
			}
		}

		packet << static_cast<sf::Uint32>(enemies.size());
		for (const auto &enemy : enemies) {
			packet << enemy->texturePath;
			packet << enemy->positionCoordinates.first << enemy->positionCoordinates.second;
			packet << enemy->storage.currentHP << enemy->storage.getHealth();
		}

		return packet;
	}

	void MainManager::sendSnapshotToClients() {
		if (!gameStatus || clients.empty()) {
			return;
		}

		for (auto it = clients.begin(); it != clients.end();) {
			sf::Packet packet = buildSnapshotPacket();
			if (it->socket->send(packet) != sf::Socket::Done) {
				it = clients.erase(it);
				networkStatus = "Clients: " + std::to_string(clients.size());
			} else {
				++it;
			}
		}
	}

	void MainManager::processClientPacket(NetworkClient &client, sf::Packet &packet) {
		sf::Uint8 type = 0;
		packet >> type;
		if (type == PACKET_UPGRADE) {
			double x = 0.0;
			double y = 0.0;
			std::string upgradeName;
			packet >> x >> y >> upgradeName;
			std::cout << "[INFO] Server processing tower upgrade for: " << client.playerIndex;
			applyUpgradeAt(x, y, upgradeName, client.playerIndex);
		}
	}

	void MainManager::processServerPacket(sf::Packet &packet) {
		sf::Uint8 type = 0;
		packet >> type;
		if (type == PACKET_WELCOME) {
			sf::Int32 idx = 0;
			packet >> idx;
			localPlayerIndex = idx;
			networkStatus = "Connected as player " + std::to_string(localPlayerIndex + 1);
			return;
		}
		if (type == PACKET_START) {
			packet >> networkMapName;
			backgroundSprite.setTexture(renderer.getTexture(getMapBackgroundImgPath(networkMapName)));
			gameStatus = std::make_shared<GameStatus>();
			clientEnemyInterp.clear();
			selectedTower = nullptr;
			currentUpgradeOptions.clear();
			state = AppState::GAME;
			return;
		}
		if (type != PACKET_SNAPSHOT) {
			return;
		}

		auto newStatus = std::make_shared<GameStatus>();
		sf::Uint32 teamCount = 0;
		packet >> teamCount;
		for (sf::Uint32 t = 0; t < teamCount; ++t) {
			sf::Uint32 playerCount = 0;
			packet >> playerCount;
			auto team = std::make_shared<EngineTeam>(Team(std::string()));
			for (sf::Uint32 p = 0; p < playerCount; ++p) {
				sf::Uint32 hp = 0;
				sf::Uint32 gold = 0;
				std::string name;
				sf::Int32 status = 0;
				packet >> hp >> gold >> name >> status;
				auto player = std::make_shared<EnginePlayer>(Player(name, 0.0));
				player->currentHp = hp;
				player->currentCurrency = gold;
				player->status = static_cast<EnginePlayer::Status>(status);
				team->teamPlayers.push_back(player);
			}
			newStatus->teams.push_back(team);
		}

		sf::Uint32 objectCount = 0;
		packet >> objectCount;
		for (sf::Uint32 i = 0; i < objectCount; ++i) {
			sf::Int32 typeValue = 0;
			std::string texturePath;
			double x = 0;
			double y = 0;
			packet >> typeValue >> texturePath >> x >> y;
			newStatus->mapObjects.push_back(
				std::make_shared<MapObject>(texturePath, x, y, static_cast<MapObjectTypes>(typeValue)));
		}

		sf::Uint32 towerCount = 0;
		packet >> towerCount;
		for (sf::Uint32 i = 0; i < towerCount; ++i) {
			std::string texturePath;
			double x = 0;
			double y = 0;
			std::vector<std::string> upgrades;
			std::vector<std::shared_ptr<EnginePlayer>> ownerPlayers;
			sf::Uint32 upgradeCount = 0;
			sf::Uint32 ownersCount = 0;
			packet >> texturePath >> x >> y >> upgradeCount;
			for (sf::Uint32 j = 0; j < upgradeCount; ++j) {
				std::string name;
				packet >> name;
				upgrades.push_back(name);
			}
			packet >> ownersCount;
			for (sf::Uint32 j = 0; j < ownersCount; ++j) {
				std::string ownerName;
				packet >> ownerName;
				for (const auto &team : newStatus->teams) {
					for (const auto &player : team->teamPlayers) {
						if (player->getPlayerName() == ownerName) {
							ownerPlayers.push_back(player);
							break;
						}
					}
				}
			}
			auto towerObj =
				std::make_shared<TowerActions>(texturePath, std::pair<double, double>{x, y}, ownerPlayers, upgrades);
			newStatus->mapObjects.push_back(towerObj);
		}

		sf::Uint32 enemyCount = 0;
		packet >> enemyCount;
		std::vector<ClientInterpSample> newEnemyInterp;
		newEnemyInterp.reserve(enemyCount);

		for (sf::Uint32 i = 0; i < enemyCount; ++i) {
			std::string texturePath;
			double x = 0;
			double y = 0;
			double curHp = 0;
			double maxHp = 0;
			packet >> texturePath >> x >> y >> curHp >> maxHp;

			ClientInterpSample sample;
			sample.targetX = x;
			sample.targetY = y;
			if (i < clientEnemyInterp.size()) {
				sample.x = clientEnemyInterp[i].x;
				sample.y = clientEnemyInterp[i].y;
			} else {
				sample.x = x;
				sample.y = y;
			}
			newEnemyInterp.push_back(sample);

			auto enemyObj =
				std::make_shared<EnemyActions>(texturePath, std::pair<double, double>{sample.x, sample.y}, curHp, maxHp);
			newStatus->mapObjects.push_back(enemyObj);
		}

		clientEnemyInterp = std::move(newEnemyInterp);
		gameStatus = newStatus;
		if (state != AppState::GAME) {
			state = AppState::GAME;
		}
	}

	void MainManager::updateClientInterpolation(sf::Time dt) {
		if (networkRole != NetworkRole::CLIENT || !gameStatus || clientEnemyInterp.empty()) {
			return;
		}

		const float alpha = std::min(1.f, dt.asSeconds() * CLIENT_INTERP_SPEED);
		size_t enemyIndex = 0;

		for (auto &obj : gameStatus->mapObjects) {
			if (obj->type != MapObjectTypes::Enemy) {
				continue;
			}
			if (enemyIndex >= clientEnemyInterp.size()) {
				break;
			}

			auto &sample = clientEnemyInterp[enemyIndex++];
			sample.x += (sample.targetX - sample.x) * alpha;
			sample.y += (sample.targetY - sample.y) * alpha;
			obj->positionCoordinates.first = sample.x;
			obj->positionCoordinates.second = sample.y;
		}
	}

	void MainManager::sendUpgradeRequest(double x, double y, const std::string &upgradeName, int playerIndex) {
		if (!serverSocket) {
			return;
		}
		sf::Packet packet;
		packet << PACKET_UPGRADE << x << y << upgradeName << playerIndex;
		serverSocket->send(packet);
	}

	void MainManager::applyUpgradeAt(double x, double y, const std::string &upgradeName, int playerIndex) {
		const auto tower = findTowerAt(x, y);
		std::cout << "[INFO] Tower finding: ";
		if (!tower || playerIndex >= engine.getAllPlayers().size()) {
			std::cout << "fail" << std::endl;
			return;
		}
		std::cout << "success" << std::endl;

		const auto player = engine.getAllPlayers()[playerIndex];
		if (!canPlayerUseTower(player, tower)) {
			return;
		}
		playerAction = std::make_shared<TowerUpgradeAction>(upgradeName, tower, player);
	}

	std::shared_ptr<TowerActions> MainManager::findTowerAt(double x, double y) {
		if (!gameStatus) {
			return nullptr;
		}
		for (const auto &obj: gameStatus->mapObjects) {
			if (obj->type != MapObjectTypes::Tower) {
				continue;
			}
			if (std::fabs(obj->positionCoordinates.first - x) < 0.001 &&
				std::fabs(obj->positionCoordinates.second - y) < 0.001) {
				return std::static_pointer_cast<TowerActions>(obj);
			}
		}
		return nullptr;
	}

	std::vector<std::string> MainManager::getUpgradeNamesForTower(const std::shared_ptr<MapObject> &tower) const {
		if (!tower) {
			return {};
		}
		return std::static_pointer_cast<TowerActions>(tower)->storage.getUpgradeNames();
	}

	bool MainManager::canPlayerUseTower(std::shared_ptr<EnginePlayer> player, const std::shared_ptr<TowerActions> &tower) {
		if (!player || !tower) {
			return false;
		}
		if (networkRole == NetworkRole::NONE) {
			return true;
		}
		if (tower->storage.ownerPlayers.empty()) {
			return true;
		}
		return tower->checkOwnership(player);
	}

	void MainManager::rebuildNetworkButtonsHover(int mouseX, int mouseY) {
		sf::Vector2f mousePos = window.mapPixelToCoords({mouseX, mouseY});
		for (auto &btn: networkButtons) {
			btn.isHovered = btn.bounds.contains(mousePos);
		}
	}

	void MainManager::rebuildNetworkMenu() {
		if (selectedNetworkMapName.empty()) {
			for (const auto &map: project.getMaps()) {
				if (map->isOnlineEnabled()) {
					selectedNetworkMapName = map->getName();
					break;
				}
			}
		}
		updateNetworkSettingsForSelectedMap();
		initNetworkMenu();
	}

	void MainManager::updateNetworkSettingsForSelectedMap() {
		const auto map = getMapByName(selectedNetworkMapName);
		if (!map) {
			return;
		}
	}

	std::shared_ptr<EnginePlayer> MainManager::getLocalPlayer() {
    auto players = getAllPlayers();
	// std::cout << "[INFO] getting local player: " << localPlayerIndex << std::endl;
	if (localPlayerIndex < 0 || players.size() <= localPlayerIndex) {
		std::cerr << "[ERROR] localplayerindex not found: " << localPlayerIndex  << " allplayers: " << getAllPlayers().size() << std::endl;
			return nullptr;
		}
		return players[localPlayerIndex];
	}
	std::vector<std::shared_ptr<EnginePlayer>> MainManager::getAllPlayers() {
		std::vector<std::shared_ptr<EnginePlayer>> players;
		for (auto team : gameStatus->teams) {
			for (auto player : team->teamPlayers) {
				players.push_back(player);
			}
		}
		return players;
	}
} // namespace TDEngine::Inner
