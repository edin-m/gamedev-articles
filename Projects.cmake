# Define which projects to build
option(BUILD_PERSP_TRIANGLE
  "Build perspective triangle example (persp-triangle)"
  ON)

option(BUILD_TEXTURE_101
  "Build texture 101 example (texture-101)"
  OFF)

option(BUILD_BVH_LOADER
  "Build BVH loader example (bvh-loader)"
  OFF)

option(BUILD_FREETYPE_101
  "Build freetype-gl 101 example (freetype-101)"
  OFF)

option(BUILD_ASSIMP_101
  "Build assimp 101 example (assimp-101)"
  OFF)

option(BUILD_PHYSFS_101
  "Build PhysFS 101 example (physfs-101)"
  OFF)

option(BUILD_PHYSFS_102
  "Build PhysFS 102 example (physfs-102)"
  OFF)

############################################################
# separate ORs with two spaces so it's more readable
############################################################

# GLFW - by default: on
set(USE_GLFW ON CACHE BOOL "" FORCE)

# DevIL
if (
  BUILD_TEXTURE_101  OR  BUILD_ASSIMP_101  OR  BUILD_PHYSFS_101 OR
  BUILD_PHYSFS_102
)
  set(USE_DEVIL ON CACHE BOOL "" FORCE)
endif()

# freetype-gl
if (
  BUILD_FREETYPE_101
)
  set(USE_FREETYPE_GL ON CACHE BOOL "" FORCE)
endif()

# assimp
if (
    BUILD_ASSIMP_101  OR  BUILD_PHYSFS_102
)
  set(USE_ASSIMP ON CACHE BOOL "" FORCE)
endif()

# PhysFS
if (
    BUILD_PHYSFS_101  OR  BUILD_PHYSFS_102
)
  set(USE_PHYSFS ON CACHE BOOL "" FORCE)
endif()

######

# set zlib
#if (
#    USE_ASSIMP
#)
#  set(USE_ZLIB ON CACHE BOOL "" FORCE)
#endif()
