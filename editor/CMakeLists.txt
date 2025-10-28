cmake_minimum_required(VERSION 3.22)
project(Editor)

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
        src/Editors/MainWindow.cpp
        include/Editors/MainWindow.h
        src/Editors/MainWindow.ui
        entity/Project.cpp
        entity/Project.h
)

target_link_libraries(editor PRIVATE
        Qt6::Widgets
)


target_include_directories(editor PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Editors/
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Entity/
)
