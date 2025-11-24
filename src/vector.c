#include "vector.h"
#include "float.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Vector* create_vector_default(void)
{
    Vector* v = (Vector*)malloc_check(sizeof(Vector));

    v->Data[0] = 0.0f;
    v->Data[1] = 0.0f;
    v->Data[2] = 0.0f;

    return v;
}

void create_vector_default_ext(Vector* u)
{
    u->Data[0] = 0.0f;
    u->Data[1] = 0.0f;
    u->Data[2] = 0.0f;
}

Vector* create_vector_random_default(void)
{
    Vector* v = (Vector*)malloc_check(sizeof(Vector));

    v->Data[0] = generate_random_value(FLT_MAX, FLT_MIN);
    v->Data[1] = generate_random_value(FLT_MAX, FLT_MIN);
    v->Data[2] = generate_random_value(FLT_MAX, FLT_MIN);

    return v;
}

void create_vector_random_default_ext(Vector* u)
{

    u->Data[0] = generate_random_value(FLT_MAX, FLT_MIN);
    u->Data[1] = generate_random_value(FLT_MAX, FLT_MIN);
    u->Data[2] = generate_random_value(FLT_MAX, FLT_MIN);

}

Vector* create_vector_random(const float min, const float max)
{

    Vector* v = (Vector*)malloc_check(sizeof(Vector));

    v->Data[0] = generate_random_value(max, min);
    v->Data[1] = generate_random_value(max, min);
    v->Data[2] = generate_random_value(max, min);

    return v;
}

void create_vector_random_ext(Vector* u, const float min, const float max)
{
    u->Data[0] = generate_random_value(max, min);
    u->Data[1] = generate_random_value(max, min);
    u->Data[2] = generate_random_value(max, min);
}

Vector* create_vector(const float x, const float y, const float z)
{
    Vector* v = (Vector*)malloc_check(sizeof(Vector));

    v->Data[0] = x;
    v->Data[1] = y;
    v->Data[2] = z;

    return v;
}

void create_vector_ext(Vector* u, const float x, const float y, const float z)
{
    u->Data[0] = x;
    u->Data[1] = y;
    u->Data[2] = z; 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Vector* add(const Vector* const u, const Vector* const v)
{
    Vector* w = (Vector*)malloc_check(sizeof(Vector));

    w->Data[0] = u->Data[0] + v->Data[0];
    w->Data[1] = u->Data[1] + v->Data[1];
    w->Data[2] = u->Data[2] + v->Data[2];

    return w;
}

void add_ext(const Vector* const u, const Vector* const v, Vector* w)
{
    w->Data[0] = u->Data[0] + v->Data[0];
    w->Data[1] = u->Data[1] + v->Data[1];
    w->Data[2] = u->Data[2] + v->Data[2];
}

Vector* sub(const Vector* const u, const Vector* const v)
{
    Vector* w = (Vector*)malloc_check(sizeof(Vector));

    w->Data[0] = u->Data[0] - v->Data[0];
    w->Data[1] = u->Data[1] - v->Data[1];
    w->Data[2] = u->Data[2] - v->Data[2];

    return w;
}

void sub_ext(const Vector* const u, const Vector* const v, Vector* w)
{
    w->Data[0] = u->Data[0] - v->Data[0];
    w->Data[1] = u->Data[1] - v->Data[1];
    w->Data[2] = u->Data[2] - v->Data[2];
}

Vector* mul(const Vector* const u, const float k)
{
    Vector* w = (Vector*)malloc_check(sizeof(Vector));

    w->Data[0] = u->Data[0] * k;
    w->Data[1] = u->Data[1] * k;
    w->Data[2] = u->Data[2] * k;

    return w;
}

void mul_ext(const Vector* const u, const float k, Vector* w)
{
    w->Data[0] = u->Data[0] * k;
    w->Data[1] = u->Data[1] * k;
    w->Data[2] = u->Data[2] * k;
}

float dot(const Vector* const u, const Vector* const v)
{
    const float scalar = u->Data[0] * v->Data[0] + u->Data[1] * v->Data[1] + u->Data[2] * v->Data[2]; 

    return scalar;
}

Vector* cross(const Vector* const u, const Vector* const v)
{
    Vector* w = (Vector*)malloc_check(sizeof(Vector));

    w->Data[0] = u->Data[1] * v->Data[2] - u->Data[2] * v->Data[1];
    w->Data[1] = u->Data[2] * v->Data[0] - u->Data[0] * v->Data[2];
    w->Data[2] = u->Data[0] * v->Data[1] - u->Data[1] * v->Data[0];

    return w;
}

void cross_ext(const Vector* const u, const Vector* const v, Vector* w)
{
    w->Data[0] = u->Data[1] * v->Data[2] - u->Data[2] * v->Data[1];
    w->Data[1] = u->Data[2] * v->Data[0] - u->Data[0] * v->Data[2];
    w->Data[2] = u->Data[0] * v->Data[1] - u->Data[1] * v->Data[0];
}

float length(const Vector* const u)
{
    const float square = dot(u,u);
    const float len = sqrtf(square);

    return len;
}

Vector* norm(const Vector* const u)
{
    Vector* w = (Vector*)malloc_check(sizeof(Vector));
    
    const float len = length(u);

    w->Data[0] = u->Data[0] / len;
    w->Data[1] = u->Data[1] / len;
    w->Data[2] = u->Data[2] / len;

    return w;
}

void norm_ext(const Vector* const u, Vector* w)
{
    const float len = length(u);

    w->Data[0] = u->Data[0] / len;
    w->Data[1] = u->Data[1] / len;
    w->Data[2] = u->Data[2] / len;
}

Vector* viewport(const Vector* const u, const float width, const float height, const float near, const float far)
{
    Vector* w = (Vector*)malloc_check(sizeof(Vector));

    w->Data[0] = u->Data[0]*width + width*0.5f;
    w->Data[1] = -u->Data[1]*height + height*0.5f;
    w->Data[2] = u->Data[2]*(far - near);
	
	return w;

}

void display_vector(const Vector* const u)
{
    fprintf(stdout, "[%f]\n[%f]\n[%f]\n\n", u->Data[0], u->Data[1], u->Data[2]);
}


void free_vector(Vector* u)
{
    if(u) free(u);
    else fprintf(stderr,"No need to free memory.\n");
}
