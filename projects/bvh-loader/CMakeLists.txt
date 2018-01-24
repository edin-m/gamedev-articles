set(PROJ_NAME bvh-loader)

set(SOURCES
  main.cpp
  bvh.cpp
  bvhloader.cpp
)

add_executable(${PROJ_NAME} ${SOURCES})
target_link_libraries(${PROJ_NAME} srclib)

#add_custom_command(TARGET bvh-loader
#          POST_BUILD
#          COMMAND ${CMAKE_COMMAND} -E copy_directory ${GIT_ROOT}/data/bvh-loader ${GIT_ROOT}/bin/bvh-loader
#)
