#ifndef RAY_H
#define RAY_H

#include "vector.h"
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief 3D ray
 * @var position Position/Start of the ray
 * @var direction Direction/Endpoint of the ray
 */
typedef struct Ray
{
    Vector position;
    Vector direction;
}Ray;

/**
 * @brief A 3D AAB
 * @var min Minimum coordinate
 * @var max Maximum coordinate
 * @var color color faces
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
 * @var Up Up vector
 * @var Right Right vector
 * @var position Camera position/ray position
 * @var fov Filed of View
 * @var width Width
 * @var height Height
 * @var ray Up vector
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
	Ray ray;
}Camera;

/**
 * @brief A 3D Sphere
 * @var radius Radius
 * @var position Position of the sphere
 */
typedef struct Sphere
{
    float radius;
    Vector position;
	Vector color;
	bool emitted;
	float albedo;
}Sphere;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
 * @brief Create 3D ray with (0,0,0) as origin and direction
 * @param ray 3D ray
 */
void create_ray_default_ext(Ray * ray);

/**
 * @brief Create 3D ray with (x0,y0,z0) as origin and (x1,y1,z1) as direction
 * @param x0 position x
 * @param y0 position y
 * @param z0 position z
 * @param x1 direction x
 * @param y1 direction y
 * @param z1 direction z
 * @param ray A 3D ray
 */
void create_ray_ext(Ray * ray, const float x0, const float y0, const float z0, const float x1, const float y1, const float z1);

/**
 * @brief Create random 3D ray from a position
 * @param Origin Ray 3D position
 * @return A 3D ray with random direction
 */
Ray random_Ray(Vector const * Origin);

/**
 * @brief Create a 3D sphere
 * @param sph Sphere to create
 * @param x position at x-axis
 * @param y position at y-axis
 * @param z position at z-axis
 * @param rad Radius
 * @param color vector rgb with r,g,b in [0,255]
 * @param is_emitted bool if the sphere emitt or not
 * @param albedo reprense the albedo of the sphere, or the light intensity for an emitted one
 */
void create_sphere(Sphere* sph ,const float x, const float y, const float z, const float rad, const Vector * color, bool is_emitted, const float albedo);

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

/**
 * @brief Compute the intersection of a ray and a sphere
 * @param r Ray
 * @param s Sphere
 * @return Set of points
 */
bool intersect_sphere(const Ray* const r, const Sphere* const s, Vector *hit);

/**
 * @brief Compute the sphere normal vector at a point
 * @param point pointer of the point
 * @param s pointer of sphere
 * @return pointer to the normal vector
 */
Vector get_normal_vector(const Vector * point, const Sphere * s);

/**
 * @brief Free ray
 * @param r Ray
 */
void free_ray(Ray* r);

/**
 * @brief Free sphere
 * @param s Sphere
 */
void free_sphere(Sphere* s);


#endif
