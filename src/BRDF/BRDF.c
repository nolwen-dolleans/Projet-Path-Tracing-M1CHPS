//
//  BRDF.c
//  Projet PPN
//
//  Created by Nolwen Dolléans on 12/11/2025.
//


#include "BRDF.h"


uint24_t * ray_sampling(Vector * eye, Ray * r, Scene * S, int d, int dmax){
	uint24_t  * color = malloc_check(sizeof(uint8_t) *3);
	struct Vector * O = create_vector_default();
	
	if (d == dmax) {
		set_color_24bit(color,0,0,0); 			// par défaut met en noir si le rayon est hors-limite
		return color;
	}
	O = get_intersection(eye,r,S);				// O le point d'intersection du rayon sur l'objet et i le type d'objet rencontré
	
	struct Ray * r_new = random_Ray(O);
	uint24_t  * color_i = ray_sampling(O,r_new,S,d+1,dmax);
	
	color->byte[0] = color->byte[0] + color_i->byte[0] * BRDF(r, r_new, 1)/2/M_PI; 	// 1 représente une sphère
	color->byte[1] = color->byte[1] + color_i->byte[1] * BRDF(r, r_new, 1)/2/M_PI; 	// 1 représente une sphère
	color->byte[2] = color->byte[2] + color_i->byte[2] * BRDF(r, r_new, 1)/2/M_PI; 	// 1 représente une sphère
	return color;
}

