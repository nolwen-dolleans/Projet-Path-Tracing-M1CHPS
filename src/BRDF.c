//
//  BRDF.c
//  Projet PPN
//
//  Created by Nolwen Dolléans on 12/11/2025.
//


#include "BRDF.h"

uint32_t * ray_sampling(Vector * eye, Ray * r, Scene * S, int d, int dmax){
	uint32_t  * color = malloc_check(sizeof(uint32_t));
	struct Vector * O = create_vector_default();
	
	if (d == dmax) {
		color = S->background_color; 			// par défaut met en noir si le rayon est hors-limite
		return color;
	}
									 
	O = intersect_in_scene(eye,r,S);				// O le point d'intersection du rayon sur l'objet et i le type d'objet rencontré
	
	Ray * r_new = random_Ray(O); 		// créé un rebond sur la zone d'intersection
	uint32_t  * color_i = ray_sampling(O,r_new,S,d+1,dmax);
	
	*color = *color_i * *BRDF(r, r_new, 1)/2/M_PI;
	return color;
}

