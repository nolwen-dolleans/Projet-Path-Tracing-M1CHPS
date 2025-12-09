#include "unity.h"
#include "vector.h"
#include "float.h"

#define TOLERANCE 1e-6


void test_create_vector(void)
{
    srand(time(NULL));

    const float vector_result_0 [3] = {0.0f, 0.0f, 0.0f};
    const float vector_result_1 [3] = {2.0f, 64.0f, 32.0f};

    const float min_max[2] = {1.6f, 12.3f};

    {
        Vector vector_test_0;
        create_vector_default_ext(&vector_test_0);
        Vector vector_test_1;
        create_vector_ext(&vector_test_1, 2.0f, 64.0f, 32.0f);
        Vector vector_test_2;
        create_vector_random_ext(&vector_test_2, min_max[0], min_max[1]);
        Vector vector_test_3;
        create_vector_random_default_ext(&vector_test_3);


        TEST_ASSERT_EQUAL_FLOAT_ARRAY(vector_result_0, vector_test_0.Data, 3);
        TEST_ASSERT_EQUAL_FLOAT_ARRAY(vector_result_1, vector_test_1.Data, 3);
        
        TEST_ASSERT_GREATER_OR_EQUAL_FLOAT(min_max[0], vector_test_2.Data[0]);
        TEST_ASSERT_LESS_OR_EQUAL_FLOAT(min_max[1], vector_test_2.Data[0]);

        TEST_ASSERT_GREATER_OR_EQUAL_FLOAT(min_max[0], vector_test_2.Data[1]);
        TEST_ASSERT_LESS_OR_EQUAL_FLOAT(min_max[1], vector_test_2.Data[1]);

        TEST_ASSERT_GREATER_OR_EQUAL_FLOAT(min_max[0], vector_test_2.Data[2]);
        TEST_ASSERT_LESS_OR_EQUAL_FLOAT(min_max[1], vector_test_2.Data[2]);

        TEST_ASSERT_GREATER_OR_EQUAL_FLOAT(FLT_MIN, vector_test_3.Data[0]);
        TEST_ASSERT_LESS_OR_EQUAL_FLOAT(FLT_MAX, vector_test_3.Data[0]);

        TEST_ASSERT_GREATER_OR_EQUAL_FLOAT(FLT_MIN, vector_test_3.Data[1]);
        TEST_ASSERT_LESS_OR_EQUAL_FLOAT(FLT_MAX, vector_test_3.Data[1]);

        TEST_ASSERT_GREATER_OR_EQUAL_FLOAT(FLT_MIN, vector_test_3.Data[2]);
        TEST_ASSERT_LESS_OR_EQUAL_FLOAT(FLT_MAX, vector_test_3.Data[2]);
    }

}

void test_sum_vector(void)
{
    srand(time(0));

    const float x_0 = generate_random_value(FLT_MAX, FLT_MIN);
    const float y_0 = generate_random_value(FLT_MAX, FLT_MIN);
    const float z_0 = generate_random_value(FLT_MAX, FLT_MIN);

    const float x_1 = generate_random_value(FLT_MAX, FLT_MIN);
    const float y_1 = generate_random_value(FLT_MAX, FLT_MIN);
    const float z_1 = generate_random_value(FLT_MAX, FLT_MIN);


    float vector_result[3] = {0.0f, 0.0f, 0.0f};

    vector_result[0] = x_0 + x_1;
    vector_result[1] = y_0 + y_1;
    vector_result[2] = z_0 + z_1;

    {
        Vector vector_test_0;
        create_vector_ext(&vector_test_0, x_0, y_0, z_0);
        Vector vector_test_1;
        create_vector_ext(&vector_test_1,x_1, y_1, z_1);

        Vector result;
        add_ext(&vector_test_0, &vector_test_1, &result);

        TEST_ASSERT_EQUAL_FLOAT_ARRAY(vector_result, result.Data, 3);
    }

}
void test_sub_vector(void)
{
    srand(time(0));

    const float x_0 = generate_random_value(FLT_MAX, FLT_MIN);
    const float y_0 = generate_random_value(FLT_MAX, FLT_MIN);
    const float z_0 = generate_random_value(FLT_MAX, FLT_MIN);

    const float x_1 = generate_random_value(FLT_MAX, FLT_MIN);
    const float y_1 = generate_random_value(FLT_MAX, FLT_MIN);
    const float z_1 = generate_random_value(FLT_MAX, FLT_MIN);


    float vector_result[3] = {0.0f, 0.0f, 0.0f};


    {
        vector_result[0] = x_0 - x_1;
        vector_result[1] = y_0 - y_1;
        vector_result[2] = z_0 - z_1;

        Vector vector_test_0;
        create_vector_ext(&vector_test_0, x_0, y_0, z_0);
        Vector vector_test_1;
        create_vector_ext(&vector_test_1,x_1, y_1, z_1);

        Vector result;
        sub_ext(&vector_test_0, &vector_test_1, &result);

        TEST_ASSERT_EQUAL_FLOAT_ARRAY(vector_result, result.Data, 3);

    }
}

void test_mul_vector(void)
{
    srand(time(0));

    const float x_0 = generate_random_value(FLT_MAX, FLT_MIN);
    const float y_0 = generate_random_value(FLT_MAX, FLT_MIN);
    const float z_0 = generate_random_value(FLT_MAX, FLT_MIN);

    const float k = generate_random_value(FLT_MAX, FLT_MIN);


    float vector_result[3] = {0.0f, 0.0f, 0.0f};

    vector_result[0] = x_0 * k;
    vector_result[1] = y_0 * k;
    vector_result[2] = z_0 * k;


    {
        Vector vector_test_0; 
        create_vector_ext(&vector_test_0, x_0, y_0, z_0);

        Vector result; 
        mul_ext(&vector_test_0, k,&result);

        TEST_ASSERT_EQUAL_FLOAT_ARRAY(vector_result, result.Data, 3);
    }
}

void test_dot_vector(void)
{
    srand(time(0));

    const float x_0 = generate_random_value(FLT_MAX, FLT_MIN);
    const float y_0 = generate_random_value(FLT_MAX, FLT_MIN);
    const float z_0 = generate_random_value(FLT_MAX, FLT_MIN);

    const float x_1 = generate_random_value(FLT_MAX, FLT_MIN);
    const float y_1 = generate_random_value(FLT_MAX, FLT_MIN);
    const float z_1 = generate_random_value(FLT_MAX, FLT_MIN);

    const float vector_result = x_0 * x_1 + y_0 * y_1 + z_0 * z_1;

	Vector* vector_test_0 = malloc_check(sizeof(Vector));
	create_vector_ext(vector_test_0,x_0, y_0, z_0);
	Vector* vector_test_1 = malloc_check(sizeof(Vector));
	create_vector_ext(vector_test_1,x_1, y_1, z_1);

    const float result = dot(vector_test_0, vector_test_1);

    TEST_ASSERT_EQUAL_FLOAT(vector_result, result);

    free_vector(vector_test_0);
    free_vector(vector_test_1);
}

void test_cross_vector(void)
{
    srand(time(0));

    const float x_0 = generate_random_value(10000, FLT_MIN);
    const float y_0 = generate_random_value(10000, FLT_MIN);
    const float z_0 = generate_random_value(10000, FLT_MIN);

    const float x_1 = generate_random_value(10000, FLT_MIN);
    const float y_1 = generate_random_value(10000, FLT_MIN);
    const float z_1 = generate_random_value(10000, FLT_MIN);
    
    float vector_result[3] = {0.0f, 0.0f, 0.0f};

    vector_result[0] = y_0 * z_1 - z_0 * y_1;
    vector_result[1] = z_0 * x_1 - x_0 * z_1;
    vector_result[2] = x_0 * y_1 - y_0 * x_1;

    {        
		Vector* vector_test_0 = malloc_check(sizeof(Vector));
		create_vector_ext(vector_test_0,x_0, y_0, z_0);
		Vector* vector_test_1 = malloc_check(sizeof(Vector));
		create_vector_ext(vector_test_1,x_1, y_1, z_1);

		Vector* result =  malloc_check(sizeof(Vector));
		cross_ext(vector_test_0, vector_test_1, result);

        TEST_ASSERT_FLOAT_ARRAY_WITHIN( TOLERANCE,vector_result, result->Data, 3);

        free_vector(vector_test_0);
        free_vector(vector_test_1);
        free_vector(result);
    }

    {
        Vector vector_test_0;
        create_vector_ext(&vector_test_0, x_0, y_0, z_0);
        Vector vector_test_1;
        create_vector_ext(&vector_test_1, x_1, y_1, z_1);

        Vector result;
        cross_ext(&vector_test_0, &vector_test_1,&result);

        TEST_ASSERT_FLOAT_ARRAY_WITHIN( TOLERANCE,vector_result, result.Data, 3);
    }
}

void test_length_vector(void)
{
    srand(time(0));

    const float x_0 = generate_random_value(FLT_MAX, FLT_MIN);
    const float y_0 = generate_random_value(FLT_MAX, FLT_MIN);
    const float z_0 = generate_random_value(FLT_MAX, FLT_MIN);

    const float vector_result = sqrtf(x_0 * x_0 + y_0 * y_0 + z_0 * z_0);

	Vector vector_test_0;
	create_vector_ext(&vector_test_0,x_0, y_0, z_0);

    const float result = length(&vector_test_0);

    TEST_ASSERT_EQUAL_FLOAT(vector_result, result);

}
void test_normalize_vector(void)
{
    srand(time(0));

    const float x_0 = generate_random_value(FLT_MAX, FLT_MIN);
    const float y_0 = generate_random_value(FLT_MAX, FLT_MIN);
    const float z_0 = generate_random_value(FLT_MAX, FLT_MIN);

    const float len = sqrtf(x_0 * x_0 + y_0 * y_0 + z_0 * z_0);

    float min_max[2] = {-1.0f, 1.0f};
    float vector_result[3] = {0.0f, 0.0f, 0.0f};

    vector_result[0] = x_0 / len;
    vector_result[1] = y_0 / len;
    vector_result[2] = z_0 / len;

    {
        Vector vector_test_0;
        create_vector_ext(&vector_test_0,x_0, y_0, z_0);

        Vector result; 
        norm_ext(&vector_test_0,&result);

        TEST_ASSERT_EQUAL_FLOAT_ARRAY(vector_result, result.Data, 3);

        TEST_ASSERT_GREATER_OR_EQUAL_FLOAT(min_max[0], result.Data[0]);
        TEST_ASSERT_LESS_OR_EQUAL_FLOAT(min_max[1], result.Data[0]);

        TEST_ASSERT_GREATER_OR_EQUAL_FLOAT(min_max[0], result.Data[1]);
        TEST_ASSERT_LESS_OR_EQUAL_FLOAT(min_max[1], result.Data[1]);

        TEST_ASSERT_GREATER_OR_EQUAL_FLOAT(min_max[0], result.Data[2]);
        TEST_ASSERT_LESS_OR_EQUAL_FLOAT(min_max[1], result.Data[2]);

    }
}
