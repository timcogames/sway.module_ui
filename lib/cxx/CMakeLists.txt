#---------------------------------------------------------------------------------
# CMake version
#---------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.25.0 FATAL_ERROR)

#---------------------------------------------------------------------------------
# [EMSCRIPTEN]: Setup
#---------------------------------------------------------------------------------

if(GLOB_EMSCRIPTEN_PLATFORM)
  emscripten_setup()
endif()

#---------------------------------------------------------------------------------
# 
#---------------------------------------------------------------------------------

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -fPIE")

set(SYSTEM_X11_INCLUDE_DIRS "/opt/X11/include")
set(SYSTEM_X11_LIBRARY_DIRS "/opt/X11/lib")

#---------------------------------------------------------------------------------
# 
#---------------------------------------------------------------------------------

include_directories("${SYSTEM_X11_INCLUDE_DIRS}")
include_directories("${GLOB_THIRD_PARTY_DIR}/json/include")
include_directories("${GLOB_FT2_ROOT_DIR}/include/freetype2")
include_directories("${MODULE_UI_LIST_DIR}/lib/cxx/include")
include_directories("${MODULE_UI_LIST_DIR}/submodules/sway.module_gapi/api/cxx/include")
include_directories("${MODULE_UI_LIST_DIR}/submodules/sway.module_core/lib/cxx/include")
include_directories("${MODULE_UI_LIST_DIR}/submodules/sway.module_math/lib/cpp/include")
include_directories("${MODULE_UI_LIST_DIR}/submodules/sway.module_rms/lib/cxx/include")

link_directories("${SYSTEM_X11_LIBRARY_DIRS}")

#---------------------------------------------------------------------------------
# Build object library
#---------------------------------------------------------------------------------

if(NOT TARGET ${MODULE_UI_OBJ_TARGET})
  set(MODULE_UI_LIB_SOURCES "${MODULE_UI_ROOT_DIR}/lib/cxx/src/*.*")
  set(MODULE_UI_ENABLE_FILE_EXPORT TRUE)

  create_object_library(${MODULE_UI_OBJ_TARGET} ${MODULE_UI_LIB_SOURCES} ${MODULE_UI_ENABLE_FILE_EXPORT})
  enable_cxx17(${MODULE_UI_OBJ_TARGET})
endif()

if(MODULE_UI_ENABLE_TESTS OR MODULE_UI_USE_GMOCK)
  target_compile_definitions(${MODULE_UI_OBJ_TARGET} PUBLIC -DUI_USE_GMOCK)
endif()

#---------------------------------------------------------------------------------
# Create interfaces
#---------------------------------------------------------------------------------

set(MODULE_UI_DEPENDENCY_LIST "module_ui_dependences")

if(NOT TARGET module_ui_dependences)
  set(TARGET_OBJECT_LIST)

  set(MODULE_CORE_OBJ_TARGET "module_core_obj")
  if(TARGET ${MODULE_CORE_OBJ_TARGET})
    list(APPEND TARGET_OBJECT_LIST $<TARGET_OBJECTS:${MODULE_CORE_OBJ_TARGET}>)
  endif()

  set(MODULE_RMS_OBJ_TARGET "module_rms_obj")
  if(TARGET ${MODULE_RMS_OBJ_TARGET})
    list(APPEND TARGET_OBJECT_LIST $<TARGET_OBJECTS:${MODULE_RMS_OBJ_TARGET}>)
  endif()

  if(TARGET ${MODULE_UI_OBJ_TARGET})
    list(APPEND TARGET_OBJECT_LIST $<TARGET_OBJECTS:${MODULE_UI_OBJ_TARGET}>)
  endif()

  create_interface(${MODULE_UI_DEPENDENCY_LIST} ${TARGET_OBJECT_LIST})
endif()

#---------------------------------------------------------------------------------
# [EMSCRIPTEN] Bitcode
#---------------------------------------------------------------------------------

if(NOT MODULE_UI_LIB_TYPE STREQUAL "object")
  if(GLOB_EMSCRIPTEN_PLATFORM)
    create_emscripten_library(${MODULE_UI_LIB_TARGET} ${MODULE_UI_ENVIRONMENT} ${MODULE_UI_COMPILATION} OUTPUT_TARGET_NAME_LIST)

    foreach(target IN LISTS OUTPUT_TARGET_NAME_LIST)
      target_link_options(${target} PUBLIC 
        "SHELL:-s STRICT=1"
        "SHELL:-s MODULARIZE=1"
        "SHELL:-s FULL_ES3=1"
        "SHELL:-s SUPPORT_LONGJMP=0"
        "SHELL:-s EXPORT_NAME=\"create_ui_module\""
        "SHELL:-s EXPORTED_FUNCTIONS=@${MODULE_UI_LIB_DIR}/exports.json"
        "SHELL:-s EXPORTED_RUNTIME_METHODS=@${MODULE_UI_LIB_DIR}/runtime-exports.json")

      if(MODULE_UI_MT)
        target_link_options(${target} PUBLIC 
          "SHELL:-Wl,--shared-memory,--no-check-features"
          "SHELL:-s USE_PTHREADS=1"
          "SHELL:-s PTHREAD_POOL_SIZE=8"
          "SHELL:-s IMPORTED_MEMORY=1")
      endif()

      printf_link_options(${target})
    endforeach()

  else()
    create_clang_library(${MODULE_UI_LIB_TARGET} "${MODULE_UI_ROOT_DIR}/lib/cxx/src/*.*")
  endif()
endif()
