
## Basic projects
if (BUILD_PERSP_TRIANGLE)
  add_subdirectory(persp-triangle)
endif()

if (BUILD_TEXTURE_101)
  add_subdirectory(texture-101)
endif()

if (BUILD_BVH_LOADER)
  add_subdirectory(bvh-loader)
endif()

if (BUILD_FREETYPE_101)
  add_subdirectory(freetype-101)
endif()

if (BUILD_ASSIMP_101)
  add_subdirectory(assimp-101)
endif()

if (BUILD_PHYSFS_101)
  add_subdirectory(physfs-101)
endif()

if (BUILD_PHYSFS_102)
  add_subdirectory(physfs-102)
endif()

#set(DESKTOP_PROJECTS
#      persp-triangle
#      texture-101
#      bvh-loader)

#foreach (proj ${DESKTOP_PROJECTS})
#  message("Adding project: ${proj}")
#  add_subdirectory(${proj})
#endforeach (proj)
