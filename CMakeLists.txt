cmake_minimum_required(VERSION 3.15)

project(ppm LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(SOURCES src/image/image.c src/image/image.h)

add_library(img SHARED src/image/image.c)

add_library(vector SHARED src/vector/vector.c src/tools/tools.c)
target_link_libraries(vector PRIVATE m)
target_include_directories(vector PRIVATE ${PROJECT_SOURCE_DIR}/src)

add_library(ray SHARED src/ray/ray.c src/tools/tools.c)
target_link_libraries(ray PUBLIC vector)
target_include_directories(ray PRIVATE ${PROJECT_SOURCE_DIR}/src)


add_executable(ppm src/ppm.c ${SOURCES})
target_link_libraries(ppm PRIVATE img ray m)
target_compile_options(ppm PRIVATE -O3 -Wall -march=native)
target_include_directories(ppm PRIVATE ${PROJECT_SOURCE_DIR}/src)

add_executable(test_runner 
                            test/test_runner.c 
                            test/test_vector.c 
                            test/test_ray.c
                            test/test_tools.c
)
target_compile_options(test_runner PRIVATE -g -Wall -O3 -march=native)
target_include_directories(test_runner PRIVATE ${PROJECT_SOURCE_DIR}/src)

include(FetchContent)
FetchContent_Declare(
    unity
    GIT_REPOSITORY  https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG         v2.6.1
    GIT_SHALLOW TRUE
)

FetchContent_GetProperties(unity)
if(NOT unity_POPULATED)
    FetchContent_MakeAvailable(unity)
endif()

target_link_libraries(test_runner PRIVATE unity vector ray)

add_custom_target(test test_runner
    DEPENDS $<TARGET_FILE:test_runner>
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running unit tests..."
)