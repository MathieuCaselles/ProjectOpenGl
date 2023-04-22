cmake_minimum_required (VERSION 3.8)

add_library(ProceduralGeneration)
add_library(ProjectOpenGl::ProceduralGeneration ALIAS ProceduralGeneration)

target_link_libraries(ProceduralGeneration PUBLIC
    project_options
)
target_include_directories(ProceduralGeneration PUBLIC
 $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../>
)

target_sources(ProceduralGeneration PRIVATE
   "Grid3d.h" "Grid3d.cpp")
