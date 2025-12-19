#include "image.h"
#include "ray.h"
#include "light.h"
#include "scene.h"
#include "vector.h"

static inline float maxf(float a, float b) { return (a < b) ? b : a;}
static inline float minf(float a, float b) { return (a < b) ? a : b;}

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
	AABB box;

	Vector sphere_color1;
	create_vector_ext(&sphere_color1, 255, 120, 20);
	create_sphere(sphere1, 0.05,0.05,-0.3,0.05, &sphere_color1, false, 0.8);
	
	Vector sphere_color2;
	create_vector_ext(&sphere_color2, 120, 255, 20);
	create_sphere(sphere2, -0.05,0,-0.3,0.05, &sphere_color2, false, 0.5);
	
	Vector sphere_color3;
	create_vector_ext(&sphere_color3, 255, 255, 255);
	create_sphere(sphere3, 0,0.2,-0.3,0.1, &sphere_color3, true, 1);
	
	Vector sphere_color4;
	create_vector_ext(&sphere_color4, 255, 255, 255);
	create_sphere(sphere4, 0,-0.2,-0.3,0.1, &sphere_color4, true, 1);
	
	
	Vector bg;
	create_vector_ext(&bg, 0, 120, 255);
	Scene_ * scene = create_scene_ptr_(4, 0, &bg);

	scene->objects[0].subStruct = sphere1;
	scene->objects[0].type = SPHERE;
	scene->objects[0].emitted = false;
	scene->objects[0].albedo = 0.8;

	scene->objects[1].subStruct = sphere2;
	scene->objects[1].type = SPHERE;
	scene->objects[1].emitted = false;
	scene->objects[1].albedo = 0.8;

	scene->objects[2].subStruct = sphere3;
	scene->objects[2].type = SPHERE;
	scene->objects[2].emitted = true;
	scene->objects[2].albedo = 5;

	scene->objects[3].subStruct = sphere4;
	scene->objects[3].type = SPHERE;
	scene->objects[3].emitted = true;
	scene->objects[3].albedo = 5;

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
				//color = path_trace(&cam, x1, y1, scene, smpls);
				color = path_trace_(&cam, x1, y1, scene, smpls);

				float r=color.Data[0],g=color.Data[1],b=color.Data[2];
				r = maxf(0, color.Data[0]);
				r = minf(255, color.Data[0]);
				g = maxf(0, color.Data[1]);
				g = minf(255, color.Data[1]);
				b = maxf(0, color.Data[2]);
				b = minf(255, color.Data[2]);
				put_color_at_32bit(image, x1, y1, (uint8_t)r, (uint8_t)g, (uint8_t)b, 0);
			}
		}
		write_image_file_32bit(image);
		free_scene_(scene);
		free_image_32bit(image);

	}
	else if(strcmp(argv[1], "24") == 0)
	{
		fprintf(stdout,"Using 24-bit image %ldx%ld.\n",width,height);

		Image_24bit* image = create_image_24bit(width, height);

		//clear_frame_sky_color_32bit(image);
		clear_frame_color_24bit(image,0,0,0);

		for(size_t y1 = 0; y1 < height; ++y1)
		{
			for(size_t x1 = 0; x1 < width; ++x1)
			{
				//color = path_trace(&cam, x1, y1, scene, smpls);
				color = path_trace_(&cam, x1, y1, scene, smpls);
				float r=color.Data[0],g=color.Data[1],b=color.Data[2];
				r = maxf(0, color.Data[0]);
				r = minf(255, color.Data[0]);
				g = maxf(0, color.Data[1]);
				g = minf(255, color.Data[1]);
				b = maxf(0, color.Data[2]);
				b = minf(255, color.Data[2]);
				put_color_at_24bit(image, x1, y1, (uint8_t)r, (uint8_t)g, (uint8_t)b);
			}
		}
		write_image_file_24bit(image);
		free_scene_(scene);
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
				//color = path_trace(&cam, x1, y1, scene, smpls);
				color = path_trace_(&cam, x1, y1, scene, smpls);
				float r=color.Data[0],g=color.Data[1],b=color.Data[2];
				r = maxf(0, color.Data[0]);
				r = minf(255, color.Data[0]);
				g = maxf(0, color.Data[1]);
				g = minf(255, color.Data[1]);
				b = maxf(0, color.Data[2]);
				b = minf(255, color.Data[2]);
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

	return 0;
}

