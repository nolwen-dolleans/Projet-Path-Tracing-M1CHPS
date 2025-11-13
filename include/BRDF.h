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



/**
 * @brief  recursive path-tracing algorithm to compute a single sample with Lambertian materials.
 * @param eye origine of the incomming ray
 * @param r incident ray
 * @param S pointer of the current scene
 * @param d current number of bounces
 * @param dmax maximum bounces number
 * @return color of the pixels at the e object
 */
uint32_t * ray_sampling(Vector * eye, Ray * r, Scene * S, int d, int dmax);


uint32_t * BRDF(Ray * r1, Ray * r2, int i);

#endif /* BRDF_h */
