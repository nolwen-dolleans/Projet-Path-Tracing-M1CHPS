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
	if(S->lightsources != NULL) free_vector(S->lightsources);
	if(S != NULL) free(S);
	
}


Scene * create_scene_ptr(size_t n_objects, size_t n_lightsources, uint24_t backgroundColor){
	Scene * s = malloc(sizeof(Scene));
	s->objects = malloc(sizeof(Sphere*)*n_objects);
	for(int i = 0; i<n_objects; ++i){
		s->objects[i] = malloc(sizeof(Sphere));
	}
	s->lightsources = malloc(sizeof(Vector)*n_lightsources);
	s->background_color = backgroundColor;
	s->size_objects = n_objects;
	s->size_lightsources = n_lightsources;
	return s;
}

Vector intersect_in_scene(const struct Ray* const r, const Scene* const S, int * object){
	int object_index = -1;
	double t = 10000; 				//distance minimale entre l'origine du rayon et de l'objet
	const Vector * origins = &r->position;
	Vector intersection;
	
	for (int i = 0; i<S->size_objects; ++i) {
		Sphere sp = *S->objects[i];
		intersection = intersect_sphere(r, &sp);
		if (!(is_null(&intersection))){
			Vector * w = sub(&intersection,origins);
			double t_new = sqrt(dot(w,w));
			if (t_new < t){
				t = t_new;
				object_index = i;
			}
		}
	}
	if(object_index == -1){
		create_vector_ext(&intersection, 0, 0, 0);
	}
	else {
		intersection = intersect_sphere(r, S->objects[object_index]);
	}
	*object = object_index;
	return intersection;
	
	
}
