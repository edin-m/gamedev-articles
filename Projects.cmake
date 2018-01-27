# Define which projects to build
option(BUILD_PERSP_TRIANGLE
  "Build perspective triangle example (persp-triangle)"
  ON)

option(BUILD_TEXTURE_101
  "Build texture 101 example (texture-101)"
  ON)

option(BUILD_BVH_LOADER
  "Build BVH loader example (bvh-loader)"
  ON)

option(BUILD_FREETYPE_101
  "Build freetype-gl 101 example (freetype-101)"
  ON)

option(BUILD_ASSIMP_101
  "Build assimp 101 example (assimp-101)"
  ON)

############################################################
# GLFW
if (
  BUILD_PERSP_TRIANGLE  OR  BUILD_TEXTURE_101  OR  BUILD_BVH_LOADER  OR
  BUILD_FREETYPE_101  OR  BUILD_ASSIMP_101
)
  set(USE_GLFW ON)
endif()

# DevIL
if (
  BUILD_TEXTURE_101 OR BUILD_ASSIMP_101
)
  set(USE_GLFW ON)
endif()

# freetype-gl
if (
  BUILD_FREETYPE_101
)
  set(USE_GLFW ON)
endif()

# assimp
if (
    BUILD_ASSIMP_101
)
  set(USE_ASSIMP ON)
endif()
