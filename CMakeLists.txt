cmake_minimum_required(VERSION 3.15)

project(ppm LANGUAGES C)
set(CMAKE_C_STANDARD 11)

<<<<<<< HEAD
=======
find_package(MPI REQUIRED)
>>>>>>> a6707ad (Initial commit on MPI branch)

add_library(LibPathTracing
    src/image.c
    src/ray.c
    src/scene.c
    src/tools.c
    src/vector.c
    src/light.c
)

set(COMMON_COMPILE_FLAGS
    $<$<CONFIG:Debug>:-Wall -Wextra -g -O0 -fno-inline -fno-omit-frame-pointer>
    $<$<CONFIG:Release>:-O3 -march=native -Wno-all>
)

target_include_directories(LibPathTracing PUBLIC ${PROJECT_SOURCE_DIR}/includes)

target_link_libraries(LibPathTracing m)

<<<<<<< HEAD
add_executable(ppm src/ppm.c
    src/image.c
    src/ray.c
    src/scene.c
    src/tools.c
    src/vector.c
    src/light.c)
target_link_libraries(ppm PRIVATE LibPathTracing)
=======
add_executable(ppm src/ppm.c)
target_link_libraries(ppm PRIVATE LibPathTracing MPI::MPI_C)
>>>>>>> a6707ad (Initial commit on MPI branch)


target_compile_options(ppm PRIVATE ${COMMON_COMPILE_FLAGS})

add_executable(test_runner
                            test/test_runner.c 
                            test/test_vector.c 
                            test/test_ray.c
                            test/test_tools.c
                            test/test_scene.c
<<<<<<< HEAD
                            src/scene.c
                            src/ray.c
=======
>>>>>>> a6707ad (Initial commit on MPI branch)
)
target_compile_options(test_runner PRIVATE ${COMMON_COMPILE_FLAGS})

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

message(STATUS "Build type = ${CMAKE_BUILD_TYPE}")
