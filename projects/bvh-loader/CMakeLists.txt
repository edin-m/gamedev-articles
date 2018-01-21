set(SOURCES
  main.cpp
  bvh.cpp)

add_executable(bvh-loader ${SOURCES})
target_link_libraries(bvh-loader srclib)

#add_custom_command(TARGET bvh-loader
#          POST_BUILD
#          COMMAND ${CMAKE_COMMAND} -E copy_directory ${GIT_ROOT}/data/bvh-loader ${GIT_ROOT}/bin/bvh-loader
#)
