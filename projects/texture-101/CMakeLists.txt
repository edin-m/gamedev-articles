set(PROJ_NAME texture-101)

if (USE_DEVIL)

  set(SOURCES main.cpp)

  add_executable(${PROJ_NAME} ${SOURCES})
  target_link_libraries(${PROJ_NAME} srclib)

  message("Will build ${PROJ_NAME}")

else()
  message("DevIL required for ${PROJ_NAME}")
endif()
