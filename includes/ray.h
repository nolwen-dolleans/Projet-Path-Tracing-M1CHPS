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
typedef enum PRIM_TYPE
{
	SPHERE,
	BOX
}PRIM_TYPE;

typedef enum {
	Lambertian,
	Specular,
	Emissive
} material_t;

/**
 * @brief 3D ray
 * @var position Position/Start of the ray
 * @var direction Direction/Endpoint of the ray
 */
typedef struct Ray
{
    Vector position;
    Vector direction;
	float t;
}Ray;

/**
 * @brief A 3D AAB
 * @var min Minimum coordinate
 * @var max Maximum coordinate
 * @var color color faces
 */
typedef struct AABB
{
	Vector bmin;
	Vector bmax;
}AABB;

/**
 * @brief A 3D Sphere
 * @var radius Radius
 * @var position Position of the sphere
 */
typedef struct Sphere
{
	float radius;
}Sphere;

typedef struct Primitive
{
	void * object;
	Vector position;
	Vector color;
	PRIM_TYPE type;
	material_t m_type;
	float albedo;
}Primitive;


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
}Camera;

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
 * @param direction Camera base in spacial base where the camera is looking ((0,0,-1) by default)
 */
void create_camera(Camera * const cam, const size_t width, const size_t height, const float fov, const float x0, const float y0, const float z0, Vector* direction);

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
 * @param rad Radius
 */
void create_sphere(Sphere* sph ,const float rad);

/**
 * @brief Create a 3D sphere
 * @param sph Sphere to create
 * @param rad Radius
 */
void create_box(AABB* box, float xmin, float ymin, float zmin, float xmax, float ymax, float zmax);

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
 * @param r Ray
 * @param box AABB box
 * @return If there is an intersection
 */
bool intersect_box(Ray* const r, const AABB* const box, Vector *hit, int * face, int * is_intern);

/**
 * @brief Compute the intersection of a ray and a sphere
 * @param r Ray
 * @param position Sphere position
 * @param radius Sphere radius
 * @return Set of points
 */
bool intersect_sphere(Ray* const r, Vector *position, float radius, Vector *hit);

/**
 * @brief Compute the sphere normal vector at a point
 * @param point pointer of the point
 * @param center sphere's center pointer
 * @return pointer to the normal vector
 */
Vector get_normal_vector_sphere(const Vector * point, const Vector *center);

/**
 * @brief Compute the sphere normal vector at a point
 * @param point pointer of the point
 * @param box pointer of box
 * @return pointer to the normal vector
 */
Vector get_normal_vector_box(const Vector * point, const AABB * box, int *face, int is_intern);

void trace_ray(const size_t i, const size_t j, const Camera *c, Ray* const r);


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
