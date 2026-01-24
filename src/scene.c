//
//  scene.c
//  Projet PPN
//
//  Created by Nolwen Dolléans on 13/11/2025.
//

#include "scene.h"

void free_scene(Scene * S){
	if(S->objects) free(S->objects);
	if(S->objects) free(S);
	
}

void create_scene_ext(size_t n_objects, const Vector * backgroundColor, Scene * s){
	float inv255 = 1.0f / 255.0f; // normalisation de la couleur
	s->objects = malloc(sizeof(Primitive)*n_objects);
    for (size_t i = 0; i < n_objects; ++i) {
        s->objects[i] = NULL;
    }
	s->background_color = malloc(sizeof(Vector));
	mul_ext(backgroundColor, inv255, s->background_color);
	s->size_objects = n_objects;
}

void create_primitive_ext(void * shape, PRIM_TYPE type, float x, float y, float z, material_t m_type, float albedo, Vector *color, Primitive *prim){
	const float inv255 = 1 / 255.0f;
	prim->type = type;
	prim->m_type = m_type;
	create_vector_ext(&prim->position, x, y, z);
	prim->albedo = albedo;
	prim->color = *color;
	for (size_t i = 0; i < 3; ++i) {
		prim->color.Data[i] = color->Data[i]*inv255;
	}
	prim->object = shape;
}

void add_primitive(Primitive * object, Scene * s){
    if (s == NULL || s->objects == NULL) return;
    for (size_t i = 0; i < s->size_objects; ++i) {
        if (s->objects[i] == NULL) {
            s->objects[i] = object;
            return;
        }
    }
	printf("Error: too many objects");
	exit(EXIT_FAILURE);
}


bool intersect_in_scene(struct Ray* r, const Scene* const S, int *object, Vector *hit, Vector *n){
	if(S->objects == NULL) return false;
	int object_index = -1;
	
	const Vector *origin = &r->position;
	Vector best_hit;
	double closest_t = 1e30;
	
	for (int i = 0; i < S->size_objects; ++i) {
		if(S->objects[i] == NULL) continue;
		
		switch (S->objects[i]->type) {
			case SPHERE:{
				const Sphere *sp = (Sphere *)S->objects[i]->object;
				
				if (!intersect_sphere(r,&S->objects[i]->position, sp->radius, hit))
					continue;
				Vector diff;
				sub_ext(hit, origin, &diff);
				double t2 = dot(&diff, &diff);
				
				if (t2 < closest_t) {
					closest_t = t2;
					object_index = i;
					best_hit = *hit;
					*n = get_normal_vector_sphere(hit, &S->objects[i]->position);
				}
				break;
			}
				
			case BOX: {
				AABB *box = (AABB *)S->objects[i]->object;
				int face;
				int is_intern;

				if (!intersect_box(r, box, hit, &face, &is_intern))
					continue;

				Vector diff;
				sub_ext(hit, origin, &diff);
				double t2 = dot(&diff, &diff);

				if (t2 < closest_t) {
					closest_t = t2;
					object_index = i;
					best_hit = *hit;
					*n = get_normal_vector_box(hit, box, &face, is_intern);
				}
				break;
			}
		}
		
	}

	if (object_index == -1)
		return false;

	*object = object_index;
	*hit = best_hit;
	return true;
}

