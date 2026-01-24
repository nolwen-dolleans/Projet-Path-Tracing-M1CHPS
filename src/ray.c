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

void create_sphere(Sphere* sph ,const float radius)
{
	sph->radius = radius;
}

void create_box(AABB* box, float xmin, float ymin, float zmin, float xmax, float ymax, float zmax)
{
	
	box->bmin.Data[0] = xmin;
	box->bmin.Data[1] = ymin;
	box->bmin.Data[2] = zmin;
	
	box->bmax.Data[0] = xmax;
	box->bmax.Data[1] = ymax;
	box->bmax.Data[2] = zmax;
	
}

bool intersect_sphere(Ray* const r, Vector *center, float radius, Vector *hit)
{
	Vector oc;
	sub_ext(&r->position, center, &oc);
	

	const float A = dot(&r->direction, &r->direction);
	const float B = 2.0f * dot(&r->direction, &oc);
	const float C = dot(&oc, &oc) - radius * radius;

	const float delta = B*B - 4.0f*A*C;

	if (delta < 0.0f)
		return false;

	float sqrt_delta = sqrtf(delta);
	float t0 = (-B - sqrt_delta) / (2.0f * A);
	float t1 = (-B + sqrt_delta) / (2.0f * A);

	float t = -1.0f;

	if (t0 > EPS)
		t = t0;
	else if (t1 > EPS)
		t = t1;
	else
		return false; // return false if each intersection point is negative

	linear_ext(&r->position, &r->direction, t, hit);
	return true;
}
/*
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
}*/


bool intersect_box(Ray* const r, const AABB* const box, Vector *hit, int *face, int *is_intern) {

	float tmin = -INFINITY;
	float tmax = INFINITY;
	int enterAxis = -1;
	int exitAxis = -1;

	for (int i = 0; i < 3; ++i) {
		float *origin = &r->position.Data[i];
		float *direction = &r->direction.Data[i];
		float *minA = &box->bmin.Data[i];
		float *maxA = &box->bmax.Data[i];

		if (fabsf(*direction) < EPS) {
			if (*origin < *minA || *origin > *maxA) return false;
			continue;
		}

		float invD = 1.0f / *direction;
		float t1 = (*minA - *origin) * invD;
		float t2 = (*maxA - *origin) * invD;
		int axisEnterCandidate = i;
		int axisExitCandidate  = i;

		if (t1 > t2) {
			float tmp = t1;
			t1 = t2;
			t2 = tmp;
		}

		if (t1 > tmin) {
			tmin = t1;
			enterAxis = axisEnterCandidate;
		}
		if (t2 < tmax) {
			tmax = t2;
			exitAxis  = axisExitCandidate;}

		if (tmin > tmax) return false;
	}

	float tHit;
	bool inside = false;

	// Si l'origine est à l'intérieur de la box
	if (tmin < EPS && tmax > EPS) {
		tHit = tmax;
		*is_intern = 1;
	}
	else if (tmin > EPS) {
		tHit = tmin;
		*is_intern = 1;
	}
	else {
		return false;
	}

	linear_ext(&r->position, &r->direction, tHit, hit);

	int axis = (*is_intern == 1) ? exitAxis : enterAxis;
	float dir = r->direction.Data[axis];
	switch (axis) {
		case 0: *face = (dir > 0) ? (inside ? MAX : MIN) : (inside ? MIN : MAX); break;
		case 1: *face = (dir > 0) ? (inside ? UP : BOTTOM) : (inside ? BOTTOM : UP); break;
		case 2: *face = (dir > 0) ? (inside ? FRONT : BACK) : (inside ? BACK : FRONT); break;
	}
	
	return true;
}

Vector get_normal_vector_sphere(const Vector * point, const Vector * center){
	Vector n;
	sub_ext(point, center, &n);
	norm_ext(&n,&n);
	return n;
}


Vector get_normal_vector_box(const Vector * point, const AABB * box, int *face, int is_intern){
	if (*face<0 || *face>5) exit(EXIT_FAILURE);

	Vector n = {0};

	switch (*face) {
		case MIN:    n.Data[0] = -1; break;
		case MAX:    n.Data[0] =  1; break;
		case BOTTOM: n.Data[1] = -1; break;
		case UP:     n.Data[1] =  1; break;
		case BACK:   n.Data[2] = -1; break;
		case FRONT:  n.Data[2] =  1; break;
	}

	if(is_intern) mul_ext(&n, -1.0, &n);

	return n;
}
/*
Vector get_normal_vector_(const Vector * point, const Primitive * p){

	Vector defaults;
	switch (p->type)
	{
	case SPHERE:
		return get_normal_vector(point, (Sphere*)p->subStruct);
	case BOX:
	default:
		break;
	}
	create_vector_default_ext(&defaults);
	return defaults;
}
*/
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

