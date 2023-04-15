# CMakeList.txt : projet CMake pour testCmakeVcpkg, incluez la source et définissez
# la logique spécifique au projet ici.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()


project (ProjectOpenGl)



list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")
#permet d'avoir un dossier bin qui se suffit à lui même pour envoyer un exécutable
include(build_configuration)



add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_20)


add_subdirectory(Tools)
add_subdirectory(Engine)
add_subdirectory(ProceduralGeneration)
add_subdirectory(Src)
