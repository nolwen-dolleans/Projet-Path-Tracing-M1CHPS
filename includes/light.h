//
//  BRDF.h
//  Projet PPN
//
//  Created by Nolwen Dolléans on 12/11/2025.
//

#ifndef light_h
#define light_h

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "image.h"
#include "vector.h"
#include "scene.h"
#include "time.h"
#include "ray.h"



/**
 * @brief  recursive path-tracing algorithm to compute a single sample with Lambertian materials.
 * @param r incident ray
 * @param S pointer of the current scene
 * @param d current number of bounces
 * @param dmax maximum bounces number
 * @return color of the pixels at the e object
 */
Vector ray_sampling(Ray * r, const Scene * S, const Camera * cam, int d, int dmax);

/**
 * @brief Trace 3D ray from the camera with Path Tracing with N samples
 * @param pixel_x index x of the pixel
 * @param pixel_y index y of the pixel
 * @param S the scene of objects
 * @param N number of samples
 * @return color the color of the pixel
 */
Vector path_trace(Camera * const cam, const size_t pixel_x, const size_t pixel_y, Scene const * S, size_t N);



/**
 * @brief  recursive path-tracing algorithm to compute a single sample with Lambertian materials.
 * @param r incident ray
 * @param S pointer of the current scene
 * @param d current number of bounces
 * @param dmax maximum bounces number
 * @return color of the pixels at the e object
 */
Vector ray_sampling_(Ray * r, const Scene_ * S, const Camera * cam, int d, int dmax);

/**
 * @brief Trace 3D ray from the camera with Path Tracing with N samples
 * @param pixel_x index x of the pixel
 * @param pixel_y index y of the pixel
 * @param S the scene of objects
 * @param N number of samples
 * @return color the color of the pixel
 */
Vector path_trace_(Camera * const cam, const size_t pixel_x, const size_t pixel_y, Scene_ const * S, size_t N);


#endif /* light_h */
