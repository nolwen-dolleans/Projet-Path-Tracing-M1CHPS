#include "ray.h"
#include "vector/vector.h"


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
    const float fov = tanf(cam->fov * (PI / 180.0f) * 0.5f);
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

void create_sphere(Sphere* const sph ,const float x, const float y, const float z, const float rad)
{
    create_vector_ext(&sph->position,x, y, z);
    sph->radius = rad;
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
        
        phong_light(0.1, &nnormal,&lightColor, &hit_point,&lightPos,&color_in,&sph->color);

        free(quad);
        return true;
        break;
    default:
        break;
    }
    return false;
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