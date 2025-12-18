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

//https://www.mathematik.uni-marburg.de/~thormae/lectures/graphics1/code/ImportanceSampling/index.html?utm_source=chatgpt.com
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
	norm_ext(&bitangent, &bitangent);
	
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


Vector ray_sampling(Ray * r, const Scene * S, const Camera * cam, int d, int dmax){
	Vector L_incident;                         //ceci va représenter la réflectance du rayon incident avec RGB comme composante
	create_vector_default_ext(&L_incident);
	Vector hit;
	Vector black;
   create_vector_ext(&black, 0, 0, 0);
	int object = -1;
	
	if (d == dmax) {
		return black;// par défaut met en noir si le rayon à fait un certain nombre de rebond
	}			// O le point d'intersection du rayon sur l'objet et object l'objet rencontré
	if (intersect_in_scene(r, S, &object, &hit) == false) {
		if (d==0) {
			return *S->background_color;// par défaut met la couleur de fond si le rayon est hors-limite;
		}
		else{
			return black;// permet d'éviter au ciel d'avoir une influance sur la couleur des objets
		}
			
	}
	
	Vector n = get_normal_vector(&hit, S->objects[object]);

	// Assurer une normale unitaire et orientée à l’opposé du rayon incident
	norm_ext(&n, &n);
	if (dot(&n, &r->direction) > 0.0f) {
		// la normale pointe du mauvais côté pour l’hémisphère sortant
		n.Data[0] = -n.Data[0];
		n.Data[1] = -n.Data[1];
		n.Data[2] = -n.Data[2];
	}
	Sphere obj = *S->objects[object];
	
	if (obj.emitted) {
		Vector res;
		float intensity = obj.albedo;
		mul_ext(&obj.color, intensity, &res);
		return res;
	}

	Vector offset_origin;
	Vector n_eps;
	mul_ext(&n, 1e-3f, &n_eps); // n_eps = n * 1e-3
	add_ext(&hit, &n_eps, &offset_origin);
	
	Ray r_new = random_Ray_demi_sphere_cosine_weighted(&offset_origin, &n);
	
	const float albedo = obj.albedo;
	Vector L_reflected_i = ray_sampling(&r_new, S, cam, d+1, dmax);
	const float cos_theta = dot(&n,&r_new.direction);
	Vector weight;
	mul_ext(&obj.color, albedo, &weight);

	for (int i = 0; i < 3; ++i) {
		L_incident.Data[i] += L_reflected_i.Data[i] * weight.Data[i];
	}
	
	return L_incident;
}

Vector path_trace(Camera * const cam, const size_t pixel_x, const size_t pixel_y, Scene const * S, size_t N)
{
 	Ray ray;
	trace_ray(pixel_x, pixel_y, cam->width, cam->height, cam->fov, &ray);

	int col = 0;	

	Vector color;
	create_vector_ext(&color,col, col, col);
	
	
	for(size_t i = 0; i<N; ++i){
		Vector radiance = ray_sampling(&ray, S, cam, 0, 8);
		for(int j = 0; j<3; ++j){
			color.Data[j] += radiance.Data[j];
		}
	}
	float inv_N = (float)1/(N+1) * 255;
	for (int i = 0; i<3; ++i) {
		
		color.Data[i] = color.Data[i]*inv_N;
		/*
		if (color.Data[i]>255) {
			perror("Error: color > 255\n");
			exit(1);
		}*/
	}
	return color;
}

Vector ray_sampling_(Ray * r, const Scene_ * S, const Camera * cam, int d, int dmax){
	Vector L_incident;                         //ceci va représenter la réflectance du rayon incident avec RGB comme composante
	create_vector_default_ext(&L_incident);
	Vector hit;
	Vector black;
   create_vector_ext(&black, 0, 0, 0);
	bool check = false;
	int object = -1;
	if (d == dmax) {
		return black;// par défaut met en noir si le rayon à fait un certain nombre de rebond
	}			// O le point d'intersection du rayon sur l'objet et object l'objet rencontré
	if (!intersect_in_scene_(r, S, &object, &hit)) {
		if (d==0) {
			return *S->background_color;// par défaut met la couleur de fond si le rayon est hors-limite;
		}
		else{
			return black;// permet d'éviter au ciel d'avoir une influance sur la couleur des objets
		}
			
	}
	
	Vector n = get_normal_vector_(&hit, &S->objects[object]);

	// Assurer une normale unitaire et orientée à l’opposé du rayon incident
	norm_ext(&n, &n);
	if (dot(&n, &r->direction) > 0.0f) {
		// la normale pointe du mauvais côté pour l’hémisphère sortant
		n.Data[0] = -n.Data[0];
		n.Data[1] = -n.Data[1];
		n.Data[2] = -n.Data[2];
	}
	Primitive obj = S->objects[object];

	if (obj.emitted) {
		Vector res;
		float intensity = obj.albedo;
		switch (obj.type)
		{
		case SPHERE:
			mul_ext(&((Sphere*)obj.subStruct)->color, intensity, &res);
			break;
		
		case BOX:
			break;
		}
		
		return res;
	}

	// Offset the new ray origin to avoid self-intersections
	Vector offset_origin;
	Vector n_eps;
	// n_eps = n * 1e-3
	mul_ext(&n, 1e-3f, &n_eps);
	add_ext(&hit, &n_eps, &offset_origin);
	
	Ray r_new = random_Ray_demi_sphere_cosine_weighted(&offset_origin, &n);
	
	const float albedo = obj.albedo;
	Vector L_reflected_i = ray_sampling_(&r_new, S, cam, d+1, dmax);
	const float cos_theta = dot(&n,&r_new.direction);
	Vector weight;

	switch (obj.type)
	{
	case SPHERE:
		mul_ext(&((Sphere*)obj.subStruct)->color,  albedo, &weight);
		break;
	
	case BOX:
		break;
	}

	for (int i = 0; i < 3; ++i) {
		if (check) {
			printf("%f ",L_incident.Data[i]);
		}
		L_incident.Data[i] += L_reflected_i.Data[i] * weight.Data[i];
	}if (check) {
		printf("\n");
	   }
	return L_incident;
}

Vector path_trace_(Camera * const cam, const size_t pixel_x, const size_t pixel_y, Scene_ const * S, size_t N)
{
 	Ray ray;
	trace_ray(pixel_x, pixel_y, cam->width, cam->height, cam->fov, &ray);

	int col = 0;	

	Vector color;
	create_vector_ext(&color,col, col, col);
	
	
	for(size_t i = 0; i<N; ++i){
		Vector radiance = ray_sampling_(&ray, S, cam, 0, 8);
		for(int j = 0; j<3; ++j){
			color.Data[j] += radiance.Data[j];
		}
	}
	float inv_N = (float)1/(N+1) * 255;
	for (int i = 0; i<3; ++i) {
		
		color.Data[i] = color.Data[i]*inv_N;
		/*
		if (color.Data[i]>255) {
			perror("Error: color > 255\n");
			exit(1);
		}*/
	}
	return color;
}

