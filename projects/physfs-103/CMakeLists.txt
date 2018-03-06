set(PROJ_NAME physfs-103)
set(SOURCES main.cpp
  ../physfs-102/physfs_assimp_iostream.cpp
  ../physfs-102/physfs_assimp_iosystem.cpp
  ../physfs-102/physfs_devil.cpp
  )

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../physfs-102)

add_executable(${PROJ_NAME} ${SOURCES})
target_link_libraries(${PROJ_NAME} srclib)

set(ARCHIVE_NAME ${PROJ_NAME}-data.zip)

set(PYSFS_103_DATA ${EXECUTABLE_OUTPUT_PATH}/data/physfs-103)

add_custom_command(OUTPUT ${PROJ_NAME}-mkdirs
  COMMAND ${CMAKE_COMMAND} -E make_directory ${PYSFS_103_DATA}
  COMMAND ${CMAKE_COMMAND} -E make_directory ${PYSFS_103_DATA}/shaders
  COMMAND ${CMAKE_COMMAND} -E make_directory ${PYSFS_103_DATA}/images
  COMMAND ${CMAKE_COMMAND} -E make_directory ${PYSFS_103_DATA}/sounds
  COMMAND ${CMAKE_COMMAND} -E make_directory ${PYSFS_103_DATA}/objects
)

add_custom_command(OUTPUT ${PROJ_NAME}-copy
  DEPENDS ${PROJ_NAME}-mkdirs
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/shaders ${PYSFS_103_DATA}/shaders
  COMMAND ${CMAKE_COMMAND} -E copy ${ROOT_DATA_DIR}/images/texture.png ${PYSFS_103_DATA}/images
  COMMAND ${CMAKE_COMMAND} -E copy ${ROOT_DATA_DIR}/sounds/bird.ogg ${PYSFS_103_DATA}/sounds
  COMMAND ${CMAKE_COMMAND} -E copy ${ROOT_DATA_DIR}/objects/suzanne.obj ${PYSFS_103_DATA}/objects
)

add_custom_command(OUTPUT ${PROJ_NAME}-tar
  DEPENDS ${PROJ_NAME}-copy
  COMMAND ${CMAKE_COMMAND} -E chdir ${PYSFS_103_DATA} tar "cfv" ${EXECUTABLE_OUTPUT_PATH}/data/${ARCHIVE_NAME} --format=zip images shaders objects sounds
)

add_custom_target(${PROJ_NAME}-deps
  DEPENDS ${PROJ_NAME}-tar
  COMMAND ${CMAKE_COMMAND} -E remove_directory ${PYSFS_103_DATA}
)

add_dependencies(${PROJ_NAME} ${PROJ_NAME}-deps)

