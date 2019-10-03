cmake_minimum_required(VERSION 3.9)

enable_testing()
include(CTest)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

project(opengl_test)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin/tests)

add_executable(${PROJECT_NAME}
        main_test.cpp
        geometry/math_vector_test.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES
        CMAKE_CXX_STANDARD 14
        CMAKE_CXX_STANDARD_REQUIRED ON
        INCLUDE_DIRECTORIES ${GTEST_INCLUDE_DIRS}
        )

target_link_libraries(${PROJECT_NAME} GTest::GTest Threads::Threads)

add_test(${PROJECT_NAME} ${PROJECT_NAME})