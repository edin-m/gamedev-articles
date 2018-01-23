
set(SRCLIB_SOURCES
  meshdata.cpp
  meshloader.cpp
  meshgraphics.cpp
  glcheck.cpp
  shader.cpp
)

if (USE_DEVIL)
  list(APPEND SRCLIB_SOURCES
    textureloader.cpp
  )
endif()

add_library(srclib
  STATIC
  ${SRCLIB_SOURCES})

target_link_libraries(srclib ${EXT_LIBS})
target_include_directories(srclib PUBLIC ${CMAKE_SOURCE_DIR})
