set(PROJ_NAME physfs-101)
set(SOURCES main.cpp)

add_executable(${PROJ_NAME} ${SOURCES})
target_link_libraries(${PROJ_NAME} srclib)

set(PHYSFS_101_DATA ${EXECUTABLE_OUTPUT_PATH}/data/physfs-101)

set(SHADERS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
set(SHADERS_DST
  ${PHYSFS_101_DATA}/shaders
)

set(ARCHIVE_NAME ${PROJ_NAME}-data.zip)

message("cd \"${PHYSFS_101_DATA}\" && \"${CMAKE_COMMAND}\" -E tar -cvf ${ARCHIVE_NAME} images shaders --format=zip && mv ${ARCHIVE_NAME} ..")

  execute_process(
	COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADERS_DST}
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${SHADERS_SRC} ${SHADERS_DST}
	COMMAND ${CMAKE_COMMAND} -E make_directory ${PHYSFS_101_DATA}/images
	COMMAND ${CMAKE_COMMAND} -E copy ${ROOT_DATA_DIR}/images/texture.png ${PHYSFS_101_DATA}/images/texture.png
	COMMAND sh -c "cd \"${PHYSFS_101_DATA}\" && \"${CMAKE_COMMAND}\" -E tar -cvf ${ARCHIVE_NAME} images shaders --format=zip && mv ${ARCHIVE_NAME} .."
  )

  add_custom_command(TARGET ${PROJ_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${PHYSFS_101_DATA}
  )
  