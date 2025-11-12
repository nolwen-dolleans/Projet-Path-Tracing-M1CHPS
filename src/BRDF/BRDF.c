//
//  BRDF.c
//  Projet PPN
//
//  Created by Nolwen Dolléans on 12/11/2025.
//


#include "BRDF.h"


uint24_t * ray_sampling(struct * vector e, const Ray * r, const struct Scene * S, const int d, const int dmax){
	uint24_t  * color = malloc(uint8_t*3);
	
	if (d == dmax) {
		set_color_24bit(color,0,0,0); 			// par défaut met en noir si le rayon est hors-limite
		return color;
	}
	e = get_intersection(e, r, S);				// par défaut met en noir si le rayon est hors-limite
}

