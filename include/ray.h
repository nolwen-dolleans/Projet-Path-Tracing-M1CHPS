#ifndef RAY_H
#define RAY_H

#include "vector.h"

/**
 * @brief 3D ray
 * @param t Parametric variable
 * @param position Position/Start of the ray
 * @param direction Direction/Endpoint of the ray
 */
typedef struct Ray
{
    float t;
    Vector position;
    Vector direction;
}Ray;

/**
 * @brief A 3D Sphere
 * @param radius Radius
 * @param position Position of the sphere
 */
typedef struct Sphere
{
    float radius;
    Vector position;
}Sphere;

/**
 * @brief Create 3D ray
 * @return A 3D ray
 */
Ray* create_ray_default();

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
Sphere* create_sphere_default();

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
 * @param sphere Sphere
 * @return Set of points
 */
Vector* intersect_sphere(const Ray* const r, const Sphere* const s);

/**
 * @brief Free ray
 * @param r Ray
 */
void free_ray(Ray* r);

/**
 * @brief Free sphere
 * @param r Ray
 */
void free_sphere(Sphere* s);


#endif
