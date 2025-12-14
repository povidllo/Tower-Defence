cmake_minimum_required(VERSION 3.22)
project(engine)
#set(CMAKE_CXX_COMPILER /usr/bin/g++-14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(engine
        main.cpp
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
        src/display/RendererGame.cpp
        src/display/RendererGame.h
        src/display/MainManager.cpp
        src/display/MainManager.h
        src/display/TextureCache.h
        src/display/TextureCache.cpp
        src/display/TextureCache.h
)

target_compile_options(engine PRIVATE -include valarray)

target_link_libraries(engine PRIVATE
        editor_common
)
