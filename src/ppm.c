#include "image.h"
#include "ray.h"
#include "light.h"
#include "scene.h"
#include "vector.h"

int main(int argc, char** argv)
{

	srand((unsigned int)time(NULL));
	if(argc != 5)
	{
		fprintf(stderr,"Error : Incomplete arguments.\n Please using: %s width height samples",argv[0]);
		exit(1);
	}

	const size_t width  = atoi(argv[2]);
	const size_t height = atoi(argv[3]);
	const size_t smpls = atoi(argv[4]);

	
//######################### Create the entier scene ###########################
//############ Camera ############
	const float x0 = 0;
	const float y0 = 0;
	const float z0 = 8;
	const float fov = 50;
	
	Camera cam;
	create_camera(&cam, width, height, fov, x0, y0, z0);

//############ Objects ############
	Sphere * sphere1 = malloc(sizeof(Sphere));
	Sphere * sphere2 = malloc(sizeof(Sphere));
	Sphere * sphere3 = malloc(sizeof(Sphere));
	Sphere * sphere4 = malloc(sizeof(Sphere));
	AABB *  raybox = malloc(sizeof(AABB));

	const float r = 0.05;
	const float x = r+0.03+EPS;
	const float y = -r+EPS;
	const float z = 0.5+EPS;
	
	Vector sphere_color1;
	create_vector_ext(&sphere_color1, 255, 120, 20);
	create_sphere(sphere1, r);
	Primitive p1;
	create_primitive_ext((void *) (sphere1), SPHERE, -x, y,-z, Lambertian, 1, &sphere_color1, &p1);
	
	Vector sphere_color2;
	create_vector_ext(&sphere_color2, 255, 255, 255);
	create_sphere(sphere2, r);
	Primitive p2;
	create_primitive_ext((void *) (sphere2), SPHERE, x,y,-(z+0.2), Specular, 0.91, &sphere_color2, &p2);
	
	Vector sphere_color3;
	create_vector_ext(&sphere_color3, 255, 255, 20);
	create_sphere(sphere3, 6*r);
	Primitive p3;
	create_primitive_ext((void *) (sphere3), SPHERE, 6*x,y,-(z+0.01), Emissive, 4, &sphere_color3, &p3);
	
	float r_ground = 1000.0;
	Vector sphere_color4;
	create_vector_ext(&sphere_color4, 193,168,154);
	Primitive p4;
	create_sphere(sphere4, r_ground);
	create_primitive_ext((void *) (sphere4), SPHERE, 0,-r_ground-2*r-EPS,-z, Lambertian, 1, &sphere_color4, &p4);
	
	
	Vector box_color;
	create_vector_ext(&box_color, 255, 255, 255);
	Primitive p5;
	create_box(raybox, -1.0f, -1.0f, 2.0f,  1.0f,  1.0f, -2.0f);
	create_primitive_ext((void *) raybox, BOX, 10, 10,10, Lambertian, 0.9, &box_color, &p5);
	
	
	Vector bg;
	create_vector_ext(&bg, 255, 0, 0);
	Scene scene;
	create_scene_ext(5, &bg, &scene);
	
	add_primitive(&p1, &scene);
	add_primitive(&p2, &scene);
	add_primitive(&p3, &scene);
	add_primitive(&p4, &scene);
	add_primitive(&p5, &scene);


	Vector color;
//#############################################################################
	
	if(strcmp(argv[1], "32") == 0)
	{
		Image_32bit * image = create_image_32bit(width, height, smpls);
		fprintf(stdout,"Using path tracing image %ldx%ld.\n",image->width,image->height);

		//clear_frame_sky_color_32bit(image);
		clear_frame_color_32bit(image, 0, 0, 0, 0);

		for(size_t y1 = 0; y1 < height; ++y1)
		{
			for(size_t x1 = 0; x1 < width; ++x1)
			{
				color = path_trace(&cam, x1, y1, &scene, smpls);

				float r=color.Data[0],g=color.Data[1],b=color.Data[2];
				r = max(0, color.Data[0]);
				r = min(255, color.Data[0]);
				g = max(0, color.Data[1]);
				g = min(255, color.Data[1]);
				b = max(0, color.Data[2]);
				b = min(255, color.Data[2]);
				put_color_at_32bit(image, x1, y1, (uint8_t)r, (uint8_t)g, (uint8_t)b, 0);
			}
		}
		write_image_file_32bit(image);
		//free_scene(&scene);
		free_image_32bit(image);

	}
	else if(strcmp(argv[1], "24") == 0)
	{
		fprintf(stdout,"Using 24-bit image %ldx%ld.\n",width,height);

		Image_24bit* image = create_image_24bit(width, height);

		clear_frame_color_24bit(image,0,0,0);

		for(size_t y1 = 0; y1 < height; ++y1)
		{
			for(size_t x1 = 0; x1 < width; ++x1)
			{
				color = path_trace(&cam, x1, y1, &scene, smpls);
				float r=color.Data[0],g=color.Data[1],b=color.Data[2];
				r = max(0, color.Data[0]);
				r = min(255, color.Data[0]);
				g = max(0, color.Data[1]);
				g = min(255, color.Data[1]);
				b = max(0, color.Data[2]);
				b = min(255, color.Data[2]);
				put_color_at_24bit(image, x1, y1, (uint8_t)r, (uint8_t)g, (uint8_t)b);
			}
		}
		write_image_file_24bit(image);
		free_image_24bit(image);
	}
	else if(strcmp(argv[1], "24ptr") == 0)
	{
		fprintf(stdout,"Using 24-bit ptr image %ldx%ld.\n",width,height);

		Image_24bit_ptr* image = create_image_24bit_ptr(width, height);
		
		clear_frame_color_24bit_ptr(image,0,0,0);
		
		for(size_t y1 = 0; y1 < height; ++y1)
		{
			for(size_t x1 = 0; x1 < width; ++x1)
			{
				color = path_trace(&cam, x1, y1, &scene, smpls);
				//color = path_trace_(&cam, x1, y1, scene, smpls);
				float r=color.Data[0],g=color.Data[1],b=color.Data[2];
				r = max(0, color.Data[0]);
				r = min(255, color.Data[0]);
				g = max(0, color.Data[1]);
				g = min(255, color.Data[1]);
				b = max(0, color.Data[2]);
				b = min(255, color.Data[2]);
				put_color_at_24bit_ptr(image, x1, y1, (uint8_t)r, (uint8_t)g, (uint8_t)b);
			}
		}

		write_image_file_24bit_ptr(image);
		free_image_24bit_ptr(image);
	}
	else
	{
		fprintf(stderr, "Erreur : argument incorect");
	}
	
	free(sphere1);
	free(sphere2);
	free(sphere3);
	free(sphere4);
	
	return 0;
}

