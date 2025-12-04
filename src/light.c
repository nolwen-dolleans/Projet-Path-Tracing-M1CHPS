//
//  BRDF.c
//  Projet PPN
//
//  Created by Nolwen Dolléans on 12/11/2025.
//


#include "light.h"

Ray random_Ray_demi_sphere(Vector * origin, Vector * normal){
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
	create_vector_ext(&up, 0, 1, 0);
	
	Vector tangent;
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
	if (dot(&ray.direction, &ray.direction)<0){
		for (short int i = 0; i<3; i++) {
			ray.direction.Data[i] *= -1;
		}
	}
	
	return ray;
}


Vector convert_to_vect(const uint24_t * rgb){
	Vector result;
	create_vector_ext(&result, (float)rgb->byte[0], (float)rgb->byte[1], (float)rgb->byte[2]);
	return result;
}

Vector ray_sampling(Ray * r, const Scene * S, int d, int dmax){
	Vector L_incident;                         //ceci va représenter la réflectance du rayon incident avec RGB comme composante
	create_vector_default_ext(&L_incident);
	int object = -1;
	if (d == dmax) {
		Vector black;
		create_vector_ext(&black, 0, 0, 0);
		return black;// par défaut met en noir si le rayon à fait un certain nombre de rebond
	}
	Vector intersection = intersect_in_scene(r,S,&object);				// O le point d'intersection du rayon sur l'objet et object l'objet rencontré
	
	if (object < 0) {
		return convert_to_vect(&S->background_color);// par défaut met la couleur de fond si le rayon est hors-limite
	}
	Vector n = get_normal_vector(&intersection, S->objects[object]);

	accumulate_diffuse_point_light(&intersection, &n, 0.9f, &S->objects[object]->color, S, &L_incident);
	
	Ray r_new = random_Ray_demi_sphere_cosine_weighted(&intersection,&n); 		// créé un rebond sur la zone d'intersection
	
	Vector L_emitted_i = ray_sampling(&r_new,S,d+1,dmax);
	
	float albedo = 0.3f;
	float cos_teta = dot(&n,&r_new.direction);
	//float pdf = 1 / 2*M_PI;
	//float BRDF = S->objects[object]->color * albedo / M_PI;
	Vector BRDF_pdf;
	mul_ext(&S->objects[object]->color, albedo,&BRDF_pdf);
	
	for(int i = 0; i<3; ++i){
		L_emitted_i.Data[i] *= BRDF_pdf.Data[i];
		L_incident.Data[i]+= L_emitted_i.Data[i];
	}
	return L_incident;
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


Vector path_trace(Camera * const cam, const float pixel_x, const float pixel_y, Scene const * S)
{
	const float aspect_ratio = (float)cam->width * cam->inv_height;
	const float fov = tanf(cam->fov * (M_PI / 180.0f) * 0.5f);
	const float pixel_nc_x = (2.0f*(pixel_x + 0.5f) * cam->inv_width - 1.0f) * aspect_ratio * fov;
	const float pixel_nc_y = (1.0f - 2.0f*(pixel_y + 0.5f) * cam->inv_height) * fov;
	
	Ray ray;
	create_ray_ext(&ray, 0, 0, 0, pixel_nc_x, pixel_nc_y, 1);
	Vector color;
	create_vector_ext(&color,0, 0, 0);
	
	for(size_t i = 0; i<N; ++i){
		Vector radiance = ray_sampling(&ray, S, 0, 10);
		for(int j = 0; j<3; ++j){
			color.Data[j] += radiance.Data[j];
		}
	}
	for (int i = 0; i<3; ++i) {
		color.Data[i] = color.Data[i]/(float)(N);
	}
	return color;
}
