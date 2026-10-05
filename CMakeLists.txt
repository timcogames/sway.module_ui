#---------------------------------------------------------------------------------
# CMake version
#---------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.25.0 FATAL_ERROR)

#---------------------------------------------------------------------------------
# Project directories
#---------------------------------------------------------------------------------

set(MODULE_UI_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(MODULE_UI_LIST_DIR "${CMAKE_CURRENT_LIST_DIR}")
set(MODULE_UI_CMAKE_DIR "${MODULE_UI_ROOT_DIR}/submodules/sway.module_core/cmake_modules")
set(MODULE_UI_LIB_DIR "${MODULE_UI_ROOT_DIR}/lib")
set(MODULE_UI_BIN_DIR "${CMAKE_SOURCE_DIR}/bin")

#---------------------------------------------------------------------------------
# Include CMake modules
#---------------------------------------------------------------------------------

include("${MODULE_UI_CMAKE_DIR}/options.cmake")
include("${MODULE_UI_CMAKE_DIR}/set_output_dir.cmake")
include("${MODULE_UI_CMAKE_DIR}/project_metadata.cmake")
include("${MODULE_UI_CMAKE_DIR}/add_submodule.cmake")
include("${MODULE_UI_CMAKE_DIR}/modules/create_object_library.cmake")
include("${MODULE_UI_CMAKE_DIR}/modules/create_interface.cmake")
include("${MODULE_UI_CMAKE_DIR}/modules/logger.cmake")
include("${MODULE_UI_CMAKE_DIR}/modules/compiler/clang/create_library.cmake")
include("${MODULE_UI_CMAKE_DIR}/modules/compiler/emscripten/setup.cmake")
include("${MODULE_UI_CMAKE_DIR}/modules/compiler/emscripten/create_library.cmake")
include("${MODULE_UI_CMAKE_DIR}/modules/compiler/emscripten/gen_target_name.cmake")
include("${MODULE_UI_CMAKE_DIR}/modules/compiler/emscripten/set_optimization.cmake")
include("${MODULE_UI_CMAKE_DIR}/modules/compiler/emscripten/set_environment.cmake")
include("${MODULE_UI_CMAKE_DIR}/modules/compiler/emscripten/set_compilation.cmake")
include("${MODULE_UI_CMAKE_DIR}/modules/compiler/emscripten/set_modularize.cmake")
include("${MODULE_UI_CMAKE_DIR}/enable_cxx17.cmake")
include("${MODULE_UI_CMAKE_DIR}/enable_coverage.cmake")

#---------------------------------------------------------------------------------
# Опции сборки
#---------------------------------------------------------------------------------

option(MODULE_UI_BUILD_SHARED "Build shared library" OFF)
option(MODULE_UI_BUILD_TYPE "" "module") # module, bitcode
option(MODULE_UI_ENVIRONMENT "Environment variable" OFF)
option(MODULE_UI_COMPILATION "Compilation variable" OFF)
option(MODULE_UI_MT "Enable multithreading" OFF)
option(MODULE_UI_ENABLE_COVERAGE "Enable coverage builds" OFF)
option(MODULE_UI_ENABLE_EXAMPLES "Enable example builds" OFF)
option(MODULE_UI_ENABLE_TESTS "Enable test builds" OFF)
option(MODULE_UI_USE_GMOCK "Use google mock" OFF)

#---------------------------------------------------------------------------------
# [EMSCRIPTEN]: Setup
#---------------------------------------------------------------------------------

if(GLOB_EMSCRIPTEN_PLATFORM)
  emscripten_setup()
endif()

#---------------------------------------------------------------------------------
# Project
#---------------------------------------------------------------------------------

project_metadata("ui" 0.1.0)
project(${MODULE_UI_PROJ_NAME} VERSION ${MODULE_UI_VERSION} LANGUAGES CXX)

#---------------------------------------------------------------------------------
# 
#---------------------------------------------------------------------------------

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -fPIE")

#---------------------------------------------------------------------------------
# Submodules
#---------------------------------------------------------------------------------

add_submodule("module_core" submodules/sway.module_core)
add_submodule("module_rms" submodules/sway.module_rms)
add_submodule("module_render" submodules/sway.module_render)
add_submodule("module_ois" submodules/sway.module_ois)

#---------------------------------------------------------------------------------
# Executable output path
#---------------------------------------------------------------------------------

set_output_dir(${MODULE_UI_BIN_DIR})

set(SYSTEM_X11_INCLUDE_DIRS "/opt/X11/include")
set(SYSTEM_X11_LIBRARY_DIRS "/opt/X11/lib")

#---------------------------------------------------------------------------------
# Includes
#---------------------------------------------------------------------------------

include_directories("${SYSTEM_X11_INCLUDE_DIRS}")
include_directories("${GLOB_THIRD_PARTY_DIR}/json/include")
include_directories("${MODULE_UI_LIST_DIR}/lib/cxx/include")
include_directories("${MODULE_UI_LIST_DIR}/submodules/sway.module_gapi/api/cxx/include")
include_directories("${MODULE_UI_LIST_DIR}/submodules/sway.module_loader/api/cxx/include")
include_directories("${MODULE_UI_LIST_DIR}/submodules/sway.module_core/lib/cxx/include")
include_directories("${MODULE_UI_LIST_DIR}/submodules/sway.module_math/lib/cpp/include")
include_directories("${MODULE_UI_LIST_DIR}/submodules/sway.module_rms/lib/cxx/include")
include_directories("${MODULE_UI_LIST_DIR}/submodules/sway.module_render/lib/cxx/include")
include_directories("${MODULE_UI_LIST_DIR}/submodules/sway.module_ois/lib/cxx/include")

#---------------------------------------------------------------------------------
# Library
#---------------------------------------------------------------------------------

add_subdirectory(lib/cxx)

#---------------------------------------------------------------------------------
# Tests
#---------------------------------------------------------------------------------

if(MODULE_UI_ENABLE_TESTS)
  if(${MODULE_UI_LIB_TYPE} MATCHES "static")
    message(STATUS "[${MODULE_UI_PROJ_NAME}]: Tests have been enabled")
    add_subdirectory(lib/cxx/tests/google)
  else()
    message(STATUS "[${MODULE_UI_PROJ_NAME}]: Tests are only allowed with a static lib")
  endif()
endif()

