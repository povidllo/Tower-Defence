#include "MainManager.h"
#include <iostream>
#include <memory>

namespace TDEngine::Inner {

	MainManager::MainManager(Project &proj, unsigned int width, unsigned int height) :
		project(proj),
		// Создаем Engine через shared_ptr, как было у вас, но лучше хранить сам объект или unique_ptr
		engine(std::make_shared<Project>(project)),
		window(sf::VideoMode(width, height), "Tower Defence", sf::Style::Titlebar | sf::Style::Close),
		renderer(window) {
		window.setFramerateLimit(60); // Ограничим FPS, чтобы не жарить CPU
	}

	std::string MainManager::getMapBackgroundImgPath(const std::string &mapName) {
		for (const auto &map: project.getMaps()) {
			if (map->getName() == mapName) {
				return map->getFinalMapImagePath();
			}
		}
		return "";
	}

	void MainManager::run(const std::string &mapName) {
		gameStatus = engine.startGame(mapName);

		// Загрузка фона
		std::string bgPath = getMapBackgroundImgPath(mapName);
		backgroundSprite.setTexture(renderer.getTexture(bgPath));

		sf::Clock clock;

		// Игровой цикл
		while (window.isOpen()) {
			sf::Time dt = clock.restart(); // Время, прошедшее с прошлого кадра

			processEvents();
			update(dt);
			render();
		}
	}

	void MainManager::processEvents() {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			} else if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					handleMouseClick(event.mouseButton.x, event.mouseButton.y);
				}
			}
		}
	}

	void MainManager::handleMouseClick(int mouseX, int mouseY) {
		// 1. Преобразуем координаты мыши в мировые (важно, если камера будет двигаться)
		// Для UI, который "приклеен" к экрану, можно использовать mouseX/mouseY напрямую,
		// но mapPixelToCoords универсальнее.
		sf::Vector2f worldPos = window.mapPixelToCoords({mouseX, mouseY});

		// --- ШАГ 1: Проверка клика по кнопкам улучшения (UI) ---
		// Проверяем UI ПЕРЕД проверкой карты, так как UI обычно рисуется поверх.
		if (selectedTower) {
			for (const auto &option: currentUpgradeOptions) {
				if (option.bounds.contains(worldPos)) {

					// ВЫВОД В ТЕРМИНАЛ (То, что вы просили)
					std::cout << "[ACTION] Tower at {" << selectedTower->positionCoordinates.first << ";"
							  << selectedTower->positionCoordinates.second << "}"
							  << " wants to upgrade to: " << option.name << std::endl;
					TowerUpgradeAction newUpgrade(option.name, std::static_pointer_cast<TowerActions>(selectedTower));
					playerAction = std::make_shared<TowerUpgradeAction>(
							option.name, std::static_pointer_cast<TowerActions>(selectedTower));

					// Здесь можно вызвать логику апгрейда:
					// engine.upgradeTower(selectedTower, option.name);

					// После апгрейда можно сбросить выделение или обновить список
					// selectedTower = nullptr;
					// currentUpgradeOptions.clear();

					return; // Прерываем функцию, чтобы не кликнуть сквозь кнопку на карту
				}
			}
		}

		// --- ШАГ 2: Проверка клика по карте (выбор башни) ---
		bool clickedOnTower = false;

		for (const auto &obj: gameStatus->mapObjects) {
			if (obj->type != MapObjectTypes::Tower)
				continue;

			// Рассчитываем позицию объекта на экране
			float objX = static_cast<float>(obj->positionCoordinates.first) * RendererGame::TILE_SIZE;
			float objY = static_cast<float>(obj->positionCoordinates.second) * RendererGame::TILE_SIZE;

			// Создаем прямоугольник коллизии
			sf::FloatRect objBounds(objX, objY, RendererGame::TILE_SIZE, RendererGame::TILE_SIZE);

			if (objBounds.contains(worldPos)) {
				// Мы нашли башню!
				selectedTower = obj; // Запоминаем её
				clickedOnTower = true;

				std::cout << "Selected tower at " << obj->positionCoordinates.first << ":"
						  << obj->positionCoordinates.second << std::endl;

				// Очищаем старые кнопки
				currentUpgradeOptions.clear();

				// Получаем доступ к методам башни (приведение типов)
				// В вашем коде это было std::static_pointer_cast<TowerActions>
				// Убедитесь, что MapObject имеет виртуальный деструктор для безопасности!
				auto towerPtr = std::static_pointer_cast<TowerActions>(obj);

				// Формируем список кнопок
				int index = 0;
				float uiStartX = 600.0f; // Координата X меню справа

				for (const auto &upgName: towerPtr->storage.getUpgradeNames()) {
					for (const auto &towerConfig: project.getTowers()) {
						if (towerConfig->getName() == upgName) {

							// Рассчитываем позицию кнопки
							float btnY = static_cast<float>(index) * 40.0f; // Шаг 40 пикселей
							sf::FloatRect btnRect(uiStartX, btnY, 32.0f, 32.0f);

							// Добавляем опцию с готовыми координатами
							currentUpgradeOptions.push_back(
									{&renderer.getTexture(towerConfig->getTowerTexturePath()), upgName, btnRect});

							index++;
							break;
						}
					}
				}
				return; // Выходим, так как нашли башню
			}
		}

		// --- ШАГ 3: Клик в пустоту ---
		// Если кликнули не по UI и не по башне — снимаем выделение
		if (!clickedOnTower) {
			selectedTower = nullptr;
			currentUpgradeOptions.clear();
			std::cout << "Selection cleared." << std::endl;
		}
	}

	void MainManager::update(sf::Time dt) {
		// Здесь передаем dt в движок, если он поддерживает (gameStep(dt)).
		// Если движок дискретный (шаговый), то вызываем gameStep раз в N времени.
		// Пример для пошагового вызова:

		static sf::Time timeAccumulator = sf::Time::Zero;
		timeAccumulator += dt;
		const sf::Time TIME_PER_FRAME = sf::seconds(1.f / 60.f); // Логика на 60 гц

		while (timeAccumulator > TIME_PER_FRAME) {
			gameStatus = engine.gameStep(playerAction);
			playerAction = nullptr;
			timeAccumulator -= TIME_PER_FRAME;
		}
	}

	void MainManager::render() {
		window.clear(); // Цвет очистки (например, черный)

		window.draw(backgroundSprite);
		renderer.renderFrame(gameStatus);

		// Отрисовка UI
		sf::Sprite uiSprite;
		for (const auto &opt: currentUpgradeOptions) {
			if (opt.texture) {
				uiSprite.setTexture(*opt.texture);
				// Используем координаты из сохраненного bounds
				uiSprite.setPosition(opt.bounds.left, opt.bounds.top);
				window.draw(uiSprite);
			}
		}

		window.display();
	}
} // namespace TDEngine::Inner
