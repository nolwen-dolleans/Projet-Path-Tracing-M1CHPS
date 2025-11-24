//
//  BRDF.c
//  Projet PPN
//
//  Created by Nolwen Dolléans on 12/11/2025.
//


#include "light.h"

inline Ray random_Ray_demi_sphere(Vector * origin, Vector * normal){
	Ray ray = random_Ray(origin);
	if (dot(normal, &ray.direction)<0){
		for (int i = 0; i<3; ++i) {
			ray.direction.Data[i] = -ray.direction.Data[i];
		}
	}
	return ray;
}


inline Vector * convert_to_vect(uint24_t rgb){
	Vector * result = create_vector_default();
	create_vector_ext(result, (float)rgb.byte[0], (float)rgb.byte[1], (float)rgb.byte[2]);
	return result;
}

Vector * ray_sampling(Ray * r, Scene * S, int d, int dmax){
	Vector * color = create_vector_default();                         //ceci va représenter la réflectance du rayon incident avec RGB comme composante
	int object = -1;
	if (d == dmax) {
		return convert_to_vect(S->background_color);// par défaut met en noir si le rayon à fait un certain nombre de rebond
	}
	Vector intersection = intersect_in_scene(r,S,&object);				// O le point d'intersection du rayon sur l'objet et object l'objet rencontré
	
	if (object < 0) {
		return convert_to_vect(S->background_color);// par défaut met la couleur de fond si le rayon est hors-limite
	}
	
	Vector n = get_normal_vector(&intersection, S->objects[object]);
	Ray r_new = random_Ray_demi_sphere(&intersection,&n); 		// créé un rebond sur la zone d'intersection
	
	Vector * color_i = ray_sampling(&r_new,S,d+1,dmax);
	
	float cos_teta = dot(&n,&r_new.direction);
	float albedo = 0.9f;
	//float pdf = 1 / M_PI;
	//float BRDF = S->objects[object]->color * albedo / M_PI;
	Vector BRDF_pdf = *mul(mul(&S->objects[object]->color, albedo), cos_teta);
	
	for(int i = 0; i<3; ++i){
		color_i->Data[i] *= BRDF_pdf.Data[i];
		color->Data[i]+= color_i->Data[i];
	}
	return color;
}


/*
float BRDF_lambertian(Ray const * ray, Vector const * normal, Sphere const * s, float const albedo){
	float res = s->color * albedo / M_PI;
	return res;
}
*/

void phong_light(const float ambient_str, Vector* normal, const Vector* color_light, Vector* hitPoint, Vector* lightPos, const Vector* color_in, Vector* color_out)
{
	//Ambient
	const float amb_x = ambient_str * color_light->Data[0] ;
	const float amb_y = ambient_str * color_light->Data[1] ;
	const float amb_z = ambient_str * color_light->Data[2] ;

	//Diffuse
	norm_ext(normal, normal);

	Vector lightDir0;
	Vector lightDir1;

	sub_ext(lightPos, hitPoint, &lightDir0);
	norm_ext(&lightDir0, &lightDir1);
	
	const float dt = dot(normal, &lightDir1);
	const float dif = dt < 0.0f ? 0.0f : dt;

	const float specular_str = 0.5f;

	const float x = (0 + dif)*color_in->Data[0];
	const float y = (0 + dif)*color_in->Data[1];
	const float z = (0 + dif)*color_in->Data[2];

	create_vector_ext(color_out, x, y, z);
}
