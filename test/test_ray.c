#include "ray.h"
#include "unity.h"
#include "float.h"

#define TOLERANCE 1e-2

void test_create_ray(void)
{
    // pos = -5, -5
    // dir = -3, -3;
    srand(time(NULL));

    const float pos_x = generate_random_value(FLT_MAX, FLT_MIN);
    const float pos_y = generate_random_value(FLT_MAX, FLT_MIN);
    const float pos_z = generate_random_value(FLT_MAX, FLT_MIN);

    const float dir_x = generate_random_value(FLT_MAX, FLT_MIN);
    const float dir_y = generate_random_value(FLT_MAX, FLT_MIN);
    const float dir_z = generate_random_value(FLT_MAX, FLT_MIN);

    const float default_0[3] = {0.0f, 0.0f, 0.0f};
    const float default_1[3] = {0.0f, 0.0f, 1.0f};

    const float pos[3] = {pos_x, pos_y, pos_z};
	Vector dir;
	create_vector_ext(&dir, dir_x, dir_y, dir_z);
	norm_ext(&dir, &dir);

    Ray* r0 = create_ray_default();
    Ray* r1 = create_ray(pos_x, pos_y, pos_z, dir_x, dir_y, dir_z);

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(default_0, r0->position.Data, 3);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(default_1, r0->direction.Data, 3);

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(pos, r1->position.Data, 3);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(dir.Data, r1->direction.Data, 3);

    free_ray(r0);
    free_ray(r1);
}

void test_ray_create_sphere(void)
{
    srand(time(NULL));

    const float pos_x = generate_random_value(FLT_MAX, FLT_MIN);
    const float pos_y = generate_random_value(FLT_MAX, FLT_MIN);
    const float pos_z = generate_random_value(FLT_MAX, FLT_MIN);

    const float r = generate_random_value(FLT_MAX, FLT_MIN);
    
    const float default_0[3] = {0.0f, 0.0f, 0.0f};

    const float pos[3] = {pos_x, pos_y, pos_z};

    Sphere* r0 = create_sphere_default();
	Sphere* r1 = malloc_check(sizeof(Sphere));
	create_sphere(r1,pos_x, pos_y, pos_z, r);

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(default_0, r0->position.Data, 3);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, r0->radius);

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(pos, r1->position.Data, 3);
    TEST_ASSERT_EQUAL_FLOAT(r, r1->radius);

    free_sphere(r0);
    free_sphere(r1);
}

void test_intersect_sphere(void)
{
    const float ox0 = 0.0f;
    const float oy0 = 0.0f;
    const float oz0 = 0.0f;

    const float dx0 = 0.0f;
    const float dy0 = 0.0f;
    const float dz0 = 1.0f;

    const float px0 = 0.0f;
    const float py0 = 0.0f;
    const float pz0 = 2.0f;

    const float r0 = 1;

	const float result_00[3] = {0, 0, 1};

    Ray* ray0 = create_ray(ox0, oy0, oz0, dx0,dy0,dz0);
	Sphere* spr0 =malloc_check(sizeof(Sphere));
	create_sphere(spr0,px0, py0, pz0, r0);

    Vector points = intersect_sphere(ray0, spr0);
	
	

    TEST_ASSERT_FLOAT_ARRAY_WITHIN(TOLERANCE,result_00, points.Data, 3);
	
	free_ray(ray0);
	
	ray0 = create_ray(1, oy0, oz0, dx0,dy0,dz0);
	
	points = intersect_sphere(ray0, spr0);
	const float result_01[3] = {1, 0, 2};
	
	TEST_ASSERT_FLOAT_ARRAY_WITHIN(TOLERANCE,result_01, points.Data, 3);
	
	free_ray(ray0);
	free_sphere(spr0);
	
}
