set(PROJ_NAME eigen-101)
set(SOURCES main.cpp)

add_executable(${PROJ_NAME} ${SOURCES})
target_link_libraries(${PROJ_NAME} srclib)

