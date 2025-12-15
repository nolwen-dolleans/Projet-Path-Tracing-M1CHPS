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

void trace_ray(const size_t i, const size_t j, const size_t width, const size_t height, const float angle, Ray* const r)
{
    const float inv_height = 1.0f/ height;
    const float fov = tanf(radian(angle * 0.5));

    float Pixel_x = (2.0f*i + 1.0f - width)   * inv_height * fov;
    float Pixel_y = (height - 2.0f*j - 1.0f)  * inv_height * fov;
    float Pixel_z =  -1;
	    
    r->position.Data[0] = 0.0f;
    r->position.Data[1] = 0.0f;
    r->position.Data[2] = 0.0f;

	r->direction.Data[0] = Pixel_x; 
	r->direction.Data[1] = Pixel_y; 
	r->direction.Data[2] = Pixel_z;

	norm_ext(&r->direction, &r->direction);
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

bool intersect_sphere(Ray* const r, const Sphere* const s, Vector *hit)
{
	
	Vector const oc = r->position;
	Vector const u = r->direction;
	
	Vector w;
	sub_ext(&oc,&s->position,&w);
	const float A = dot(&u,&u);
	const float B = 2.0f * dot(&u,&w);
	const float C = dot(&w,&w) - s->radius*s->radius;

	const float delta = B*B - 4.0f*A*C;

	const float EPS = 1e-6f;
	
	if(delta < 0.0f) return false;
	
	float t = -1; // t goes from -1 to 1
	float t0 = 0.0f;

	if(delta < EPS) 
		t0 = -B/(2.0f*A);
	else if(delta > EPS) 
		t0 = (-B - sqrtf(delta))/(2.0f*A);

	t = t0 > EPS ? t0 : t;

	// Sinon on calcule le point d’impact
	linear_ext(&r->position, &r->direction, t, hit);

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


bool intersect_box(Ray* const r, AABB* const box, Vector *hit)
{

	
    float tmin = (box->min.Data[0] - r->position.Data[0]) / r->direction.Data[0];
    float tmax = (box->max.Data[0] - r->position.Data[0]) / r->direction.Data[0];

    if (tmin > tmax) swap(&tmin, &tmax);

    for (size_t i = 1; i < 3; ++i) {
        float t1 = (box->min.Data[i] - r->position.Data[i]) / r->direction.Data[i];
        float t2 = (box->max.Data[i] - r->position.Data[i]) / r->direction.Data[i];
        if (t1 > t2) swap(&t1, &t2);

        tmin = max(&tmin, &t1);
        tmax = min(&tmax, &t2);

        if (tmin > tmax) return false;
    }


	linear_ext(&r->position, &r->direction, tmax, hit);

    bool test_left_face  = box->min.Data[0] == hit->Data[0];
         test_left_face = test_left_face && box->min.Data[1] <= hit->Data[1] && hit->Data[1] <= box->max.Data[1];
         test_left_face = test_left_face && box->min.Data[2] <= hit->Data[2] && hit->Data[2] <= box->max.Data[2];

    bool test_right_face  = box->max.Data[0] == hit->Data[0];
         test_right_face = test_right_face && box->min.Data[1] <= hit->Data[1] && hit->Data[1] <= box->max.Data[1];
         test_right_face = test_right_face && box->min.Data[2] <= hit->Data[2] && hit->Data[2] <= box->max.Data[2];

    bool test_up_face  = box->min.Data[0] <= hit->Data[0] && hit->Data[1] <= box->max.Data[0];
         test_up_face = test_up_face && box->max.Data[1] == hit->Data[1];
         test_up_face = test_up_face && box->min.Data[2] <= hit->Data[2] && hit->Data[2] <= box->max.Data[2];

    bool test_bottom_face  = box->min.Data[0] <= hit->Data[0] && hit->Data[1] <= box->max.Data[0];
         test_bottom_face = test_bottom_face && box->min.Data[1] == hit->Data[1];
         test_bottom_face = test_bottom_face && box->min.Data[2] <= hit->Data[2] && hit->Data[2] <= box->max.Data[2];

    bool test_back_face  = box->min.Data[0] <= hit->Data[0] && hit->Data[1] <= box->max.Data[0];
         test_back_face = test_back_face && box->min.Data[1] <= hit->Data[1] && hit->Data[1] <= box->max.Data[1];
         test_back_face = test_back_face && box->min.Data[2] == hit->Data[2];


    if(test_left_face)
		box->color[0] = 0 << 24 | 100 << 16 | 100 << 8 | 100;
    else if(test_right_face)
		box->color[1] = 0 << 24 | 100 << 16 | 100 << 8 | 100;
    else if(test_up_face)
		box->color[2] = 0 << 24 | 250 << 16 | 0 << 8 | 0;
    else if(test_bottom_face)
		box->color[3] = 0 << 24 | 0 << 16 | 255 << 8 | 0;
    else if(test_back_face)
		box->color[4] = 0 << 24 | 0 << 16 | 0 << 8 | 255;

    return true;

}

bool intersect(Ray* const r, const Primitive* const p, Vector *hit)
{
	switch (p->type)
	{
	case SPHERE:
		return intersect_sphere(r, (Sphere*)p->subStruct, hit);
	case BOX:
		return intersect_box(r, (AABB*)p->subStruct, hit);
	}
}


Vector get_normal_vector(const Vector * point, const Sphere * s){
	Vector n;
	sub_ext(point, &s->position, &n);
	norm_ext(&n,&n);
	return n;
}

Vector get_normal_vector_(const Vector * point, const Primitive * p){


	switch (p->type)
	{
	case SPHERE:
		return get_normal_vector(point, (Sphere*)p->subStruct);
	case BOX:
	default:
		break;
	}

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

