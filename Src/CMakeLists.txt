cmake_minimum_required (VERSION 3.8)


add_executable(testCmakeVcpkg2)

target_link_libraries(testCmakeVcpkg2 PUBLIC
    project_options
    testCmakeVcpkg2::Engine
    testCmakeVcpkg2::ProceduralGeneration
)
target_include_directories(testCmakeVcpkg2 PUBLIC
 $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../>
)

target_sources(testCmakeVcpkg2 PRIVATE
  "main.cpp"
  "Scenes/SceneEnum.h" "Scenes/MainScene.h"  "Scenes/MainScene.cpp"
)