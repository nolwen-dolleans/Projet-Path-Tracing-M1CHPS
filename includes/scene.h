//
//  scene.h
//  Projet PPN
//
//  Created by Nolwen Dolléans on 13/11/2025.
//

#ifndef scene_h
#define scene_h

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include "tools.h"
#include "vector.h"
#include "image.h"
#include "ray.h"

typedef struct Scene{
	Sphere ** objects;
	Vector * background_color;
	Vector * lightsources;
	size_t size_objects;
	size_t size_lightsources;
}Scene;

typedef struct Scene_{
	Primitive* objects;
	Vector* background_color;
	Vector * lightsources;
	size_t size_objects;
	size_t size_lightsources;
}Scene_;


/**
 * @brief return a pointer of the intersection point between the ray and the closer object
 * @param r pointer of the incident ray
 * @param S pointer of the scene
 * @return index of the object
 * @return intersection:  The poter to the intersection point
 */
bool intersect_in_scene(const struct Ray* const r, const Scene* const S, int * object, Vector *hit);

/**
 * @brief return a pointer of the intersection point between the ray and the closer object
 * @param r pointer of the incident ray
 * @param S pointer of the scene
 * @return index of the object
 * @return intersection:  The poter to the intersection point
 */
bool intersect_in_scene_(Ray* const r, const Scene_* const S, int * object, Vector *hit);


/**
 * @brief Free scene
 * @param S A pointer of a scene
 */
void free_scene(Scene * S);

/**
 * @brief Free scene
 * @param S A pointer of a scene
 */
void free_scene_(Scene_ * S);


/**
 * @brief Alloc a scene of n\_objects objects, nb\_lightsources light sources and with a background color
 * @param n_objects number of objects
 * @param n_lightsources number of light sources
 * @param backgroundColor a uint32_t represent the background color
 * @return Scene
 */
Scene * create_scene_ptr(size_t n_objects, size_t n_lightsources, const Vector * backgroundColor);

/**
 * @brief Alloc a scene of n\_objects objects, nb\_lightsources light sources and with a background color
 * @param n_objects number of objects
 * @param n_lightsources number of light sources
 * @param backgroundColor a uint32_t represent the background color
 * @return Scene
 */
Scene_ * create_scene_ptr_(size_t n_objects, size_t n_lightsources, const Vector * backgroundColor);


#endif /* scene_h */
