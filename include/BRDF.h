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
#include "image.h"
#include "ray.h"
#include "vector.h"
#include "scene.h"
#include "time.h"



/**
 * @brief  recursive path-tracing algorithm to compute a single sample with Lambertian materials.
 * @param r incident ray
 * @param S pointer of the current scene
 * @param d current number of bounces
 * @param dmax maximum bounces number
 * @return color of the pixels at the e object
 */
Vector * ray_sampling(Ray * r, Scene * S, int d, int dmax);


/**
 * @brief  Lambertan BRDF equation
 * @param ray indident ray
 * @param normal pointer to the normal vector
 * @return color of the pixels at the e object
 */
float BRDF_lambertian(Ray const * ray, Vector const * normal, Sphere const * s, float const albedo);

#endif /* BRDF_h */
