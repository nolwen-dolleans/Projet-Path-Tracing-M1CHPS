add_library(LibPathTracing
    image.c
    ray.c
    scene.c
    tools.c
    vector.c
    light.c
)

target_include_directories(LibPathTracing PUBLIC ${PROJECT_SOURCE_DIR}/includes)

target_link_libraries(LibPathTracing m)

add_executable(ppm ppm.c)
target_link_libraries(ppm PRIVATE LibPathTracing)
