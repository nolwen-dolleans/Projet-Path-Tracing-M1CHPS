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
#include "../tools/tools.h"
#include "../vector/vector.h"
#include "../ray/ray.h"


typedef struct Scene{
	Sphere **objects;
	uint32_t * background_color;
	Vector * lightsources;
}Scene;



/**
 * @brief return a pointer of the intersection point between the ray and the closer object
 * @param origin pointer of the ray origin
 * @param r pointer of the incident ray
 * @param S pointer of the scene
 * @return pointer of the closer intersection point
 */
Vector* intersect_in_scene(Vector * origin, const Ray* const r, const Scene* const S);

/**
 * @brief Free scene
 * @param S A pointer of a scene
 */
void free_scene(Scene * S);

/**
 * @brief Alloc a scene of n\_objects objects, nb\_lightsources light sources and with a background color
 * @param n_objects number of objects
 * @param nb_lightsources number of light sources
 * @param backgroundColor a uint32_t represent the background color
 * @return Scene
 */
Scene * mk_empty_scene(size_t n_objects, size_t nb_lightsources, uint32_t * backgroundColor);

#endif /* scene_h */
