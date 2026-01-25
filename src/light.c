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

Ray random_Ray_demi_sphere_cosine_weighted(const Vector * origin, const Vector * normal){
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
	
	
	if (fabs(norm.Data[1])<0.999f){
		create_vector_ext(&up, 0, 1, 0);
	}
	else{
		create_vector_ext(&up, 1, 0, 0);
	}
	
	Vector tangent;
	cross_ext(&norm, &up, &tangent);
    norm_ext(&tangent, &tangent);
	
	Vector bitangent;
	cross_ext(&tangent, &norm, &bitangent);
	norm_ext(&bitangent, &bitangent);
	
	//vecteur direction = x*tangent + y*bitangent + z*normal
	Vector direction;
	mul_ext(&tangent, x, &tangent);
	mul_ext(&bitangent, y, &bitangent);
	mul_ext(&norm, z, &norm);
	
	add_ext(&tangent, &bitangent, &direction);
	add_ext(&direction, &norm, &direction);
	norm_ext(&direction, &direction);
	
	
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
	Vector n;
	if (!intersect_in_scene(r, S, &object, &hit, &n)) {
		create_vector_ext(radiance, 1.0f, 0, 1.0f);// par défaut met la couleur de fond si le rayon est hors-limite
		return;
	}
	
	const Primitive *obj = S->objects[object];
	const float albedo = obj->albedo;
	
	
	Vector offset_origin;
	Vector n_eps;
	mul_ext(&n, EPS, &n_eps);
	add_ext(&hit, &n_eps, &offset_origin);
	
	
	switch (obj->m_type){
		case Emissive:{
			float weight = albedo;
			mul_ext(&obj->color, weight, radiance);
			return;
		}
	
		case Lambertian:{
			
			if (dot(&n, &r->direction) > 0.0f) {
				mul_ext(&n, -1.0f, &n);
				mul_ext(&n, 1e-4, &n_eps);
				add_ext(&hit, &n_eps, &offset_origin);
			}
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
		case Specular:{
			float dotn = dot(&r->direction, &n);
			
			Vector wo;
			wo.Data[0] = r->direction.Data[0] - 2.0f * dotn * n.Data[0];
			wo.Data[1] = r->direction.Data[1] - 2.0f * dotn * n.Data[1];
			wo.Data[2] = r->direction.Data[2] - 2.0f * dotn * n.Data[2];
			
			Ray r_new;
			
			r_new.direction = wo;
			r_new.position = offset_origin;
			
			Vector L_reflected;
			ray_sampling(&r_new, S, cam, d + 1, dmax, &L_reflected);
			
			for (int i = 0; i < 3; ++i)
				radiance->Data[i] = L_reflected.Data[i] * obj->color.Data[i] * obj->albedo;
			
			return;
		}
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
	create_vector_ext(&white, 1, 1, 1);
	
	for(size_t i = 0; i<N; ++i){
		ray_sampling(&ray, S, cam, 0, 6, &radiance);
		for(int j = 0; j<3; ++j){
			color.Data[j] += radiance.Data[j];
		}
	}
	float inv_N = (float)1/(N) * 255;
	for (int i = 0; i<3; ++i) {
		
		color.Data[i] = color.Data[i]*inv_N;
	}
	
	return color;
}
