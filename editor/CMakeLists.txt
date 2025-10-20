cmake_minimum_required(VERSION 3.22)
project(Editor)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


add_executable(editor
        src/main.cpp
        src/ProjectSelectorDialog.cpp
        include/ProjectSelectorDialog.h
        src/ProjectSelectorDialog.ui
        src/ProjectCreationDialog.cpp
        include/ProjectCreationDialog.h
        src/ProjectCreationDialog.ui
        ../Core/ProjectEditor.cpp
        ../Core/ProjectEditor.h
        src/MainWindow.cpp
        include/MainWindow.h
        src/MainWindow.ui
)

target_link_libraries(editor PRIVATE
        Qt6::Widgets
)


target_include_directories(editor PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../Core
)
