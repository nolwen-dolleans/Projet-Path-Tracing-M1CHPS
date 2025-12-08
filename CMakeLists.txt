cmake_minimum_required(VERSION 3.15)

project(ppm LANGUAGES C)


set(CMAKE_C_STANDARD 11)

add_library(LibPathTracing
    src/image.c
    src/ray.c
    src/scene.c
    src/tools.c
    src/vector.c
    src/light.c
)

target_include_directories(LibPathTracing PUBLIC ${PROJECT_SOURCE_DIR}/includes)

target_link_libraries(LibPathTracing m)

add_executable(ppm src/ppm.c src/light.c)
target_link_libraries(ppm PRIVATE LibPathTracing)


target_compile_options(ppm PRIVATE -O0 -Wall -march=native -g)

add_executable(test_runner 
                            test/test_runner.c 
                            test/test_vector.c 
                            test/test_ray.c
                            test/test_tools.c
                            test/test_scene.c
                            src/scene.c
                            src/ray.c
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
