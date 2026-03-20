#include "light.h"




int russian_roulette(Vector* throughput, unsigned int* seed){
	float p = max(throughput->Data[0], max(throughput->Data[1], throughput->Data[2]));
	p = fmax(p, 0.05f);
	p = fmin(p, 0.95f);
	
	float epsilon = rand_r(seed) / (float)RAND_MAX;
	
	if (epsilon > p) {
		//kill path
		return 1;
	}
	for (int i = 0; i<3; ++i) {
		throughput->Data[i] /= p;
	}
	return 0;
	
}

Ray random_Ray_demi_sphere_cosine_weighted(const Vector * origin, const Vector * normal, unsigned int* seed){
	
	const float u1 = rand_r(seed) / (float)RAND_MAX;
	const float u2 = rand_r(seed) / (float)RAND_MAX;
	const float atheta = sqrtf(u1);
	const float phi = 2*M_PI*u2;
	//les coordonnées dans la base locale
	float x, y;
	y = sinf(phi);
	x = cosf(phi);
	x *= atheta;
	y *= atheta;
	const float z = sqrtf(1 - u1);
	
	//coordonnées dans la base orthonormée (up,right,forward) https://www.opengl-tutorial.org/fr/intermediate-tutorials/tutorial-13-normal-mapping/
	Vector up;
	Vector norm;
	Ray ray;
	ray.position = *origin;
	
	
	if (fabs((*normal).Data[1])<1.0f-EPS){
		create_vector_ext(&up, 0, 1, 0);
	}
	else{
		create_vector_ext(&up, 1, 0, 0);
	}
	
	Vector tangent;
	cross_ext(normal, &up, &tangent);
    norm_ext(&tangent, &tangent);
	
	Vector bitangent;
	cross_ext(&tangent, normal, &bitangent);
	
	mul_ext(&tangent, x, &tangent);
	mul_ext(&bitangent, y, &bitangent);
	mul_ext(normal, z, &norm);
	
	add_ext(&tangent, &bitangent, &ray.direction);
	add_ext(&ray.direction, &norm, &ray.direction);
	
	return ray;
}

void ray_sampling(Ray *r, const Scene * S, int dmax, Vector * radiance, unsigned int* seed){
	Vector throughput = {1.f, 1.f, 1.f};
	Ray current_ray = *r;
	
	for (int i = 0; i < 3; ++i){
			radiance->Data[i] = 0.0f;
	}
	
	for (int d = 0; d<dmax; ++d) {
		
		Vector n;
		Vector hit;
		int object = -1;
		if (!intersect_in_scene(&current_ray, S, &object, &hit, &n)) {
			for (int i = 0; i < 3; ++i) {
				radiance->Data[i] += throughput.Data[i] * S->background_color->Data[i];
			}
			return;
		}
		
		if (dot(&n, &current_ray.direction) > 0.0f) {
			mul_ext(&n, -1.0f, &n);
		}
		
		const Primitive *obj = S->objects[object];
		const float albedo = obj->albedo;
		
		
		Vector offset_origin;
		Vector n_eps;
		mul_ext(&n, EPS, &n_eps);
		add_ext(&hit, &n_eps, &offset_origin);
		
		
		switch (obj->m_type){
			case Emissive:{
				for (int i = 0; i < 3; ++i){
					radiance->Data[i] += throughput.Data[i] * obj->color.Data[i] * albedo;
				}
				return;
			}
				
			case Lambertian:
			{
				
				if (dot(&n, &current_ray.direction) > 0.0f) {
					mul_ext(&n, -1.0f, &n);
					mul_ext(&n, 1e-4, &n_eps);
					add_ext(&hit, &n_eps, &offset_origin);
				}
				Ray r_new = random_Ray_demi_sphere_cosine_weighted(&offset_origin, &n, seed);
				for (int i = 0; i < 3; ++i){
					throughput.Data[i] *= obj->color.Data[i] * albedo;
				}
				current_ray = r_new;
				
				
				/*
				if (d > 7) {
					if (russian_roulette(&throughput, seed)) {
						return;
					}
				}
				 */
				break;
			}
			case Specular:{
				float dotn = dot(&current_ray.direction, &n);
				
				
				Vector wo;
				wo.Data[0] = current_ray.direction.Data[0] - 2.0f * dotn * n.Data[0];
				wo.Data[1] = current_ray.direction.Data[1] - 2.0f * dotn * n.Data[1];
				wo.Data[2] = current_ray.direction.Data[2] - 2.0f * dotn * n.Data[2];
				
				norm_ext(&wo, &wo);
				
				Ray r_new;
				r_new.direction = wo;
				r_new.position = offset_origin;
				
				
				for (int i = 0; i < 3; ++i){
					throughput.Data[i] *= obj->color.Data[i] * obj->albedo;
				}
				/*
				if (d > 7) {
					if (russian_roulette(&throughput, seed)) {
						return;
					}
				}
				*/
				current_ray = r_new;
				break;
			}
		}
	}
	for (int i = 0; i < 3; ++i){
			radiance->Data[i] = 0.0f;
	}
}

void path_trace(const int x1, const int y1, const int local_y, const int width, Scene const * S, const size_t bounces, float* color_buffer, unsigned int* seed)
{
	
	Ray ray;
	trace_ray(x1, y1, &S->camera, &ray);
	
	Vector radiance;
	ray_sampling(&ray, S, (int)bounces, &radiance, seed);
	
	size_t index = (local_y * width + x1) * 3;
	
	color_buffer[index+0] += radiance.Data[0];
	color_buffer[index+1] += radiance.Data[1];
	color_buffer[index+2] += radiance.Data[2];
	 
	
	return;
}

int get_bounces(void){
	char * env = getenv("BOUNCES");
	if(!env) return 26;
	else return atoi(env);
	return 0;
}


void benchmark_huge(Scene* scene, size_t width, size_t height){
	
	const float x0  =  0.0;
	const float y0  =  0;
	const float z0  =  1;
	const float fov =  50;
	material_t mat[3] = {Lambertian, Emissive, Specular};
	
	Camera cam;
	create_camera(&cam, width, height, fov, x0, y0, z0, 0, 0);
	scene->camera = cam;
	
	int amount = 900;
	int amount_s = (int)sqrtf((float)amount);
	Vector bg;
	create_vector_ext(&bg, 0, 0, 0);
	create_scene_ext(amount+1, &bg, scene);
	int dist = 10;
	float ymax = dist * tanf(25.f * M_PI / 180.0f);
	
	float xmax = ymax * width / height;
	
	Vector color = { 255, 255, 255 };
	for (int i = 0; i < amount_s; ++i){
		for (int j = 0; j < amount_s; ++j){
			
			
			float u = (float)i / (amount_s-1);
			float v = (float)j / (amount_s-1);

			float x = -xmax + u * 2.f * xmax;
			float y = -ymax + v * 2.f * ymax;
			float albedo = (i%3 == 2) ? 3 : 0.9;
			Primitive* p = malloc(sizeof(Primitive));
			
			if (j%2 == 0){
				create_sphere(p, 0.075, x, y, -dist, mat[i%3], albedo, &color);
				add_primitive(p, scene);
			}
			else {
				create_cube(p, 0.075, x, y, -dist, mat[2-i%3], albedo, &color);
				add_primitive(p, scene);
			}
		}
	}
	
	Vector color_bg = {200.f,200.f,200.f};
	Primitive* p = malloc(sizeof(Primitive));
	create_cube(p, dist*2+0.5, 0, 0, 0, Lambertian, 1, &color_bg);
	add_primitive(p, scene);
}

void benchmark_big(Scene* scene, size_t width, size_t height){
	
	const float x0  =  0.0;
	const float y0  =  0;
	const float z0  =  1;
	const float fov =  50;
	material_t mat[3] = {Lambertian, Emissive, Specular};
	
	Camera cam;
	create_camera(&cam, width, height, fov, x0, y0, z0, 0, 0);
	scene->camera = cam;
	
	int amount = 100;
	int amount_s = (int)sqrtf((float)amount);
	Vector bg;
	create_vector_ext(&bg, 0, 0, 0);
	create_scene_ext(amount+1, &bg, scene);
	int dist = 10;
	float ymax = dist * tanf(25.f * M_PI / 180.0f);
	
	float xmax = ymax * width / height;
	
	Vector color = { 255, 255, 255 };
	for (int i = 0; i < amount_s; ++i){
		for (int j = 0; j < amount_s; ++j){
			
			
			float u = (float)i / (amount_s-1);
			float v = (float)j / (amount_s-1);

			float x = -xmax + u * 2.f * xmax;
			float y = -ymax + v * 2.f * ymax;
			float albedo = (i%3 == 2) ? 3 : 0.9;
			Primitive* p = malloc(sizeof(Primitive));
			
			if (j%2 == 0){
				create_sphere(p, 0.075, x, y, -dist, mat[i%3], albedo, &color);
				add_primitive(p, scene);
			}
			else {
				create_cube(p, 0.075, x, y, -dist, mat[2-i%3], albedo, &color);
				add_primitive(p, scene);
			}
		}
	}
	
	Vector color_bg = {200.f,200.f,200.f};
	Primitive* p = malloc(sizeof(Primitive));
	create_cube(p, dist*2+0.5, 0, 0, 0, Lambertian, 1, &color_bg);
	add_primitive(p, scene);
}


void benchmark1(Scene* scene, size_t width, size_t height){
	
	const float x0  =  0.0;
	const float y0  =  0.0;
	const float z0  =  0.0;
	const float fov =  50;
	
	Camera cam;
	create_camera(&cam, width, height, fov, x0, y0, z0, 0, 0);
	scene->camera = cam;
	
	Vector bg;
	create_vector_ext(&bg, 230, 230, 230);
	create_scene_ext(11, &bg, scene);
	
	Primitive*  p1  = malloc(sizeof(Primitive));
	Primitive*  p2  = malloc(sizeof(Primitive));
	Primitive*  p3  = malloc(sizeof(Primitive));
	Primitive*  p4  = malloc(sizeof(Primitive));
	Primitive*  p5  = malloc(sizeof(Primitive));
	Primitive*  p6  = malloc(sizeof(Primitive));
	Primitive*  p7  = malloc(sizeof(Primitive));
	Primitive*  p8  = malloc(sizeof(Primitive));
	Primitive*  p9  = malloc(sizeof(Primitive));
	Primitive*  p10 = malloc(sizeof(Primitive));
	Primitive*  p11 = malloc(sizeof(Primitive));
	
	Vector head  = {255, 220, 180};
	Vector black = {60, 60.0, 60.0};
	Vector white = {255.0, 255.0, 255.0};
	create_sphere(p1, 0.5, 0, 0, -1.5, Lambertian, 1, &head);
	add_primitive(p1, scene);
	
	create_sphere(p2, 0.30, -0.45, 0.45, -1.6, Lambertian, 0.9, &black);
	add_primitive(p2, scene);
	
	create_sphere(p3, 0.30,  0.45, 0.45, -1.6, Lambertian, 0.9, &black);
	add_primitive(p3, scene);
	
	create_sphere(p4, 0.10, -0.18, 0.12, -1.03, Lambertian, 0.9, &white);
	add_primitive(p4, scene);
	
	create_sphere(p5, 0.06, -0.18, 0.10, -0.99, Lambertian, 0.9, &black);
	add_primitive(p5, scene);
	
	create_sphere(p6, 0.06, 0.18, 0.10, -0.99, Lambertian, 0.9, &black);
	add_primitive(p6, scene);
	
	create_sphere(p7, 0.10, 0.18, 0.12, -1.03, Lambertian, 0.9, &white);
	add_primitive(p7, scene);
	
	create_sphere(p8, 0.10, 0.0, -0.02, -0.98, Lambertian, 0.9, &black);
	add_primitive(p8, scene);
	
	Vector cheeks_color = {255, 150, 150};
	create_sphere(p9, 0.07, -0.30, -0.05, -1.09, Lambertian, 0.9, &cheeks_color);
	add_primitive(p9, scene);
	
	create_sphere(p10, 0.07, 0.30, -0.05, -1.09, Lambertian, 0.9, &cheeks_color);
	add_primitive(p10, scene);
	
	Vector raybox_color;
	create_vector_ext(&raybox_color, 230, 230, 230);
	create_box(p11, 10, 0.1, 10, 0, -0.51, 0.0, Lambertian, 0.9, &raybox_color, 0, 0);
	add_primitive(p11, scene);
}

void benchmark_medium(Scene* scene, size_t width, size_t height){
	
	const float x0 = 0;
	const float y0 = 0;
	const float z0 = 1.7;
	const float fov = 50;
	
	Camera cam;
	create_camera(&cam, width, height, fov, x0, y0, z0, 0, 0);
	scene->camera = cam;
	
	Vector bg;
	create_vector_ext(&bg, 42, 230, 42);
	create_scene_ext(15, &bg, scene);
	
	const float r = 0.15;
	
	Primitive*  p1 = malloc(sizeof(Primitive));
	Primitive*  p2 = malloc(sizeof(Primitive));
	Primitive*  p3 = malloc(sizeof(Primitive));
	Primitive*  p4 = malloc(sizeof(Primitive));
	Primitive*  p5 = malloc(sizeof(Primitive));
	Primitive*  p6 = malloc(sizeof(Primitive));
	Primitive*  p7 = malloc(sizeof(Primitive));
	Primitive*  p8 = malloc(sizeof(Primitive));
	Primitive*  p9 = malloc(sizeof(Primitive));
	Primitive* p10 = malloc(sizeof(Primitive));
	Primitive* p11 = malloc(sizeof(Primitive));

	Vector raybox_color;
	create_vector_ext(&raybox_color, 230, 230, 230);
	create_box(p1, 2.85, 2, 5, 0, 0.25, 0, Lambertian, 0.9, &raybox_color, 0, 0);
	add_primitive(p1, scene);

	Vector pylone;
	create_vector_ext(&pylone, 255, 123, 123);
	create_box(p2, 0.05, 1.5, 0.05, -1.2, 0, -2, Lambertian, 0.9, &pylone, 0, 0);
	add_primitive(p2, scene);

	Vector pylone_base;
	create_vector_ext(&pylone_base, 255, 123, 123);
	create_box(p3, 0.4, 0.1, 0.4, -1.2, -0.7, -2, Lambertian, 0.9, &pylone_base, 0, 0);
	add_primitive(p3, scene);

	Vector pylone_up;
	create_vector_ext(&pylone_up, 255, 123, 123);
	create_box(p4, 0.17, 0.06, 0.17, -1.2, 0.72, -2, Lambertian, 0.9, &pylone_up, 0, 0);
	add_primitive(p4, scene);

	Vector light_bulb;
	create_vector_ext(&light_bulb, 255, 255, 255);
	create_sphere(p5, r, -1.2, 0.9, -2, Emissive, 4, &light_bulb);
	add_primitive(p5, scene);
	
	float x2 = 1.2, z2 = -2;
	
	create_vector_ext(&pylone, 123, 123, 255);
	create_box(p6, 0.05, 1.5, 0.05, x2, 0, z2, Lambertian, 0.9, &pylone, 0, 0);
	add_primitive(p6, scene);
	
	create_vector_ext(&pylone_base, 123, 123, 255);
	create_box(p7, 0.4, 0.1, 0.4, x2, -0.7, z2, Lambertian, 0.9, &pylone_base, 0, 0);
	add_primitive(p7, scene);
	
	create_vector_ext(&pylone_up, 123, 123, 255);
	create_box(p8, 0.17, 0.06, 0.17, x2, 0.72, z2, Lambertian, 0.9, &pylone_up, 0, 0);
	add_primitive(p8, scene);

	create_sphere(p9, r, x2, 0.9, z2, Emissive, 4, &light_bulb);
	add_primitive(p9, scene);

	Vector mirror;
	create_vector_ext(&mirror, 255, 255, 255);
	create_box(p10, 0.3, 1, 0.01, -1, -0.1, -0.8, Specular, 0.92, &mirror, 125, 75);
	add_primitive(p10, scene);
	
	create_box(p11, 1, 0.01, 0.3, -1.2, -0.3, -2.1, Specular, 0.92, &mirror, -10, 90);
	add_primitive(p11, scene);
}

void compute_normal(Primitive* const p, Vector* n, int is_intern, int face, Vector* hit){
	if (!p || !n) return;
	
	
	switch (p->type) {
		case SPHERE:
			*n = get_normal_vector_sphere(hit, &p->position);
			break;
		case BBOX:
			*n = get_normal_vector_box(face, is_intern);
			break;
		case BOX:
		{
			OBB *box = (OBB *)p->object;
			Vector normal;
			switch (face) {
				case MIN:    normal = box->obb_right;   mul_ext(&normal, -1, &normal); break;
				case MAX:    normal = box->obb_right;   break;
				case BOTTOM: normal = box->obb_up;      mul_ext(&normal, -1, &normal); break;
				case UP:     normal = box->obb_up;      break;
				case BACK:   normal = box->obb_direction; mul_ext(&normal, -1, &normal); break;
				case FRONT:  normal = box->obb_direction; break;
			}
			
			if (is_intern)
				mul_ext(&normal, -1.0f, &normal);
			
			*n = normal;
			break;
		}
	}
}

void ray_sampling_t(Ray* const r, object_tree_t* const scene, int dmax, Vector * radiance, unsigned int* seed, Vector* bg_color){
	Vector throughput = {1.f, 1.f, 1.f};
	Ray current_ray = *r;
	
	for (int i = 0; i < 3; ++i)
			radiance->Data[i] = 0.0f;
	
	int is_intern = 0, face = -1;
	for (int d = 0; d<dmax; ++d) {
		Vector n;
		Vector hit;
		float t = FLT_MAX;
		Primitive* obj = NULL;
		
		if (!intersect_in_tree(scene, &current_ray, &t, &obj, &is_intern, &face) || !obj) {
			for (int i = 0; i < 3; ++i) {
				radiance->Data[i] += throughput.Data[i] * bg_color->Data[i];
			}
			return;
		}
		
		linear_ext(&current_ray.position, &current_ray.direction, t, &hit);
		compute_normal(obj, &n, is_intern, face, &hit);
		
		if (dot(&n, &current_ray.direction) > 0.0f) {
			mul_ext(&n, -1.0f, &n);
		}
		const float albedo = obj->albedo;
		
		
		Vector offset_origin;
		Vector n_eps;
		mul_ext(&n, EPS, &n_eps);
		add_ext(&hit, &n_eps, &offset_origin);
		
		
		
		
		switch (obj->m_type){
			case Emissive:{
				for (int i = 0; i < 3; ++i){
					radiance->Data[i] += throughput.Data[i] * obj->color.Data[i] * albedo;
				}
				return;
			}
				
			case Lambertian:
			{
				Ray r_new = random_Ray_demi_sphere_cosine_weighted(&offset_origin, &n, seed);
				for (int i = 0; i < 3; ++i){
					throughput.Data[i] *= obj->color.Data[i] * albedo;
				}
				current_ray = r_new;
				
				
				if (d > 7) {
					if (russian_roulette(&throughput, seed)) {
						return;
					}
				}
				 
				break;
			}
			case Specular:{
				float dotn = dot(&current_ray.direction, &n);
				
				
				Vector wo;
				wo.Data[0] = current_ray.direction.Data[0] - 2.0f * dotn * n.Data[0];
				wo.Data[1] = current_ray.direction.Data[1] - 2.0f * dotn * n.Data[1];
				wo.Data[2] = current_ray.direction.Data[2] - 2.0f * dotn * n.Data[2];
				
				norm_ext(&wo, &wo);
				
				Ray r_new;
				r_new.direction = wo;
				r_new.position = offset_origin;
				
				
				for (int i = 0; i < 3; ++i){
					throughput.Data[i] *= obj->color.Data[i] * obj->albedo;
				}
				
				if (d > 7) {
					if (russian_roulette(&throughput, seed)) {
						return;
					}
				}
				
				current_ray = r_new;
				break;
			}
		}
	}
	for (int i = 0; i < 3; ++i){
			radiance->Data[i] = 0.0f;
	}
}

void path_trace_t(const int x1, const int y1, const int local_y, const int width, Scene const * S, const size_t bounces, float* color_buffer, unsigned int* seed, object_tree_t* const tree)
{
	
	Ray ray;
	trace_ray(x1, y1, &S->camera, &ray);
	
	Vector radiance;
	ray_sampling_t(&ray, tree, (int)bounces, &radiance, seed, S->background_color);
	
	size_t index = (local_y * width + x1) * 3;
	
	color_buffer[index+0] += radiance.Data[0];
	color_buffer[index+1] += radiance.Data[1];
	color_buffer[index+2] += radiance.Data[2];
	 
	
	return;
}
