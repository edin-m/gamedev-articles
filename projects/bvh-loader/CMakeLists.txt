
set(BVH_SOURCES
  main.cpp)

message(${EXT_LIBS})

add_executable(bvh-loader ${BVH_SOURCES})
target_link_libraries(bvh-loader ${EXT_LIBS})

