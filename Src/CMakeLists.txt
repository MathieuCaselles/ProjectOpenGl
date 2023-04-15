cmake_minimum_required (VERSION 3.8)


add_executable(ProjectOpenGl)

target_link_libraries(ProjectOpenGl PUBLIC
    project_options
    ProjectOpenGl::Engine
    ProjectOpenGl::ProceduralGeneration
)
target_include_directories(ProjectOpenGl PUBLIC
 $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../>
)

target_sources(ProjectOpenGl PRIVATE
  "main.cpp"
  "Scenes/SceneEnum.h" "Scenes/MainScene.h"  "Scenes/MainScene.cpp"
)