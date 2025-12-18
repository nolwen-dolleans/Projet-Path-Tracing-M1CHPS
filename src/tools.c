#include "tools.h"

void* malloc_check(const size_t size)
{
    void* data = malloc(size);

    if(data == NULL)
    {
        fprintf(stderr, "Failed to alloc memory.\n");
        exit(EXIT_FAILURE);
    }

    return data;
}

const float generate_random_value(const float max, const float min)
{
    const float delta = max - min;
    const float RMAX = 1.0f/RAND_MAX;

    const float random_value = ((float)(rand())*RMAX) * delta + min;
    
    return random_value;
}

Quadratic_info* quadratic_resolution(const float a, const float b, const float c)
{
    Quadratic_info* quad = NULL;

    const float delta = b * b - 4.0f * a * c;

    if(delta < 0) return quad;
    else if(delta == 0.0f)
    {
        quad = (Quadratic_info*)malloc_check(sizeof(Quadratic_info));
        quad->state = ONE_SOLUTION;
        quad->x0 = -b/(2.0f*a);
        quad->x1 = 0.0f; 
    }
    else
    {
        quad = (Quadratic_info*)malloc_check(sizeof(Quadratic_info));
        quad->state = TWO_SOLUTION;
        quad->x0 = (-b - sqrtf(delta))/(2.0f*a);
        quad->x1 = (-b + sqrtf(delta))/(2.0f*a);
    }

    return quad;
} 


Linear_info* linear_resolution(const float a, const float b)
{
    Linear_info* lin = NULL;

    if(a == 0.0f)
        return lin;

    lin = (Linear_info*)malloc_check(sizeof(Linear_info));
    lin->x0 = -b/a;
	
	return lin;
}


void swap(float* const restrict a, float* const restrict b)
{
    float c = *a;
    *a = *b;
    *b = c;
}

float max(const float* const a, const float* const b)
{
    return *a < *b ? *b : *a;
}

float min(const float* const a, const float* const b)
{
    return *a > *b ? *b : *a;
}

float radian(const float degree)
{
    // pi/180 * degree
    return 0.01745329251f * degree;
}

float degree(const float radian)
{
    // 180/pi * radian
    return 57.2957795131f * radian;
}
