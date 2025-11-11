#include "ray.h"
#include "vector/vector.h"

Ray* create_ray_default()
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

    return ray;
}

Sphere* create_sphere(const float x, const float y, const float z, const float rad)
{
    Sphere* sphere = (Sphere*)malloc_check(sizeof(Sphere));
    create_vector_ext(&sphere->position,x, y, z);
    sphere->radius = rad;

    return sphere;
}

Sphere* create_sphere_default()
{
    Sphere* sphere = (Sphere*)malloc_check(sizeof(Sphere));
    create_vector_ext(&sphere->position,0.0f, 0.0f, 0.0f);
    sphere->radius = 2.0f;

    return sphere;
}


Vector* intersect_sphere(const Ray* const r, const Sphere* const s)
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

    const float A = dot(&r->direction,&r->direction);
    const float B = 2.0f * dot(&r->position,&r->direction) - 2.0f * dot(&r->direction,&s->position);
    const float C = dot(&r->position,&r->position) - dot(&s->position,&s->position) - s->radius*s->radius;

    Quadratic_info* quad = quadratic_resolution(A, B, C);
    Vector* solutions = NULL;
    Vector u0;
    Vector u1;

    if(quad == NULL)return solutions;
    switch (quad->state)
    {
    case ONE_SOLUTION:
        solutions = (Vector*)malloc_check(1 * sizeof(Vector));
        mul_ext(&r->direction, quad->x0,&u0);
        add_ext(&r->position, &u0, &solutions[0]);
        break;
    case TWO_SOLUTION:
        solutions = (Vector*)malloc_check(2 *sizeof(Vector));
        mul_ext(&r->direction, quad->x0,&u0);
        add_ext(&r->position, &u0, &solutions[0]);

        mul_ext(&r->direction, quad->x1,&u1);
        add_ext(&r->position, &u1, &solutions[1]);
        break;
        
    default:
        break;
    }
    return solutions;

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
