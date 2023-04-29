cmake_minimum_required (VERSION 3.8)

add_library(Engine)
add_library(ProjectOpenGl::Engine ALIAS Engine)

find_package(GLEW REQUIRED)
find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(SFML CONFIG REQUIRED COMPONENTS graphics system window)
find_package(ImGui-SFML CONFIG REQUIRED)



target_link_libraries(Engine PUBLIC
    project_options
    GLEW::GLEW
    imgui::imgui
    sfml-system sfml-network sfml-graphics sfml-window
    ImGui-SFML::ImGui-SFML
)

target_include_directories(Engine PUBLIC
 $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../>
)

target_sources(Engine PRIVATE
    "Game/Game.h" "Game/Game.cpp"
    "Scene/Scene.h" "Scene/Scene.cpp"
)