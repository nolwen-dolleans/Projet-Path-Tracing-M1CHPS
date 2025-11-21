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
#include "ray.h"


typedef struct Scene{
	Sphere ** objects;
	Vector * background_color;
	Vector * lightsources;
	size_t size_objects;
	size_t size_lightsources;
}Scene;



/**
 * @brief return a pointer of the intersection point between the ray and the closer object
 * @param r pointer of the incident ray
 * @param S pointer of the scene
 * @return index of the object
 * @return intersection:  The poter to the intersection point
 */
Vector * intersect_in_scene(const Ray* const r, const Scene* const S, int * object);



void add_object_to_scene(Scene * S, Sphere * s);

/**
 * @brief Free scene
 * @param S A pointer of a scene
 */
void free_scene(Scene * S);

/**
 * @brief Alloc a scene of n\_objects objects, nb\_lightsources light sources and with a background color
 * @param n_objects number of objects
 * @param n_lightsources number of light sources
 * @param backgroundColor a uint32_t represent the background color
 * @return Scene
 */
Scene * create_scene_ptr(size_t n_objects, size_t n_lightsources, uint32_t * backgroundColor);

#endif /* scene_h */
