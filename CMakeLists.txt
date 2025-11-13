cmake_minimum_required(VERSION 3.15)

project(ppm LANGUAGES C)

add_subdirectory(${PROJECT_SOURCE_DIR}/src)

set(CMAKE_C_STANDARD 11)

target_compile_options(ppm PRIVATE -O3 -Wall -march=native)

add_executable(test_runner 
                            test/test_runner.c 
                            test/test_vector.c 
                            test/test_ray.c
                            test/test_tools.c
)
target_compile_options(test_runner PRIVATE -g -Wall -O3 -march=native)

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

target_link_libraries(test_runner PRIVATE unity LibPathTracing)

add_custom_target(test test_runner
    DEPENDS $<TARGET_FILE:test_runner>
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running unit tests..."
)
