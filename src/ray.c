#include "ray.h"

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

void trace_ray(Camera * const cam, const float pixel_x, const float pixel_y)
{
	const float aspect_ratio = (float)cam->width * cam->inv_height;
	const float fov = tanf(cam->fov * (M_PI / 180.0f) * 0.5f);
	const float pixel_nc_x = (2.0f*(pixel_x + 0.5f) * cam->inv_width - 1.0f) * aspect_ratio * fov;
	const float pixel_nc_y = (1.0f - 2.0f*(pixel_y + 0.5f) * cam->inv_height) * fov;

	Vector ray0;
	Vector ray1;
	Vector ray2;

	create_vector_ext(&ray0,pixel_nc_x, pixel_nc_y, -1);
	//to_world_space(cam, &ray0, &ray1);
	norm_ext(&ray0,&ray1);
	cam->direction = ray1;

	cam->inv_direction.Data[0] = 1.0f/ray1.Data[0];
	cam->inv_direction.Data[1] = 1.0f/ray1.Data[1];
	cam->inv_direction.Data[2] = 1.0f/ray1.Data[2];

}

Ray* create_ray_default(void)
{
    Ray* ray = (Ray*)malloc_check(sizeof(Ray));
    create_vector_default_ext(&ray->position);
    create_vector_ext(&ray->direction,0.0f, 0.0f, 1.0f);

    return ray;
}

Ray* create_ray(const float x0, const float y0, const float z0, const float x1, const float y1, const float z1)
{
    Ray* ray = (Ray*)malloc_check(sizeof(Ray));
    create_vector_ext(&ray->position,x0, y0, z0);
    create_vector_ext(&ray->direction,x1, y1, z1);
	norm(&ray->direction);

    return ray;
}

Ray random_Ray(Vector const * Origin){
	Ray ray;
	ray.position = *Origin;
	for (int i = 0; i<3; ++i) {
		ray.direction.Data[i] = rand()%2;
	}
	norm(&ray.direction);
	
	return ray;
}


void create_sphere(Sphere* sph ,const float x, const float y, const float z, const float rad)
{
	create_vector_ext(&sph->position,x, y, z);
	sph->radius = rad;
}

Sphere* create_sphere_default(void)
{
    Sphere* sphere = (Sphere*)malloc_check(sizeof(Sphere));
    create_vector_ext(&sphere->position,0.0f, 0.0f, 0.0f);
    sphere->radius = 0.0f;

    return sphere;
}

bool sphere_intersection(const Camera* const cam, Sphere* const sph)
{
	Vector oc;
	sub_ext(&cam->position, &sph->position, &oc);

	const float A = dot(&cam->direction,&cam->direction);
	const float B = 2.0f * (dot(&cam->direction,&oc));
	const float C = dot(&oc,&oc) - sph->radius*sph->radius;

	Quadratic_info* quad = quadratic_resolution(A, B, C);

	if(quad == NULL)return false;

	Vector lightPos;
	Vector lightColor;
	Vector color_in;
	Vector color_ambient;
	Vector color_diffuse;

	create_vector_ext(&lightPos, 50, 200, -100);
	create_vector_ext(&lightColor, 255, 255, 255);
	create_vector_ext(&color_in, 255, 0, 0);

	float dif = 1.0f;
	switch (quad->state)
	{
	case ONE_SOLUTION:
		
		free(quad);
		return true;
		break;
	case TWO_SOLUTION:
		Vector hit_point;

		const float t = quad->x0;

		linear_ext(&cam->position, &cam->direction, t, &hit_point);
		Vector normal;
		sub_ext(&hit_point, &sph->position, &normal);
		Vector nnormal;
		
		//phong_light(0.1, &nnormal,&lightColor, &hit_point,&lightPos,&color_in,&sph->color);

		free(quad);
		return true;
		break;
	default:
		break;
	}
	return false;
}


Vector intersect_sphere(const Ray* const r, const Sphere* const s)
{
/*     const float bx2 = r->direction.Data[0]*r->direction.Data[0];
    const float by2 = r->direction.Data[1]*r->direction.Data[1];
    const float bz2 = r->direction.Data[2]*r->direction.Data[2];

    const float ax2 = r->position.Data[0]*r->position.Data[0];
    const float ay2 = r->position.Data[1]*r->position.Data[1];
    const float az2 = r->position.Data[2]*r->position.Data[2];
 
    const float xc2 = s->position.Data[0]*s->position.Data[0];
    const float yc2 = s->position.Data[1]*s->position.Data[1];
    const float zc2 = s->position.Data[2]*s->position.Data[2];

    const float r2 = s->radius*s->radius;

    const float ax_bx = 2.0f*r->position.Data[0]*r->direction.Data[0];
    const float ay_by = 2.0f*r->position.Data[1]*r->direction.Data[1];
    const float az_bz = 2.0f*r->position.Data[2]*r->direction.Data[2];

    const float bx_xc = 2.0f*r->direction.Data[0]*s->position.Data[0];
    const float by_yc = 2.0f*r->direction.Data[1]*s->position.Data[1];
    const float bz_zc = 2.0f*r->direction.Data[2]*s->position.Data[2];

    const float A = bx2 + by2 + bz2;
    const float B = ax_bx + ay_by + az_bz - bx_xc - by_yc - bz_zc;
    const float C = ax2 + ay2 + az2 - xc2 - yc2 - zc2 - r2; */
	
	Vector const * O = &r->position;
	Vector const * u = &r->direction;
	Vector * w = sub(O,&s->position);

    const float A = dot(u,u);
    const float B = 2.0f * dot(u,w);
    const float C = dot(w,w) - s->radius*s->radius;

    Quadratic_info* quad = quadratic_resolution(A, B, C);
	Vector solutions;
	create_vector_ext(&solutions, 0, 0, 0);
    Vector u0;
    Vector u1;
	if(quad == NULL)return solutions;
    switch (quad->state)
    {
    case ONE_SOLUTION:
        mul_ext(u, quad->x0,&u0);
        add_ext(O, &u0, &solutions);
        break;
    case TWO_SOLUTION:
        Vector s1;
        Vector s2;
        mul_ext(u, quad->x0,&u0);
        add_ext(O, &u0, &s1);

        mul_ext(u, quad->x1,&u1);
        add_ext(O, &u1, &s2);
		
		Vector * d1 = sub(&s1, O);
		Vector * d2 = sub(&s2, O);
		if(length(d1) < length(d2)){
			solutions = s1;
		}
		else{
			solutions = s2;
		}
			
		free_vector(d1);
		free_vector(d2);
		break;
        
    default:
        break;
    }
	free_vector(w);
    return solutions;

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
	for (int i = 0; i<3; ++i) {
		n.Data[i] = point->Data[i]-s->position.Data[i];
	}
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
