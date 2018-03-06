set(PROJ_NAME physfs-102)
set(SOURCES main.cpp
  physfs_assimp_iostream.cpp
  physfs_assimp_iosystem.cpp
  physfs_devil.cpp)

add_executable(${PROJ_NAME} ${SOURCES})
target_link_libraries(${PROJ_NAME} srclib)

set(ARCHIVE_NAME ${PROJ_NAME}-data.zip)

set(PYSFS_102_DATA ${EXECUTABLE_OUTPUT_PATH}/data/physfs-102)

add_custom_command(OUTPUT ${PROJ_NAME}-mkdirs
  COMMAND ${CMAKE_COMMAND} -E make_directory ${PYSFS_102_DATA}
  COMMAND ${CMAKE_COMMAND} -E make_directory ${PYSFS_102_DATA}/shaders
  COMMAND ${CMAKE_COMMAND} -E make_directory ${PYSFS_102_DATA}/images
  COMMAND ${CMAKE_COMMAND} -E make_directory ${PYSFS_102_DATA}/objects
)

add_custom_command(OUTPUT ${PROJ_NAME}-copy
  DEPENDS ${PROJ_NAME}-mkdirs
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/shaders ${PYSFS_102_DATA}/shaders
  COMMAND ${CMAKE_COMMAND} -E copy ${ROOT_DATA_DIR}/images/texture.png ${PYSFS_102_DATA}/images
  COMMAND ${CMAKE_COMMAND} -E copy ${ROOT_DATA_DIR}/objects/suzanne.obj ${PYSFS_102_DATA}/objects
)

add_custom_command(OUTPUT ${PROJ_NAME}-tar
  DEPENDS ${PROJ_NAME}-copy
  COMMAND ${CMAKE_COMMAND} -E chdir ${PYSFS_102_DATA} tar "cfv" ${EXECUTABLE_OUTPUT_PATH}/data/${ARCHIVE_NAME} --format=zip images shaders objects
)

add_custom_target(${PROJ_NAME}-deps
  DEPENDS ${PROJ_NAME}-tar
  COMMAND ${CMAKE_COMMAND} -E remove_directory ${PYSFS_102_DATA}
)

add_dependencies(${PROJ_NAME} ${PROJ_NAME}-deps)

