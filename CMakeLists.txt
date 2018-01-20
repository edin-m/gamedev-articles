cmake_minimum_required(VERSION 2.8)

project(gamedev-articles)

include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

set(GIT_ROOT ${PROJECT_SOURCE_DIR})
set(SRC_PATH ${GIT_ROOT}/src)
set(EXT_LIB_DIR ${GIT_ROOT}/lib)

set(EXECUTABLE_OUTPUT_PATH ${GIT_ROOT}/bin)
#set(LIBRARY_OUTPUT_PATH ${GIT_ROOT}/bin)

include_directories("${SRC_PATH}")

set(CMAKE_CXX_STANDARD 11)

# Specify the list of directories to search for cmake modules.
list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if (APPLE)
  set(CMAKE_MACOSX_RPATH 1)
  add_definitions(-DAPPLE)
endif()


#option(BUILD_IOS "Build iOS projects" OFF)

option(USE_GLFW "Use GLFW" ON)
option(USE_GLEW "Use GLEW" ON)
option(USE_DEVIL "Use DevIL" ON)
option(USE_SOIL "Use SOIL" OFF)
option(USE_ASSIMP "Use assimp" OFF)
option(USE_OPENEXR "Use OpenEXR" OFF)
option(USE_LOGGINGPP "Use loggingpp" ON)

# default to release
if (NOT CMAKE_BUILD_TYPE)
  message("Setting build type to release by default")
  set(CMAKE_CONFIGURATION_TYPES "Release" CACHE STRING "" FORCE)
endif()

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# GLFW
if (USE_GLFW)
  find_package(OpenGL REQUIRED)
  if (OPENGL_FOUND)
    include_directories(${OPENGL_INCLUDE_DIR})
  endif (OPENGL_FOUND)
  set(EXT_LIBS ${EXT_LIBS} ${OPENGL_gl_LIBRARY})
endif ()

if (USE_GLFW)
  set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  add_subdirectory(${EXT_LIB_DIR}/glfw)
  set(EXT_LIBS ${EXT_LIBS} glfw)
endif (USE_GLFW)

if (APPLE AND (NOT USE_GLEW))
  set(EXT_LIBS ${EXT_LIBS} "-framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo")
endif ()

if (USE_GLEW)
  if (NOT EXISTS ${GIT_ROOT}/lib_bin/glew-2.1.0.zip)
    execute_process(
        COMMAND wget -P ${GIT_ROOT}/lib_bin https://netcologne.dl.sourceforge.net/project/glew/glew/2.1.0/glew-2.1.0.zip
    )
  endif ()
  if (NOT EXISTS ${GIT_ROOT}/lib/glew-2.1.0)
    execute_process(
      COMMAND unzip ${GIT_ROOT}/lib_bin/glew-2.1.0.zip -d ${GIT_ROOT}/lib
    )
  endif()

  add_definitions(-DGLEW_STATIC)
  set(GLEW_USE_STATIC_LIBS ON)
  add_subdirectory(${EXT_LIB_DIR}/glew-2.1.0/build/cmake)
  set(EXT_LIBS ${EXT_LIBS} glew)
  include_directories(${EXT_LIB_DIR}/glew-2.1.0/include)
endif (USE_GLEW)

# GLM
include_directories(${EXT_LIB_DIR}/glm)

# DEVIL
if (USE_DEVIL)
  set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
  set(IL_TESTS OFF CACHE BOOL "" FORCE)
  add_subdirectory(${EXT_LIB_DIR}/DevIL/DevIL)
#  set_target_properties(IL PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${GIT_ROOT}/bin")
#  set_target_properties(IL PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${GIT_ROOT}/bin")
#  set_target_properties(IL PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${GIT_ROOT}/bin")
  set_target_properties(ILU PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${GIT_ROOT}/bin")
  set_target_properties(ILU PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${GIT_ROOT}/bin")
#  set_target_properties(ILU PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${GIT_ROOT}/bin")
  set_target_properties(ILUT PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${GIT_ROOT}/bin")
  set_target_properties(ILUT PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${GIT_ROOT}/bin")
#  set_target_properties(ILUT PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${GIT_ROOT}/bin")
  set(EXT_LIBS ${EXT_LIBS} IL ILU ILUT)
  include_directories(${GIT_ROOT}/lib/DevIL/DevIL/include)
endif()

if (USE_SOIL)
  add_subdirectory(${EXT_LIB_DIR}/soil)
  set(EXT_LIBS ${EXT_LIBS} SOIL)
  include_directories(${GIT_ROOT}/lib/soil/src)
endif()

# assimp
if (USE_ASSIMP)
  set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
  set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
  set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  add_subdirectory(${EXT_LIB_DIR}/assimp)
  if (assimp_FOUND)
    set(EXT_LIBS ${EXT_LIBS} ${assimp_LIBRARIES})
  endif()
endif()

## OpenEXR - unused
#if (USE_OPENEXR)
#  set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
##  add_subdirectory(${EXT_LIB_DIR}/openexr/IlmBase)
#  set(ILMBASE_PACKAGE_PREFIX ${GIT_ROOT}/del/openexr/IlmBase/build/install)
#  add_subdirectory(${EXT_LIB_DIR}/openexr/OpenEXR)
#endif()

if (USE_LOGGINGPP)
  set(build_static_lib ON CACHE BOOL "" FORCE)
  add_subdirectory(${EXT_LIB_DIR}/loggingpp)
  set(EXT_LIBS ${EXT_LIBS} ${easyloggingpp_LIBRARIES})
endif()

## GLFM - ios/andriod glfw
#if (BUILD_IOS OR BUILD_ANDROID)

#endif()

message(${EXT_LIBS})
add_subdirectory(src)
add_subdirectory(projects)
