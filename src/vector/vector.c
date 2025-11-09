#include "vector.h"

Vector* create_vector_default()
{
    Vector* v = (Vector*)malloc_check(sizeof(Vector));

    v->Data[0] = 0.0f;
    v->Data[1] = 0.0f;
    v->Data[2] = 0.0f;

    return v;
}

Vector* create_vector_random_default()
{
    Vector* v = (Vector*)malloc_check(sizeof(Vector));

    v->Data[0] = generate_random_value(__FLT32_MAX__, __FLT32_MIN__);
    v->Data[1] = generate_random_value(__FLT32_MAX__, __FLT32_MIN__);
    v->Data[2] = generate_random_value(__FLT32_MAX__, __FLT32_MIN__);

    return v;
}

Vector* create_vector_random(const float min, const float max)
{

    Vector* v = (Vector*)malloc_check(sizeof(Vector));

    v->Data[0] = generate_random_value(max, min);
    v->Data[1] = generate_random_value(max, min);
    v->Data[2] = generate_random_value(max, min);

    return v;
}

Vector* create_vector(const float x, const float y, const float z)
{
    Vector* v = (Vector*)malloc_check(sizeof(Vector));

    v->Data[0] = x;
    v->Data[1] = y;
    v->Data[2] = z;

    return v;
}


Vector* add(const Vector* const u, const Vector* const v)
{
    Vector* w = (Vector*)malloc_check(sizeof(Vector));

    w->Data[0] = u->Data[0] + v->Data[0];
    w->Data[1] = u->Data[1] + v->Data[1];
    w->Data[2] = u->Data[2] + v->Data[2];

    return w;
}

Vector* sub(const Vector* const u, const Vector* const v)
{
    Vector* w = (Vector*)malloc_check(sizeof(Vector));

    w->Data[0] = u->Data[0] - v->Data[0];
    w->Data[1] = u->Data[1] - v->Data[1];
    w->Data[2] = u->Data[2] - v->Data[2];

    return w;
}

Vector* mul(const Vector* const u, const float k)
{
    Vector* w = (Vector*)malloc_check(sizeof(Vector));

    w->Data[0] = u->Data[0] * k;
    w->Data[1] = u->Data[1] * k;
    w->Data[2] = u->Data[2] * k;

    return w;
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
