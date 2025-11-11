#include "ray/ray.h"
#include "unity.h"

#define TOLERANCE 1e-2

void test_create_ray()
{
    // pos = -5, -5
    // dir = -3, -3;
    srand(time(NULL));

    const float pos_x = generate_random_value(__FLT32_MAX__, __FLT32_MIN__);
    const float pos_y = generate_random_value(__FLT32_MAX__, __FLT32_MIN__);
    const float pos_z = generate_random_value(__FLT32_MAX__, __FLT32_MIN__);

    const float dir_x = generate_random_value(__FLT32_MAX__, __FLT32_MIN__);
    const float dir_y = generate_random_value(__FLT32_MAX__, __FLT32_MIN__);
    const float dir_z = generate_random_value(__FLT32_MAX__, __FLT32_MIN__);

    const float default_0[3] = {0.0f, 0.0f, 0.0f};
    const float default_1[3] = {0.0f, 0.0f, 1.0f};

    const float pos[3] = {pos_x, pos_y, pos_z};
    const float dir[3] = {dir_x, dir_y, dir_z};

    Ray* r0 = create_ray_default();
    Ray* r1 = create_ray(pos_x, pos_y, pos_z, dir_x, dir_y, dir_z);

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(default_0, r0->position.Data, 3);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(default_1, r0->direction.Data, 3);

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(pos, r1->position.Data, 3);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(dir, r1->direction.Data, 3);

    free_ray(r0);
    free_ray(r1);
}

void test_ray_create_sphere()
{
    srand(time(NULL));

    const float pos_x = generate_random_value(__FLT32_MAX__, __FLT32_MIN__);
    const float pos_y = generate_random_value(__FLT32_MAX__, __FLT32_MIN__);
    const float pos_z = generate_random_value(__FLT32_MAX__, __FLT32_MIN__);

    const float r = generate_random_value(__FLT32_MAX__, __FLT32_MIN__);
    
    const float default_0[3] = {0.0f, 0.0f, 0.0f};

    const float pos[3] = {pos_x, pos_y, pos_z};

    Sphere* r0 = create_sphere_default();
    Sphere* r1 = create_sphere(pos_x, pos_y, pos_z, r);

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(default_0, r0->position.Data, 3);
    TEST_ASSERT_EQUAL_FLOAT(2.0f, r0->radius);

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(pos, r1->position.Data, 3);
    TEST_ASSERT_EQUAL_FLOAT(r, r1->radius);

    free_sphere(r0);
    free_sphere(r1);
}

void test_intersect_sphere()
{
    const float ox0 = -4.0f;
    const float oy0 = -4.0f;
    const float oz0 = 0;

    const float dx0 = 2.0f;
    const float dy0 = 2.0f;
    const float dz0 = 0.0f;

    const float px0 = 0.0f;
    const float py0 = 0.0f;
    const float pz0 = 0.0f;

    const float r0 = 2;

    const float result_00[3] = {-1.41, -1.41, 0};
    const float result_01[3] = {1.41421, 1.41421, 0};

    Ray* ray0 = create_ray(ox0, oy0, oz0, dx0,dy0,dz0);
    Sphere* spr0 = create_sphere(px0, py0, pz0, r0);

    Vector* points = intersect_sphere(ray0, spr0);

    TEST_ASSERT_FLOAT_ARRAY_WITHIN(TOLERANCE,result_00, points[0].Data, 3);
    TEST_ASSERT_FLOAT_ARRAY_WITHIN(TOLERANCE,result_01, points[1].Data, 3);

}