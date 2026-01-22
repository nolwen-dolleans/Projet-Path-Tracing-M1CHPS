#include "vector.h"
#include "float.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void create_vector_random_default_ext(Vector* u)
{

    u->Data[0] = generate_random_value(FLT_MAX, FLT_MIN);
    u->Data[1] = generate_random_value(FLT_MAX, FLT_MIN);
    u->Data[2] = generate_random_value(FLT_MAX, FLT_MIN);

}

void create_vector_random_ext(Vector* u, const float min, const float max)
{
    u->Data[0] = generate_random_value(max, min);
    u->Data[1] = generate_random_value(max, min);
    u->Data[2] = generate_random_value(max, min);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Vector* viewport(const Vector* const u, const float width, const float height, const float near, const float far)
{
    Vector* w = (Vector*)malloc_check(sizeof(Vector));

    w->Data[0] = u->Data[0]*width + width*0.5f;
    w->Data[1] = -u->Data[1]*height + height*0.5f;
    w->Data[2] = u->Data[2]*(far - near);
	
	return w;

}

void display_vector(const Vector* const u)
{
    fprintf(stdout, "[%f]\n[%f]\n[%f]\n\n", u->Data[0], u->Data[1], u->Data[2]);
}

void free_vector(Vector* u)
{
    if(u) free(u);
    else fprintf(stderr,"No need to free memory.\n");
}
