set(PROJ_NAME freetype-101)

if (USE_FREETYPE_GL)

  set(SOURCES
      ${EXT_LIB_DIR}/freetype-gl/demos/mat4.c
      ${EXT_LIB_DIR}/freetype-gl/demos/shader.c
      main.cpp
    )

  set(DATA
    shaders/v3f-t2f-c4f.frag
    shaders/v3f-t2f-c4f.vert
  )

  add_executable(${PROJ_NAME} ${SOURCES})
  target_link_libraries(${PROJ_NAME} srclib)

  set(SHADERS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
  set(SHADERS_DST ${EXECUTABLE_OUTPUT_PATH}/data/${PROJ_NAME}/shaders)

  add_custom_command(TARGET ${PROJ_NAME}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADERS_DST}
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${SHADERS_SRC} ${SHADERS_DST}
  )

  message("Will build ${PROJ_NAME}")

else()
  message("Freetype required for ${PROJ_NAME}")
endif()
