cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME game_engine)

project(${PROJECT_NAME})

add_subdirectory(game_engine_core)
add_subdirectory(game_engine_editor)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    PROPERTY VS_STARTUP_PROJECT game_engine_editor
)
