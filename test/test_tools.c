#include "tools.h"
#include "unity.h"

void test_linear_resolution(void)
{
    const float a0 = 1.0f;
    const float b0 = 5.0f;
    const float x0 = -5.0f;

    const float a1 = 0.0f;
    const float b1 = 5.0f;

    const float a2 = 5.0f;
    const float b2 = 0.0f;
    const float x2 = 0.0f;

    Linear_info* info_0 = linear_resolution(a0,b0);
    Linear_info* info_1 = linear_resolution(a1,b1);
    Linear_info* info_2 = linear_resolution(a2,b2);

    TEST_ASSERT_NOT_NULL(info_0);
    TEST_ASSERT_NULL(info_1);
    TEST_ASSERT_NOT_NULL(info_2);

    TEST_ASSERT_EQUAL_FLOAT(x0, info_0->x0);
    TEST_ASSERT_EQUAL_FLOAT(x2, info_2->x0);

    free(info_0);
    free(info_2);
}

void test_quadratic_resolution(void)
{
    const float a0 = 1.0f;
    const float b0 = 5.0f;
    const float c0 = 3.0f;
    const float x0 = -4.30278f;
    const float y0 = -0.697224f;


    const float a1 = 1.0f;
    const float b1 = 2.0f;
    const float c1 = 1.0f;
    const float x1 = -1.0f;

    const float a2 = 9.0f;
    const float b2 = 8.0f;
    const float c2 = 11.0f;

    Quadratic_info* info_0 = quadratic_resolution(a0,b0,c0);
    Quadratic_info* info_1 = quadratic_resolution(a1,b1,c1);
    Quadratic_info* info_2 = quadratic_resolution(a2,b2,c2);

    TEST_ASSERT_NOT_NULL(info_0);
    TEST_ASSERT_NOT_NULL(info_1);
    TEST_ASSERT_NULL(info_2);

    TEST_ASSERT_EQUAL_UINT8(TWO_SOLUTION, info_0->state);
    TEST_ASSERT_EQUAL_UINT8(ONE_SOLUTION, info_1->state);

    TEST_ASSERT_EQUAL_FLOAT(x0, info_0->x0);
    TEST_ASSERT_EQUAL_FLOAT(y0, info_0->x1);

    TEST_ASSERT_EQUAL_FLOAT(x1, info_1->x0);

    free(info_0);
    free(info_1);

}
