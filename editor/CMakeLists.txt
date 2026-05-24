find_package(Qt6 REQUIRED COMPONENTS Widgets)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


add_executable(editor
        src/main.cpp
        src/Editors/ProjectSelectorDialog.cpp
        include/Editors/ProjectSelectorDialog.h
        src/Editors/ProjectSelectorDialog.ui
        src/Editors/ProjectCreationDialog.cpp
        include/Editors/ProjectCreationDialog.h
        src/Editors/ProjectCreationDialog.ui
        src/Editors/ProjectEditor.cpp
        include/Editors/ProjectEditor.h
        src/Editors/ProjectEditor.ui
        src/Entity/Project.cpp
        include/Entity/Project.h
        include/Utils/Serializable.h
        src/Entity/AbilitySample.cpp
        include/Entity/AbilitySample.h
        src/Entity/EffectCreatorSample.cpp
        include/Entity/EffectCreatorSample.h
        src/Entity/EffectSample.cpp
        include/Entity/EffectSample.h
        src/Entity/EnemyEffectSample.cpp
        include/Entity/EnemyEffectSample.h
        src/Entity/TowerEffectSample.cpp
        include/Entity/TowerEffectSample.h
        src/Controls/AbilityController.cpp
        include/Controls/AbilityController.h
        src/Controls/EffectCreatorController.cpp
        include/Controls/EffectCreatorController.h
        src/Controls/EffectController.cpp
        include/Controls/EffectController.h
        src/Editors/AbilityEditor.cpp
        include/Editors/AbilityEditor.h
        src/Editors/AbilityEditor.ui
        src/Editors/EffectCreatorEditor.cpp
        include/Editors/EffectCreatorEditor.h
        src/Editors/EffectCreatorEditor.ui
        src/Editors/EffectEditor.cpp
        include/Editors/EffectEditor.h
        src/Editors/EffectEditor.ui
        src/Controls/ProjectController.cpp
        include/Controls/ProjectController.h
        include/Utils/BaseController.h
        src/Entity/TowerSample.cpp
        include/Entity/TowerSample.h
        src/Controls/TowerController.cpp
        include/Controls/TowerController.h
        src/Editors/TowerEditor.cpp
        include/Editors/TowerEditor.h
        src/Editors/TowerEditor.ui
        src/Entity/EnemySample.cpp
        include/Entity/EnemySample.h
        src/Controls/EnemyController.cpp
        include/Controls/EnemyController.h
        src/Editors/EnemyEditor.cpp
        include/Editors/EnemyEditor.h
        src/Editors/EnemyEditor.ui
        src/Utils/BaseEditor.cpp
        include/Utils/BaseEditor.h
        src/Entity/Map.cpp
        include/Entity/Map.h
        src/Controls/MapController.cpp
        include/Controls/MapController.h
        src/Utils/TextureManager.cpp
        include/Utils/TextureManager.h
        src/Editors/MapEditor.cpp
        include/Editors/MapEditor.h
        src/Editors/MapEditor.ui
        src/Utils/TextureUtils.cpp
        include/Utils/TextureUtils.h
        src/Entity/WaveSample.cpp
        include/Entity/WaveSample.h
        src/Entity/WaveChain.cpp
        include/Entity/WaveChain.h
        src/Editors/WaveEditor.cpp
        include/Editors/WaveEditor.h
        src/Editors/WaveEditor.ui
        src/Editors/WaveChainEditor.cpp
        include/Editors/WaveChainEditor.h
        src/Editors/WaveChainEditor.ui
        src/Entity/Player.cpp
        include/Entity/Player.h
        src/Entity/Team.cpp
        include/Entity/Team.h
)
#file(GLOB SRC_CONTROLS CONFIGURE_DEPENDS src/Controls/*.cpp)
#file(GLOB SRC_EDITORS CONFIGURE_DEPENDS src/Editors/*.cpp src/Editors/*.ui)
#file(GLOB SRC_ENTITY CONFIGURE_DEPENDS src/Entity/*.cpp)
#
#file(GLOB INC_CONTROLS CONFIGURE_DEPENDS include/Controls/*.hpp)
#file(GLOB INC_EDITORS CONFIGURE_DEPENDS include/Editors/*.hpp)
#file(GLOB INC_ENTITY CONFIGURE_DEPENDS include/Entity/*.hpp)
#
#add_executable(editor
#        src/main.cpp
#        ${SRC_CONTROLS}
#        ${SRC_EDITORS}
#        ${SRC_ENTITY}
#        ${INC_CONTROLS}
#        ${INC_EDITORS}
#        ${INC_ENTITY}
#        src/Editors/EnemyEditor.cpp
#        include/Editors/EnemyEditor.h
#        src/Editors/EnemyEditor.ui
#)

target_link_libraries(editor PRIVATE
        Qt6::Widgets
)


target_include_directories(editor PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Editors/
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Entity/
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Utils/
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Controls/
)

find_package(nlohmann_json REQUIRED)

target_link_libraries(editor PRIVATE nlohmann_json::nlohmann_json)
