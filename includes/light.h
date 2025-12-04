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

#define N 1000


/**
 * @brief  recursive path-tracing algorithm to compute a single sample with Lambertian materials.
 * @param r incident ray
 * @param S pointer of the current scene
 * @param d current number of bounces
 * @param dmax maximum bounces number
 * @return color of the pixels at the e object
 */
Vector ray_sampling(Ray * r, const Scene * S, int d, int dmax);

Ray random_Ray_demi_sphere(Vector * origin, Vector * normal);

Vector convert_to_vect(const uint24_t * rgb);

/**
 * @brief  Lambertan BRDF equation
 * @param ray indident ray
 * @param normal pointer to the normal vector
 * @return color of the pixels at the e object
 */
float BRDF_lambertian(Ray const * ray, Vector const * normal, Sphere const * s, float const albedo);

void phong_light(const float ambient_str, Vector* normal, const Vector* color_light, Vector* hitPoint, Vector* lightPos, const Vector* color_in, Vector* color_out);



/**
 * @brief Trace 3D ray from the camera with Path Tracing
 * @param pixel_x index x of the pixel
 * @param pixel_y index y of the pixel
 * @param S the scene of objects
 * @return color the color of the pixel
 */
Vector path_trace(Camera * const cam, const float pixel_x, const float pixel_y, Scene const * S);

/**
 * @brief Set a simple diffuse point light at a given position with RGB intensity.
 * @param position World position of the light
 * @param intensity RGB intensity (radiant intensity)
 */
void add_diffuse_point_light(const Vector* position, const Vector* intensity);

/**
 * @brief Accumulate diffuse contribution from the configured point light to a shading point if visible.
 * @param point Shading point position
 * @param normal Shading normal (will be normalized internally)
 * @param albedo Diffuse albedo scalar in [0,1]
 * @param surfaceColor RGB surface color
 * @param S Scene for shadow testing
 * @param L_inout Accumulator to add the direct lighting contribution
 */
void accumulate_diffuse_point_light(const Vector* point, const Vector* normal, float albedo, const Vector* surfaceColor, const Scene* S, Vector* L_inout);

#endif /* light_h */
