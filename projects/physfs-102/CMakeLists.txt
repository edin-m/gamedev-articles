set(PROJ_NAME physfs-102)
set(SOURCES main.cpp
  physfs_assimp_iostream.cpp
  physfs_assimp_iosystem.cpp
  physfs_devil.cpp)

add_executable(${PROJ_NAME} ${SOURCES})
target_link_libraries(${PROJ_NAME} srclib)

set(PYSFS_102_DATA ${EXECUTABLE_OUTPUT_PATH}/data/physfs-102)

set(SHADERS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
set(SHADERS_DST
  ${PYSFS_102_DATA}/shaders
)

set(ARCHIVE_NAME ${PROJ_NAME}-data.zip)

if (MSVC OR WIN32)
  execute_process(
    COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADERS_DST}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${SHADERS_SRC} ${SHADERS_DST}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${PHYSFS_101_DATA}/images
    COMMAND ${CMAKE_COMMAND} -E copy ${ROOT_DATA_DIR}/images/texture.png ${PYSFS_102_DATA}/images/texture.png
    COMMAND sh -c "cd \"${PHYSFS_101_DATA}\" && \"${CMAKE_COMMAND}\" -E tar -cvf ${ARCHIVE_NAME} images shaders --format=zip && mv ${ARCHIVE_NAME} .."
    )

  add_custom_command(TARGET ${PROJ_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${PYSFS_102_DATA}
    )
else ()
  add_custom_command(TARGET ${PROJ_NAME}
     POST_BUILD
     COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADERS_DST}
     COMMAND ${CMAKE_COMMAND} -E copy_directory ${SHADERS_SRC} ${SHADERS_DST}
     COMMAND ${CMAKE_COMMAND} -E make_directory ${PYSFS_102_DATA}/images
     COMMAND ${CMAKE_COMMAND} -E make_directory ${PYSFS_102_DATA}/objects
     COMMAND ${CMAKE_COMMAND} -E copy ${ROOT_DATA_DIR}/images/texture.png ${PYSFS_102_DATA}/images/texture.png
     COMMAND ${CMAKE_COMMAND} -E copy ${ROOT_DATA_DIR}/objects/suzanne.obj ${PYSFS_102_DATA}/objects/suzanne.obj
     COMMAND ${CMAKE_COMMAND} -E chdir ${PYSFS_102_DATA} tar "cfv" ${EXECUTABLE_OUTPUT_PATH}/data/${ARCHIVE_NAME} --format=zip images shaders objects
     COMMAND ${CMAKE_COMMAND} -E remove_directory ${PYSFS_102_DATA}
   )
endif ()
