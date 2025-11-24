#include "unity.h"
#include "scene.h"
#include "float.h"

#define TOLERANCE 1e-6


void test_intersect_scene(void){
	
	Scene * s = create_scene_ptr(1, 1, 0);
	
	s->objects[0] = create_sphere(0, 0, 2, 1);
	Vector * intersect = NULL;
	Vector * origin = create_vector(0, 0, 0);
	int object = -1;
	Ray * ray = create_ray(0, 0, 0, 0, 0, 0.5);
	
	intersect = intersect_in_scene(origin, ray, s, &object);
	
	
	TEST_ASSERT_EQUAL_INT(object, 0);
	
	TEST_ASSERT_FLOAT_WITHIN(TOLERANCE,0,intersect->Data[0]);
	TEST_ASSERT_FLOAT_WITHIN(TOLERANCE,0,intersect->Data[1]);
	TEST_ASSERT_FLOAT_WITHIN(TOLERANCE,1,intersect->Data[2]);
	
	free_scene(s);
	free_ray(ray);
	free_vector(origin);
	free_vector(intersect);
}


void test_create_scene(void){
	srand((unsigned int)time(NULL));
	int nbobjects = rand()%101;
	Scene * S = create_scene_ptr(nbobjects, rand(), 0);
	for (int i = 0; i<S->size_objects; ++i) {
		float x = rand();
		float y = rand();
		float z = rand();
		Sphere * s = create_sphere(x, y, z, 1);
		add_object_to_scene(S,s);
		TEST_ASSERT_FLOAT_WITHIN(TOLERANCE,S->objects[i]->radius,1);
		
		TEST_ASSERT_FLOAT_WITHIN(TOLERANCE,S->objects[i]->position.Data[0],x);
		TEST_ASSERT_FLOAT_WITHIN(TOLERANCE,S->objects[i]->position.Data[1],y);
		TEST_ASSERT_FLOAT_WITHIN(TOLERANCE,S->objects[i]->position.Data[2],z);
		
	}
	
	free_scene(S);
	
	
}
