#include "unity.h"
#include "scene.h"
#include "float.h"

#define TOLERANCE 1e-6


void test_intersect_scene(void){
	
	Vector BLACK;
	create_vector_ext(&BLACK, 0, 0, 0);
	Scene * s = create_scene_ptr(1, 1, &BLACK);
	printf("%p\n", s->objects[0]);
	
	create_sphere(s->objects[0], 0, 0, 2, 1, NULL, 0, Lambertian);
	Vector hit;
	int object = -1;
	Ray ray;
	create_ray_ext(&ray, 0, 0, 0, 0, 0, 0.5);
	
	intersect_in_scene(&ray, s, &object, &hit);
	
	
	TEST_ASSERT_EQUAL_INT(object, 0);
	
	TEST_ASSERT_FLOAT_WITHIN(TOLERANCE,0,hit.Data[0]);
	TEST_ASSERT_FLOAT_WITHIN(TOLERANCE,0,hit.Data[1]);
	TEST_ASSERT_FLOAT_WITHIN(TOLERANCE,1,hit.Data[2]);
	
	free_scene(s);
}


void test_create_scene(void){
	srand((unsigned int)time(NULL));
	int nbobjects = rand()%101;
	Vector BLACK;
	create_vector_ext(&BLACK, 0, 0, 0);
	
	Scene * S = create_scene_ptr(nbobjects, rand(), &BLACK);
	for (int i = 0; i<S->size_objects; ++i) {
		float x = rand();
		float y = rand();
		float z = rand();
		Sphere * s = malloc_check(sizeof(Sphere));
		create_sphere(s,x, y, z, 1, NULL, 0, Lambertian);
		S->objects[i] = s;
		TEST_ASSERT_FLOAT_WITHIN(TOLERANCE,S->objects[i]->radius,1);
		
		TEST_ASSERT_FLOAT_WITHIN(TOLERANCE,S->objects[i]->position.Data[0],x);
		TEST_ASSERT_FLOAT_WITHIN(TOLERANCE,S->objects[i]->position.Data[1],y);
		TEST_ASSERT_FLOAT_WITHIN(TOLERANCE,S->objects[i]->position.Data[2],z);
		
	}
	free_scene(S);
	
	
}
