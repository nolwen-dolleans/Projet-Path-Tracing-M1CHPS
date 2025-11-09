#ifndef TOOL_H
#define TOOL_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>


/**
 * @brief Alloc memory and check if it's succed
 * @param size Size of the memory to alloc
 * @return memory allocated
 */
static inline void* malloc_check(const size_t size)
{
    void* data = malloc(size);

    if(data == NULL)
    {
        fprintf(stderr, "Failed to alloc memory.\n");
        exit(EXIT_FAILURE);
    }

    return data;
}



/**
 * @brief Generate random single precision scalar
 * @param max Max value
 * @param min Min value
 * @return a single precision scalar
 */
static inline const float generate_random_value(const float max, const float min)
{
    const float delta = max - min;
    const float RMAX = 1.0f/RAND_MAX;

    const float random_value = ((float)(rand())*RMAX) * delta + min;
    
    return random_value;
}

#endif