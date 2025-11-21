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
    Quadratic_info* info = NULL;

    const float delta = b * b - 4.0f * a * c;

    if(delta < 0) return info;
    else if(delta == 0.0f)
    {
        info = (Quadratic_info*)malloc_check(sizeof(Quadratic_info));
        info->state = ONE_SOLUTION;
        info->x0 = -b/(2.0f*a);
        info->x1 = 0.0f; 
    }
    else
    {
        info = (Quadratic_info*)malloc_check(sizeof(Quadratic_info));
        info->state = TWO_SOLUTION;
        info->x0 = (-b + sqrtf(delta))/(2.0f*a);
        info->x1 = (-b - sqrtf(delta))/(2.0f*a);
    }

    return info;
} 


Linear_info* linear_resolution(const float a, const float b)
{
    Linear_info* info = NULL;

    info = (Linear_info*)malloc_check(sizeof(Linear_info));
    info->x0 = -b/a;

    return info;
}