cmake_minimum_required(VERSION 3.15)

set(ENGINE_PROJECT_NAME game_engine_core)

set(ENGINE_PUBLIC_INCLUDES
    includes/game_engine_core/app.hpp
    includes/game_engine_core/log.hpp
    includes/game_engine_core/event.hpp
)

set(ENGINE_PRIVATE_INCLUDES
    includes/game_engine_core/window.hpp
)

set(ENGINE_PRIVATE_SOURCES
    src/game_engine_core/app.cpp
    src/game_engine_core/log.cpp
    src/game_engine_core/window.cpp
    src/game_engine_core/event.cpp
)


add_library(${ENGINE_PROJECT_NAME}
${ENGINE_PUBLIC_INCLUDES}
    ${ENGINE_PRIVATE_INCLUDES}
    ${ENGINE_PRIVATE_SOURCES}
    )
    
    target_include_directories(${ENGINE_PROJECT_NAME} PUBLIC includes)
    target_include_directories(${ENGINE_PROJECT_NAME} PRIVATE src)
    target_compile_features(${ENGINE_PROJECT_NAME} PUBLIC cxx_std_17)
    
    add_subdirectory(../external/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
    target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE glfw)

add_subdirectory(../external/spdlog ${CMAKE_CURRENT_BINARY_DIR}/spdlog)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE spdlog)

add_subdirectory(../external/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE glad)

set(IMGUI_INCLUDES
    ../external/imgui/imgui.h
    ../external/imgui/backends/imgui_impl_glfw.h
    ../external/imgui/backends/imgui_impl_opengl3.h
)

set(IMGUI_SOURCES
    ../external/imgui/imgui.cpp
    ../external/imgui/backends/imgui_impl_glfw.cpp
    ../external/imgui/backends/imgui_impl_opengl3.cpp
    ../external/imgui/imgui_draw.cpp
    ../external/imgui/imgui_tables.cpp
    ../external/imgui/imgui_widgets.cpp
    ../external/imgui/imgui_demo.cpp
)

add_library(im_gui STATIC
    ${IMGUI_INCLUDES}
    ${IMGUI_SOURCES}
)

target_include_directories(im_gui PUBLIC ../external)
target_include_directories(im_gui PUBLIC ../external/imgui)

target_link_libraries(im_gui PRIVATE glad glfw)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE im_gui)
