cmake_minimum_required(VERSION 3.9)

project(learn_opengl)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})


set(SOURCES src/main.cpp)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

add_executable(${PROJECT_NAME} ${SOURCES})

set(LINK_LIBS "-L/usr/local/lib -lXi -lGLEW -lGLU -lSOIL -lGL -ldrm -lXdamage -lX11-xcb -lxcb-glx -lxcb-dri2 -lxcb-dri3 -lxcb-present -lxcb-sync -lxshmfence -lglfw3 -lrt -lm -ldl -lXrandr -lXinerama -lXxf86vm -lXcursor -lXext -lXrender -lXfixes -lX11 -lpthread -lxcb -lXau -lXdmcp")

set_target_properties(${PROJECT_NAME} PROPERTIES 
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON 
    COMPILE_OPTIONS -Wall -Wextra -Wpedantic
)

target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})
# target_link_libraries(${PROJECT_NAME} GLEW::GLEW ${OPENGL_gl_LIBRARY} ${GLFW_LIBRARIES})

add_subdirectory(tests)