cmake_minimum_required(VERSION 2.8)

project(gamedev-articles)

set(GIT_ROOT ${PROJECT_SOURCE_DIR}/)
set(SRC_PATH ${GIT_ROOT}/src)

set(CMAKE_BINARY_DIR ${GIT_ROOT}/bin)
set(EXECUTABLE_OUTPUT_PATH ${GIT_ROOT}/bin)
set(LIBRARY_OUTPUT_PATH ${GIT_ROOT}/bin)

include_directories("${SRC_PATH}")

set(CMAKE_CXX_STANDARD 11)

# Specify the list of directories to search for cmake modules.
set(CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
)

option(USE_GLFW "Use GLFW" ON)
option(USE_GLEW "Use GLEW" ON)

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
  add_subdirectory(${GIT_ROOT}/lib/glfw)
  set(EXT_LIBS ${EXT_LIBS} glfw)
endif (USE_GLFW)

if (APPLE AND (NOT USE_GLEW))
  set(EXT_LIBS ${EXT_LIBS} "-framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo")
endif ()

if (USE_GLEW)
  add_definitions(-DGLEW_STATIC)
  set(GLEW_USE_STATIC_LIBS ON)
  set(GLEW_INCLUDE_DIR "${GIT_ROOT}/lib_bin/glew-2.1.0/macos/include")
  set(GLEW_LIBRARY "${GIT_ROOT}/lib_bin/glew-2.1.0/macos/lib")
  if (APPLE)
    find_package(GLEW REQUIRED)
    if (GLEW_FOUND)
        include_directories(${GLEW_INCLUDE_DIR})
    endif()
    link_directories(${GLEW_LIBRARY})
    set(EXT_LIBS ${EXT_LIBS} GLEW)
  endif(APPLE)
endif (USE_GLEW)

add_subdirectory(projects)
