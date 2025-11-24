//
//  scene.c
//  Projet PPN
//
//  Created by Nolwen Dolléans on 13/11/2025.
//

#include "scene.h"






void free_scene(Scene * S){
	for(int i = 0;i<S->size_objects;++i){
		free_sphere(S->objects[i]);
	}
	free(S->objects);
	free_vector(S->lightsources);
	free(S);
	
}


Scene * create_scene_ptr(size_t n_objects, size_t n_lightsources, uint24_t backgroundColor){
	Scene * s = malloc(sizeof(Scene));
	s->objects = malloc(sizeof(Sphere)*n_objects);
	s->lightsources = malloc(sizeof(Vector)*n_lightsources);
	s->background_color = backgroundColor;
	s->size_objects = n_objects;
	s->size_lightsources = n_lightsources;
	return s;
}



void add_object_to_scene(Scene * S, Sphere * s){
	int i = 0;
	while (S->objects[i] != NULL) {
		++i;
	}
	S->objects[i] = s;
}

Vector intersect_in_scene(const Ray* const r, const Scene* const S, int * object){
	int object_index = -1;
	double t = 10000; 				//distance minimale entre l'origine du rayon et de l'objet
	const Vector * origins = &r->position;
	Vector intersection;
	Vector * int_ptr = &intersection;
	
	for (int i = 0; i<S->size_objects; ++i) {
		Sphere *sp = S->objects[i];
		int_ptr = intersect_sphere(r, sp);
		if (int_ptr != NULL){
			Vector * w = sub(int_ptr,origins);
			double t_new = sqrt(dot(w,w));
			if (t_new < t){
				t = t_new;
				object_index = i;
			}
		}
	}
	if(object_index == -1){
		int_ptr = NULL;
	}
	else {
		int_ptr = intersect_sphere(r, S->objects[object_index]);
	}
	*object = object_index;
	printf("%f %f %f\n",int_ptr->Data[0],int_ptr->Data[1],int_ptr->Data[2]);
	return intersection;
	
	
}
