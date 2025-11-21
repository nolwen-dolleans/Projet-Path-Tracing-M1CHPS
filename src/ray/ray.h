#ifndef RAY_H
#define RAY_H

#include "vector/vector.h"
#include "tools/light.h"
#include <inttypes.h>

// Left face
#define MIN 0
// RIght face
#define MAX 1

// Back face
#define BACK 2
// Front face
#define FRONT 3

// Up face
#define UP 4
// Bottom face
#define BOTTOM 5

/**
 * @brief A 3D Sphere
 * @param radius Radius
 * @param position Position of the sphere
 */
typedef struct Sphere
{
    float radius;
    Vector position;
    Vector color;
}Sphere;

/**
 * @brief A 3D AAB
 * @param min Minimum coordinate
 * @param max Maximum coordinate
 * @param color color faces
 */
typedef struct AABB
{
    Vector min;
    Vector max;
    uint32_t color[6];
    uint8_t color_hit_r;
    uint8_t color_hit_g;
    uint8_t color_hit_b;

}AABB;

/**
 * @brief Camera
 * @param Up Up vector
 * @param Right Right vector
 * @param position Camera position/ray position
 * @param fov Filed of View
 * @param width Width
 * @param height Height
 * @param ray Up vector
 */
typedef struct Camera
{
    Vector up;
    Vector right;
    Vector position;
    Vector direction;
    Vector inv_direction;
    float fov;
    size_t width;
    size_t height;
    float inv_width;
    float inv_height;
    Vector ray;
}Camera;

/**
 * @brief Create a camera at some position with up, right and front vector
 * @param cam Camera to set up
 * @param width Width screen
 * @param height Height screen
 * @param fov Field of view
 * @param x0 Position x of the camera
 * @param y0 Position y of the camera
 * @param z0 Position z of the camera
 */
void create_camera(Camera * const cam, const size_t width, const size_t height, const float fov, const float x0, const float y0, const float z0);

/**
 * @brief Trace 3D ray from the camera
 * @param cam Camera that trace the ray
 * @param width Screen Width
 * @param height Screen Height
 * @param x0 position x
 * @param y0 position y
 * @param z0 position z
 * @param x1 direction x
 * @param y1 direction y
 * @param z1 direction z
 */
void trace_ray(Camera * const cam, const float x1, const float y1);

/**
 * @brief Create a 3D sphere
 * @param sph Sphere to create
 * @param x position at x-axis
 * @param y position at y-axis
 * @param z position at z-axis
 * @param rad Radius
 */
void create_sphere(Sphere* const sph ,const float x, const float y, const float z, const float rad);

/**
 * @brief Compute the intersection of a camra ray and a sphere
 * @param cam Camera that trace the ray
 * @param sph Sphere
 * @return If there is an intersection
 */
bool sphere_intersection(const Camera* const cam, Sphere* const sph);

/**
 * @brief Create an AABB ray box
 * @param box Ray box to setup
 * @param xmin xmin
 * @param ymin ymin
 * @param xmax xmax
 * @param ymax ymax
 * @param color_min color of the left face
 * @param color_max color of the right face
 * @param color_back color of the back face
 * @param color_front color of the front face
 * @param color_bottom color of the bottom face
 * @param color_up color of the up face
 */
void create_ray_box(AABB * const box, const uint32_t color_min, const uint32_t color_max, const uint32_t color_back, const uint32_t color_front, const uint32_t color_bottom, const uint32_t color_up);

/**
 * @brief Compute the intersection of a camra ray and a AABB box
 * @param cam Camera that trace the ray
 * @param box AABB box
 * @return If there is an intersection
 */
bool box_intersection(const Camera* const cam, AABB* const box);

#endif
