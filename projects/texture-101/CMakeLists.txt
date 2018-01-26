set(PROJ_NAME texture-101)
set(SOURCES main.cpp)

add_executable(${PROJ_NAME} ${SOURCES})
target_link_libraries(${PROJ_NAME} srclib)

set(SHADERS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
set(SHADERS_DST
  ${EXECUTABLE_OUTPUT_PATH}/data/${PROJ_NAME}/shaders
)
set(DATA_DST
  ${ROOT_DATA_DIR}/images/texture.png)

add_custom_command(TARGET ${PROJ_NAME}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADERS_DST}
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${SHADERS_SRC} ${SHADERS_DST}
  COMMAND ${CMAKE_COMMAND} -E make_directory ${EXECUTABLE_OUTPUT_PATH}/data/images
  COMMAND ${CMAKE_COMMAND} -E copy ${ROOT_DATA_DIR}/images/texture.png ${EXECUTABLE_OUTPUT_PATH}/data/images/texture.png
)
