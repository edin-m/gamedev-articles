
## Basic projects
#add_subdirectory(persp-triangle)
#add_subdirectory(texture-101)

#add_subdirectory(bvh-loader)

set(DESKTOP_PROJECTS
      persp-triangle
      texture-101
      bvh-loader)

# Enable / disable projects to build
# add_subdirectory(projects/persp-triangle)
# add_subdirectory(projects/texture-101)

# add_subdirectory(projects/bvh-loader)

foreach (proj ${DESKTOP_PROJECTS})
  message("Adding project: ${proj}")
  add_subdirectory(${proj})
endforeach (proj)
