#include "ray/ray.h"
#include "image/image.h"

#include "unity.h"

#define TOLERANCE 1e-2


void test_create_sphere()
{
    srand(time(NULL));

    const float pos_x = generate_random_value(__FLT32_MAX__, __FLT32_MIN__);
    const float pos_y = generate_random_value(__FLT32_MAX__, __FLT32_MIN__);
    const float pos_z = generate_random_value(__FLT32_MAX__, __FLT32_MIN__);

    const float r = generate_random_value(__FLT32_MAX__, __FLT32_MIN__);
    
    const float pos[3] = {pos_x, pos_y, pos_z};

    Sphere sph;
    create_sphere(&sph, pos_x, pos_y, pos_z, r);


    TEST_ASSERT_EQUAL_FLOAT_ARRAY(pos, sph.position.Data, 3);
    TEST_ASSERT_EQUAL_FLOAT(r, sph.radius);
}

void test_sphere_intersection()
{

}