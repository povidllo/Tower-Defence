cmake_minimum_required(VERSION 4.0)
project(engine)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(engine main.cpp
        ../editor/src/Entity/WaveSample.cpp
        ../editor/src/Entity/EnemySample.cpp
        ../editor/src/Entity/TowerSample.cpp
        src/inner/core/Engine.cpp
        src/inner/core/Engine.h
        src/inner/core/TickGenerator.cpp
        src/inner/core/TickGenerator.h
        src/inner/core/BoundaryDataTransfer.cpp
        src/inner/core/BoundaryDataTransfer.h
        src/inner/core/EngineStorage.cpp
        src/inner/core/EngineStorage.h
        src/inner/game_objects/IActing.h
        src/inner/game_objects/MapObject.h
        src/inner/game_objects/GameStatus.h
        src/inner/game_objects/Projectile.cpp
        src/inner/game_objects/Projectile.h
        src/inner/game_objects/WaveActions.cpp
        src/inner/game_objects/WaveActions.h
        src/inner/game_objects/AbilityActions.cpp
        src/inner/game_objects/AbilityActions.h
        src/inner/game_objects/TowerActions.cpp
        src/inner/game_objects/TowerActions.h
        src/inner/game_objects/EnemyActions.cpp
        src/inner/game_objects/EnemyActions.h
        src/inner/core/FrameData.h
        src/inner/player_actions/Replay.h
        src/inner/player_actions/IPlayerAction.h
        src/inner/player_actions/AbilityUseAction.cpp
        src/inner/player_actions/AbilityUseAction.h
        src/inner/player_actions/TowerUpgradeAction.cpp
        src/inner/player_actions/TowerUpgradeAction.h
        src/inner/game_objects/entities/Enemy.h
        src/inner/game_objects/entities/Wave.h
        src/inner/game_objects/entities/Tower.h
)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(nlohmann_json REQUIRED)

target_link_libraries(engine PRIVATE
        Qt6::Widgets
        nlohmann_json::nlohmann_json
)

target_include_directories(engine PRIVATE
        ${CMAKE_SOURCE_DIR}/../editor/include/Entity
        ${CMAKE_SOURCE_DIR}/../editor/include/Utils
)
#


