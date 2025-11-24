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
)

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