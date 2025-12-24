cmake_minimum_required(VERSION 3.22)
project(TowerDefence)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_FIND_PACKAGE_PREFER_CONFIG ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
find_package(nlohmann_json REQUIRED)

add_subdirectory(editor)
add_subdirectory(engine)

add_library(editor_common STATIC
        editor/src/Entity/EnemySample.cpp
        editor/src/Entity/Map.cpp
        editor/src/Entity/Project.cpp
        editor/src/Entity/TowerSample.cpp
        editor/src/Entity/WaveSample.cpp
        editor/src/Utils/BaseEditor.cpp
        editor/src/Utils/TextureManager.cpp
        editor/src/Utils/TextureUtils.cpp
)

target_include_directories(editor_common PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/editor/include
        ${CMAKE_CURRENT_SOURCE_DIR}/editor/include/Entity
        ${CMAKE_CURRENT_SOURCE_DIR}/editor/include/Utils
)

target_link_libraries(editor_common PUBLIC
        Qt6::Core
        Qt6::Widgets
        nlohmann_json::nlohmann_json
)
