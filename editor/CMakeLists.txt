cmake_minimum_required(VERSION 3.22)
project(Editor)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


add_executable(editor
        main.cpp
        ProjectSelectorDialog.cpp
        ProjectSelectorDialog.h
        ProjectSelectorDialog.ui
        ProjectCreationDialog.cpp
        ProjectCreationDialog.h
        ProjectCreationDialog.ui
        ../Core/ProjectEditor.cpp
        ../Core/ProjectEditor.h
        MainWindow.cpp
        MainWindow.h
        MainWindow.ui
)

target_link_libraries(editor PRIVATE
        Qt6::Widgets
)


target_include_directories(editor PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../Core
)
