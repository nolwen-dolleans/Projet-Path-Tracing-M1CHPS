#ifndef RAY_H
#define RAY_H

#include "vector.h"

/**
 * @brief 3D ray
 * @var t Parametric variable
 * @var position Position/Start of the ray
 * @var direction Direction/Endpoint of the ray
 */
typedef struct Ray
{
    float t;
    Vector position;
    Vector direction;
}Ray;

/**
 * @brief A 3D Sphere
 * @var radius Radius
 * @var position Position of the sphere
 */
typedef struct Sphere
{
    float radius;
    Vector position;
	uint32_t color;
}Sphere;

/**
 * @brief Create 3D ray
 * @return A 3D ray
 */
Ray* create_ray_default(void);

/**
 * @brief Create 3D ray
 * @param x0 position x
 * @param y0 position y
 * @param z0 position z
 * @param x1 direction x
 * @param y1 direction y
 * @param z1 direction z
 * @return A 3D ray
 */
Ray* create_ray(const float x0, const float y0, const float z0, const float x1, const float y1, const float z1);

/**
 * @brief Create a 3D sphere
 * @return A 3D sphere
 */
Sphere* create_sphere_default(void);

/**
 * @brief Create random 3D ray from a position
 * @param Origin Ray 3D position
 * @return A 3D ray with random direction
 */
Ray* random_Ray(Vector const * Origin);

/**
 * @brief Create a 3D sphere
 * @param x position at x-axis
 * @param y position at y-axis
 * @param z position at z-axis
 * @param rad Radius
 * @return A 3D sphere
 */
Sphere* create_sphere(const float x, const float y, const float z, const float rad);

/**
 * @brief Compute the intersection of a ray and a sphere
 * @param r Ray
 * @param s Sphere
 * @return Set of points
 */
Vector* intersect_sphere(const Ray* const r, const Sphere* const s);

/**
 * @brief Compute the sphere normal vector at a point
 * @param point pointer of the point
 * @param s pointer of sphere
 * @return pointer to the normal vector
 */
Vector* get_normal_vector(const Vector * point, const Sphere * s);

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
