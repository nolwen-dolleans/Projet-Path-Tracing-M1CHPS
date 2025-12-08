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
	const Vector * origin = &r->position;
	Vector intersection;
	create_vector_ext(&intersection, 0, 0, 0);
	double closest_t = 1e30; //distance minimale entre l'origine du rayon et de l'objet
	
	for (int i = 0; i<S->size_objects; ++i) {
		Sphere sp = *S->objects[i];
		Vector proche_intersection = intersect_sphere(r, &sp);
		if (is_null(&proche_intersection)){
			continue;
		}
		
		
		Vector diff;
		sub_ext(&intersection, origin, &diff);
		double t = sqrt(dot(&diff, &diff));
		
		if  (t<closest_t){
			closest_t = t;
			object_index = i;
			proche_intersection = intersection;
		}
		
	}
	*object = object_index;
	return intersection;
}
