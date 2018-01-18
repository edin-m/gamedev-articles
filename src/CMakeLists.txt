
set(SRCLIB_SOURCES
  bvh/bvh.cpp
  meshdata.cpp
  meshloader.cpp
  meshgraphics.cpp
  shader.cpp)

add_library(srclib
  STATIC
  ${SRCLIB_SOURCES})

target_link_libraries(srclib ${EXT_LIBS})
target_include_directories(srclib PUBLIC ${CMAKE_SOURCE_DIR})
