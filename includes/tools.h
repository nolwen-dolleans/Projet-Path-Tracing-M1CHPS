#ifndef TOOL_H
#define TOOL_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

/**
 * @brief State of quadratic resolution
 * @var TWO_SOLUTION two solution available
 *
 * @var ONE_SOLUTION one solution available
 */
typedef enum QUADRATIC_STATE
{
    TWO_SOLUTION,
    ONE_SOLUTION,
}QUADRATIC_STATE;


/**
 * @brief Quadratic solution info
 * @var x0 solution one
 * @var x1 solution two
 */
typedef struct Quadratic_info
{
    QUADRATIC_STATE state;
    float x0;
    float x1;
}Quadratic_info;

/**
 * @brief Linear solution info
 * @var x0 solution one
 */
typedef struct Linear_info
{
    float x0;
}Linear_info;

/**
 * @brief Alloc memory and check if it's succed
 * @param size Size of the memory to alloc
 * @return memory allocated
 */
void* malloc_check(const size_t size);

/**
 * @brief Generate random single precision scalar
 * @param max Max value
 * @param min Min value
 * @return a single precision scalar
 */
const float generate_random_value(const float max, const float min);

/**
 * @brief Resolve a quadratic equation
 * @param a a
 * @param b b
 * @param c c
 * @return Quadratic solution info
 */
Quadratic_info* quadratic_resolution(const float a, const float b, const float c);

/**
 * @brief Resolve a linear equation
 * @param a a
 * @param b b
 * @return Linear solution info
 */
Linear_info* linear_resolution(const float a, const float b);


#endif
