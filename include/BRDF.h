//
//  BRDF.h
//  Projet PPN
//
//  Created by Nolwen Dolléans on 12/11/2025.
//

#ifndef BRDF_h
#define BRDF_h

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "../image/image.h"
#include "../ray/ray.h"
#include "../vector/vector.h"
#include "../Scene/Scene.h"

/**
 * @brief  list of all objects of the scene.
 * @param Objects array of objects
 */

extern Vector* get_intersection(const Ray* const r, Object* O);

/**
 * @brief  recursive path-tracing algorithm to compute a single sample with Lambertian materials.
 * @param e origine of the incomming ray
 * @param r incident ray
 * @param S pointer of the current scene
 * @param d current number of bounces
 * @param dmax maximum bounces number
 * @return color of the pixels at the e object
 */
uint24_t * ray_sampling(const struct vector e, const Ray * r, const struct Scene * S, const int d, const int dmax);


#endif /* BRDF_h */
