option(BUILD_ALL_DESKTOP_PROJECTS "Build all desktop projects - takes a long time" OFF)

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

option(BUILD_ALURE_101
  "Build alure 101 example (alure-101)"
  OFF)

option(BUILD_EIGEN_101
  "Build eigen 101 example (eigen-101)"
  OFF)

############################################################
# separate ORs with two spaces so it's more readable
############################################################

if (BUILD_ALL_DESKTOP_PROJECTS)
  set(BUILD_PERSP_TRIANGLE ON)
  set(BUILD_TEXTURE_101 ON)
  set(BUILD_BVH_LOADER ON)
  set(BUILD_FREETYPE_101 ON)
  set(BUILD_ASSIMP_101 ON)
  set(BUILD_PHYSFS_101 ON)
  set(BUILD_PHYSFS_102 ON)
  set(BUILD_ALURE_101 ON)
  set(BUILD_EIGEN_101 ON)
endif()

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

# alure
if (
    BUILD_ALURE_101
)
  set(USE_AUDIO_ALURE ON CACHE BOOL "" FORCE)
endif()

# eigen
if (
    BUILD_EIGEN_101
)
  set(USE_EIGEN ON CACHE BOOL "" FORCE)
endif()

######

# set zlib
#if (
#    USE_ASSIMP
#)
#  set(USE_ZLIB ON CACHE BOOL "" FORCE)
#endif()
