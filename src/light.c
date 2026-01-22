//
//  BRDF.c
//  Projet PPN
//
//  Created by Nolwen Dolléans on 12/11/2025.
//

#include "light.h"

static inline Ray random_Ray_demi_sphere(Vector * origin, Vector * normal){
	Ray ray = random_Ray(origin);
	if (dot(normal, &ray.direction)<0){
		for (int i = 0; i<3; ++i) {
			ray.direction.Data[i] = -ray.direction.Data[i];
		}
	}
	return ray;
}

static inline Ray random_Ray_demi_sphere_cosine_weighted(const Vector * origin, const Vector * normal){
	const float u1 = (float)rand() / (float)RAND_MAX;
	const float u2 = (float)rand() / (float)RAND_MAX;
	const float atheta = sqrtf(u1);
	const float phi = 2*M_PI*u2;
	//les coordonnées dans la base locale
	const float x = atheta*cosf(phi);
	const float y = atheta*sinf(phi);
	const float z = sqrtf(1 - u1);
	
	//coordonnées dans la base orthonormée (up,right,forward) https://www.opengl-tutorial.org/fr/intermediate-tutorials/tutorial-13-normal-mapping/
	Vector up;
	Vector norm = *normal;
	
	
	Vector tangent;
	if (fabs(norm.Data[1])<0.9f){
		create_vector_ext(&up, 1, 0, 0);
	}
	else{
		create_vector_ext(&up, 0, 1, 0);
	}
	cross_ext(&norm, &up, &tangent);
    norm_ext(&tangent, &tangent);
	
	Vector bitangent;
	cross_ext(&norm, &tangent, &bitangent);
	
	//vecteur direction = x*tangent + y*bitangent + z*normal
	Vector direction;
	mul_ext(&tangent, x, &tangent);
	mul_ext(&bitangent, y, &bitangent);
	mul_ext(&norm, z, &norm);
	
	add_ext(&tangent, &bitangent, &direction);
	add_ext(&direction, &norm, &direction);
	
	Ray ray;
	ray.position = *origin;
	ray.direction = direction;
	norm_ext(&ray.direction, &ray.direction);
	
	return ray;
}


void ray_sampling(Ray * r, const Scene * S, const Camera * cam, int d, int dmax, Vector * radiance){
	Vector hit;
	int object = -1;
	
	if (d == dmax) {
		*radiance = black;// par défaut met en noir si le rayon à fait un certain nombre de rebond
		return;
	}			// O le point d'intersection du rayon sur l'objet et object l'objet rencontré
	if (intersect_in_scene(r, S, &object, &hit) == false) {
		if(d==0) {
			*radiance = *S->background_color;// par défaut met la couleur de fond si le rayon est hors-limite
		}
		else *radiance = *S->background_color;
		return;
	}
	
	const Sphere *obj = S->objects[object];
	const float albedo = obj->albedo;
	
	Vector n = get_normal_vector(&hit, S->objects[object]);
	
	if (obj->emitted) {
		float weight = albedo;
		mul_ext(&obj->color, weight, radiance);
		return;
	}
	
	Vector offset_origin;
	Vector n_eps;
	mul_ext(&n, 1e-3f, &n_eps); // n_eps = n * 1e-3
	add_ext(&hit, &n_eps, &offset_origin);
	
	Ray r_new = random_Ray_demi_sphere_cosine_weighted(&offset_origin, &n);
	
	Vector L_reflected_i;
	ray_sampling(&r_new, S, cam, d+1, dmax, &L_reflected_i);
	//const float cos_theta = dot(&n,&r_new.direction);
	Vector weight;
	mul_ext(&obj->color, albedo, &weight);

	for (int i = 0; i < 3; ++i) {
		radiance->Data[i] = L_reflected_i.Data[i] * weight.Data[i];
	}
	
	return;
}

Vector path_trace(Camera * const cam, const size_t pixel_x, const size_t pixel_y, Scene const * S, size_t N)
{
 	Ray ray;
	trace_ray(pixel_x, pixel_y, cam->width, cam->height, cam->fov, &ray);

	int col = 0;	

	Vector color;
	create_vector_ext(&color,col, col, col);
	Vector radiance;
	
	
	create_vector_ext(&black, 0, 0, 0);
	create_vector_ext(&white, 255, 255, 255);
	
	for(size_t i = 0; i<N; ++i){
		ray_sampling(&ray, S, cam, 0, 8, &radiance);
		for(int j = 0; j<3; ++j){
			color.Data[j] += radiance.Data[j];
		}
	}
	float inv_N = (float)1/(N+1) * 255;
skip:
	for (int i = 0; i<3; ++i) {
		
		color.Data[i] = color.Data[i]*inv_N;
	}
	
	return color;
}

