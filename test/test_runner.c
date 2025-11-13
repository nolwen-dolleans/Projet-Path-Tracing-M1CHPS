#include "unity.h"
// vector tests
extern void test_create_vector(void);

extern void test_sum_vector(void);
extern void test_sub_vector(void);

extern void test_mul_vector(void);
extern void test_dot_vector(void);
extern void test_cross_vector(void);

extern void test_length_vector(void);
extern void test_normalize_vector(void);

// tools tests
extern void test_linear_resolution(void);
extern void test_quadratic_resolution(void);

// ray tests 
extern void test_create_ray(void);
extern void test_ray_create_sphere(void);
extern void test_intersect_sphere(void);

void setUp(void) {
}
void tearDown(void) {}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_create_vector);

    RUN_TEST(test_sum_vector);
    RUN_TEST(test_sub_vector);

    RUN_TEST(test_mul_vector);
    RUN_TEST(test_dot_vector);
    RUN_TEST(test_cross_vector);

    RUN_TEST(test_length_vector);
    RUN_TEST(test_normalize_vector);

    RUN_TEST(test_linear_resolution);
    RUN_TEST(test_quadratic_resolution);

    RUN_TEST(test_create_ray);
    RUN_TEST(test_ray_create_sphere);
    RUN_TEST(test_intersect_sphere);


    UNITY_END();
}
