#ifndef LIGHT_H
#define LIGHT_H

#include "vector/vector.h"

void phong_light(const float ambient_str, Vector* normal, const Vector* color_light, Vector* hitPoint, Vector* lightPos, const Vector* color_in, Vector* color_out)
{
    //Ambient
    const float amb_x = ambient_str * color_light->Data[0] ;
    const float amb_y = ambient_str * color_light->Data[1] ;
    const float amb_z = ambient_str * color_light->Data[2] ;

    //Diffuse
    norm_ext(normal, normal);

    Vector lightDir0;
    Vector lightDir1;

    sub_ext(lightPos, hitPoint, &lightDir0);
    norm_ext(&lightDir0, &lightDir1);
    
    const float dt = dot(normal, &lightDir1);
    const float dif = dt < 0.0f ? 0.0f : dt; 

    const float specular_str = 0.5f;

    const float x = (0 + dif)*color_in->Data[0];
    const float y = (0 + dif)*color_in->Data[1];
    const float z = (0 + dif)*color_in->Data[2];

    create_vector_ext(color_out, x, y, z);
}

#endif