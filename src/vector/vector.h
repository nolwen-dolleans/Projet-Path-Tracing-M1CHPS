#ifndef VECTOR_H
#define VECTOR_H

#include "../tools.h"

/**
 * @brief Single precision 3D Vector (AoS version)
 * @param Data Componants values
 */
typedef struct Vector
{
    float Data[3];
}Vector;

/**
 * @brief Create a single precision null 3D vector (AoS version)
 * @return A single precision 3D vector
 */
Vector* create_vector_default();

/**
 * @brief Create a random single precision 3D vector (AoS version)
 * @return A single precision 3D vector
 */
Vector* create_vector_random_default();

/**
 * @brief Create a random single precision 3D vector (AoS version)
 * @return A single precision 3D vector
 */
Vector* create_vector_random(const float min, const float max);

/**
 * @brief Create a single precision 3D vector (AoS version)
 * @param x x componant
 * @param y y componant
 * @param z z componant
 * @return A single precision 3D vector
 */
Vector* create_vector(const float x, const float y, const float z);



/**
 * @brief Sum of two single precision 3D vector
 * @param u A single precision 3D vector
 * @param v A single precision 3D vector
 * @return A single precision  3D vector
 */
Vector* add(const Vector* const u, const Vector* const v);

/**
 * @brief Substraction of two single precision 3D vector
 * @param u A single precision 3D vector
 * @param v A single precision 3D vector
 * @return A single precision  3D vector
 */
Vector* sub(const Vector* const u, const Vector* const v);

/**
 * @brief Multiplication of a single precision 3D vector by a single precsion scalar
 * @param u A single precision 3D vector
 * @param k A single precision scalar
 * @return A single precision  3D vector
 */
Vector* mul(const Vector* const u, const float k);

/**
 * @brief Dot product of two single precision 3D vector
 * @param u A single precision 3D vector
 * @param v A single precision 3D vector
 * @return A single precision  scalar
 */
float dot(const Vector* const u, const Vector* const v);

/**
 * @brief Cross product of two single precision 3D vector
 * @param u A single precision 3D vector
 * @param v A single precision 3D vector
 * @return A single precision  3D vector perpandicular to u and v
 */
Vector* cross(const Vector* const u, const Vector* const v);

/**
 * @brief Single precision Length of single precsion 3D vector
 * @param u A single precision 3D vector
 * @return Single precision scalar 
 */
float length(const Vector* const u);

/**
 * @brief Normalize a single precsion 3D vector
 * @param u A single single precision 3D vector
 * @return A single precision normalized 3D vector
 */
Vector* norm(const Vector* const u);

/**
 * @brief Display single precision 3D vector
 * @param u A single single precision 3D vector
 */
void display_vector(const Vector* const u);

/**
 * @brief Free single precision 3D vector
 * @param u A single single precision 3D vector
 */
void free_vector(Vector* u);

#endif