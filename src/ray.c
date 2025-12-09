#include "ray.h"
#include "light.h"


void create_camera(Camera * const cam, const size_t width, const size_t height, const float fov, const float x0, const float y0, const float z0)
{
	create_vector_ext(&cam->position, x0, y0, z0);
	create_vector_ext(&cam->direction, 0.0f,0.0f,-1.0f);

	create_vector_ext(&cam->up, 0.0f, 1.0f, 0.0f);
	
	cross_ext(&cam->up, &cam->direction, &cam->right);
	norm_ext(&cam->right,&cam->right);

	cross_ext(&cam->right, &cam->direction, &cam->up);
	norm_ext(&cam->up,&cam->up);

	cam->width = width;
	cam->height = height;
	cam->fov = fov;

	cam->inv_height = 1.0f/height;
	cam->inv_width = 1.0f/width;

}

void to_world_space(Camera * const cam, Vector* u, Vector* w)
{
	w->Data[0] = cam->right.Data[0] * u->Data[0] + cam->up.Data[0] * u->Data[1] + cam->up.Data[0] * u->Data[2] - cam->position.Data[0];
	w->Data[1] = cam->right.Data[1] * u->Data[0] + cam->up.Data[1] * u->Data[1] + cam->up.Data[1] * u->Data[2] - cam->position.Data[1];
	w->Data[2] = cam->right.Data[2] * u->Data[0] + cam->up.Data[2] * u->Data[1] + cam->up.Data[2] * u->Data[2] - cam->position.Data[2];
}

void create_ray_default_ext(Ray * ray)
{
    create_vector_default_ext(&ray->position);
    create_vector_ext(&ray->direction,0.0f, 0.0f, 0.0f);
}

void create_ray_ext(Ray * ray, const float x0, const float y0, const float z0, const float x1, const float y1, const float z1)
{
    create_vector_ext(&ray->position,x0, y0, z0);
    create_vector_ext(&ray->direction,x1, y1, z1);
	norm_ext(&ray->direction,&ray->direction);

}

Ray random_Ray(Vector const * Origin){
	Ray ray;
	ray.position = *Origin;
	for (int i = 0; i<3; ++i) {
		ray.direction.Data[i] = rand();
	}
	norm_ext(&ray.direction, &ray.direction);
	
	return ray;
}


void create_sphere(Sphere* sph ,const float x, const float y, const float z, const float rad, const Vector * color, bool is_emitted, const float albedo)
{
	float inv255 = 1.0f/255.0f;
	create_vector_ext(&sph->position,x, y, z);
	sph->radius = rad;
	mul_ext(color, inv255, &sph->color);
	sph->emitted = is_emitted;
	sph->albedo = albedo;
}

bool intersect_sphere(const Ray* const r, const Sphere* const s, Vector *hit)
{
	
	Vector const oc = r->position;
	Vector const u = r->direction;
	
	Vector w;
	sub_ext(&oc,&s->position,&w);
	const float A = dot(&u,&u);
	const float B = 2.0f * dot(&u,&w);
	const float C = dot(&w,&w) - s->radius*s->radius;

	Quadratic_info* quad = quadratic_resolution(A, B, C);
	
	if(quad == NULL)return false;
	
	const float EPS = 1e-4f;
	float t = -1;
	switch (quad->state)
	{
	case ONE_SOLUTION: {
		float t0 = quad->x0;
		if (t0 > EPS) {t = t0;}
		break;
	}
	case TWO_SOLUTION: {
		float t0 = quad->x0;
		float t1 = quad->x1;

		if (t0 > t1) {
			float tmp = t0; t0 = t1; t1 = tmp;
		}
		if (t0 > EPS) {t = t0;}
		else if (t1 > EPS) {t = t1;}
		break;
	}
	default:
		break;
	}

	if (t <= 0.0f) {
		free(quad);
		return false;
	}

	// Sinon on calcule le point d’impact
	Vector tu;
	mul_ext(&r->direction, t, &tu);
	add_ext(&r->position, &tu, hit);
	free(quad);
	return true;

}

void create_ray_box(AABB * const box, const uint32_t color_min, const uint32_t color_max, const uint32_t color_back, const uint32_t color_front, const uint32_t color_bottom, const uint32_t color_up)
{
	create_vector_ext(&box->min, -100, -100, -1);
	create_vector_ext(&box->max, 200, 200, -1);

	box->color[MIN] = color_min;
	
	box->color[MAX] = color_max;
	box->color[BACK] = color_back;

	box->color[FRONT] = color_front;
	box->color[BOTTOM] = color_bottom;
	box->color[UP] = color_up;
	
	box->color_hit_r = 155;
	box->color_hit_g = 155;
	box->color_hit_b = 155;
}

bool box_intersection(const Camera* const cam, AABB* const box)
{

	const float xaBmin = box->min.Data[0] - cam->position.Data[0];
	const float yaBmin = box->min.Data[1] - cam->position.Data[1];
	const float zaBmin = box->min.Data[2] - cam->position.Data[2];
	const float xaBmax = box->max.Data[0] - cam->position.Data[0];
	const float yaBmax = box->max.Data[1] - cam->position.Data[1];
	const float zaBmax = box->max.Data[2] - cam->position.Data[2];

	const float txBmin = xaBmin * cam->inv_direction.Data[0];
	const float tyBmin = yaBmin * cam->inv_direction.Data[1];
	const float tzBmin = zaBmin * cam->inv_direction.Data[2];

	const float txBmax = xaBmax * cam->inv_direction.Data[0];
	const float tyBmax = yaBmax * cam->inv_direction.Data[1];
	const float tzBmax = zaBmax * cam->inv_direction.Data[2];

	float tmin = txBmin > tyBmin ? txBmin : tyBmin;
	tmin = tzBmin > tmin ? tzBmin : tmin;

	float tmax = txBmax > tyBmax ? txBmax : tyBmax;
	tmax = tzBmax > tmax ? tzBmax : tmax;

	if(tmin < tmax)
	{
		const float x = cam->position.Data[0] + cam->direction.Data[0]*tmin;
		const float y = cam->position.Data[1] + cam->direction.Data[1]*tmin;
		const float z = cam->position.Data[2] + cam->direction.Data[2]*tmin;

		if(box->min.Data[0] < x && x < box->max.Data[0] && box->min.Data[1] < y && y < box->max.Data[1] && box->min.Data[2] < z && z < box->max.Data[2])
		{

			box->color_hit_r = box->color[MIN] >> 0x8;
			box->color_hit_g = (box->color[MIN] & 0x00F0) >> 0x4;
			box->color_hit_b = box->color[MIN] & 0xF;
			
		}
  
		return true;
	}

	return false;
}
Vector get_normal_vector(const Vector * point, const Sphere * s){
	Vector n;
	sub_ext(point, &s->position, &n);
	norm_ext(&n,&n);
	return n;
}

void free_ray(Ray* r)
{
    if(r) free(r);
    else fprintf(stdout, "No need to free memory.\n");
}

void free_sphere(Sphere* s)
{
    if(s) free(s);
    else fprintf(stdout, "No need to free memory.\n");
}

