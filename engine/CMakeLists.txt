cmake_minimum_required(VERSION 4.0)
project(engine)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(engine main.cpp
        src/inner/core/Engine.cpp
        src/inner/core/Engine.h
        src/inner/core/TickGenerator.cpp
        src/inner/core/TickGenerator.h
        src/inner/core/BoundaryDataTransfer.cpp
        src/inner/core/BoundaryDataTransfer.h
        src/inner/core/EngineStorage.cpp
        src/inner/core/EngineStorage.h
        src/inner/game_entities/IActing.h
        src/inner/game_entities/MapObject.h
        src/inner/game_entities/GameStatus.h
        src/inner/game_entities/Projectile.cpp
        src/inner/game_entities/Projectile.h
        src/inner/game_entities/WaveActions.cpp
        src/inner/game_entities/WaveActions.h
        src/inner/game_entities/AbilityActions.cpp
        src/inner/game_entities/AbilityActions.h
        src/inner/game_entities/TowerActions.cpp
        src/inner/game_entities/TowerActions.h
        src/inner/game_entities/EnemyActions.cpp
        src/inner/game_entities/EnemyActions.h
        src/inner/Zaglushki/MapSample.h
        src/inner/core/FrameData.h
        src/inner/player_actions/Replay.h
        src/inner/player_actions/IPlayerAction.h
        src/inner/player_actions/AbilityUseAction.cpp
        src/inner/player_actions/AbilityUseAction.h
        src/inner/player_actions/TowerUpgradeAction.cpp
        src/inner/player_actions/TowerUpgradeAction.h
        src/inner/core/ProjectLoader.cpp
        src/inner/core/ProjectLoader.h
        src/inner/Zaglushki/ProjectSample.h
)
target_link_libraries(engine
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

