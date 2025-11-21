//
//  BRDF.c
//  Projet PPN
//
//  Created by Nolwen Dolléans on 12/11/2025.
//


#include "BRDF.h"

uint32_t* ray_sampling(Ray * r, Scene * S, int d, int dmax){
	uint32_t* color = create_vector_default();
	int object = -1;
	
	
	if (d == dmax) {
		color = S->background_color; 			// par défaut met en noir si le rayon à fait un certain nombre de rebond
		return color;
	}
									 
	Vector * O = intersect_in_scene(r,S,&object);				// O le point d'intersection du rayon sur l'objet et object l'objet rencontré
	
	if (object < 0) {
		color = S->background_color; 			// par défaut met la couleur de fond si le rayon est hors-limite
		return color;
	}
	
	Vector * n = get_normal_vector(O, S->objects[object]);
	Ray * r_new = random_Ray(O); 		// créé un rebond sur la zone d'intersection
	
	Vector * color_i = ray_sampling(r_new,S,d+1,dmax);
	
	color = add(color, mul(color_i, BRDF_lambertian(r, n, S->objects[object], 0.9f)));
	free_vector(O);
	free_vector(n);
	free_ray(r_new);
	return color;
}



float BRDF_lambertian(Ray const * ray, Vector const * normal, Sphere const * s, float const albedo){
	float res = s->color * albedo * 2 * dot(normal, &ray->direction);
	return res;
}
