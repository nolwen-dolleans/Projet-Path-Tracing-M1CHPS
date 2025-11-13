//
//  scene.c
//  Projet PPN
//
//  Created by Nolwen Dolléans on 13/11/2025.
//

#include "scene.h"






void free_scene(Scene * S){
	for(int i = 0;i<sizeof(S->objects);++i){
		free_sphere(S->objects[i]);
	}
	
	free(S->objects);
	free_vector(S->lightsources);
	free(S->background_color);
	
}


Scene * mk_scene_ptr(size_t n_objects, size_t nb_lightsources, uint32_t * backgroundColor){
	Scene * s = malloc(sizeof(Scene));
	s->objects = malloc(sizeof(Sphere)*n_objects);
	s->lightsources = malloc(sizeof(Vector)*nb_lightsources);
	s->background_color = backgroundColor;
	return s;
}



void add_object_to_scene(Scene * S, Sphere * s){
	int i = 0;
	while (S->objects[i] != NULL) {
		++i;
	}
	S->objects[i] = s;
}

Vector* intersect_in_scene(Vector * origin, const Ray* const r, const Scene* const S){
	Vector* intersection = create_vector_default();
	int object_index = -1;
	double t = 10000; 				//distance minimale entre l'origine du rayon et de l'objet
	
	for (int i = 0; (i<sizeof(S->objects) || (intersection == NULL)); ++i) {
		Sphere *sp = S->objects[i];
		intersection = intersect_sphere(r, S->objects[i]);
		if (intersection != NULL){
			double t_new = sqrt(dot(intersection,origin));
			if (t_new < t){
				t = t_new;
				object_index = i;
			}
		}
	}
	if(object_index == -1){
		return NULL;
	}

	return intersect_sphere(r, S->objects[object_index]);
	
	
}
