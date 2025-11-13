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
#include <math.h>
#include "../image/image.h"
#include "../ray/ray.h"
#include "../vector/vector.h"
#include "../tools/tools.h"


typedef struct Scene{}Scene;  //will be put inside of scene.h
typedef struct Object{}Object;


extern Vector* get_intersection(Vector * position,Ray* r,Scene * S);

/**
 * @brief  recursive path-tracing algorithm to compute a single sample with Lambertian materials.
 * @param eye origine of the incomming ray
 * @param r incident ray
 * @param S pointer of the current scene
 * @param d current number of bounces, initialized to 0
 * @param dmax maximum bounces number
 * @return color of the pixels at the eye position
 */
uint24_t * ray_sampling(Vector * eye, Ray * r, Scene * S, int d, int dmax);


/**
 * @brief make a random ray emeted in the point origin
 * @param origin origin of the emeted ray
 * @return Ray emited
 */
struct Ray * random_Ray(Vector * origin);


float BRDF(Ray * r_i, Ray * r_e, int object);

#endif /* BRDF_h */
