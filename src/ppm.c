#include "image.h"
#include "ray.h"
#include "light.h"
#include "scene.h"
#include "vector.h"
#include <time.h>
#include <unistd.h>

int main(int argc, char** argv)
{
	struct timespec t0, t1;
	clock_gettime(CLOCK_MONOTONIC, &t0);
	srand((unsigned int)time(NULL));
	if(argc != 5)
	{
		fprintf(stderr,"Error : Incomplete arguments.\n Please using: %s width height samples",argv[0]);
		exit(1);
	}

	const size_t width  = atoi(argv[2]);
	const size_t height = atoi(argv[3]);
	const size_t smpls = atoi(argv[4]);
	
	size_t bounces = (size_t) get_bounces();

	
//######################### Create the entier scene ###########################
//############ Camera ############
	const float x0 = 0;
	const float y0 = 0;
	const float z0 = 0;
	const float fov = 50;
	
	Camera cam;
	create_camera(&cam, width, height, fov, x0, y0, z0, 0, 0);

//############ Objects ############
	Vector bg;
	create_vector_ext(&bg, 255, 0, 0);
	Scene scene;
	create_scene_ext(6, &bg, &scene);
	
	
	const float r = 0.05;
	const float x = r+0.03+EPS;
	const float y = -r+EPS;
	const float z = 0.5+EPS;
	
	Vector sphere_color1;
	create_vector_ext(&sphere_color1, 255, 120, 20);
	Primitive p1;
	create_sphere(&p1, r, -x, y, -z, Lambertian, 1, &sphere_color1);
	add_primitive(&p1, &scene);
	
	Vector sphere_color2;
	create_vector_ext(&sphere_color2, 255, 255, 255);
	Primitive p2;
	create_sphere(&p2, r, x,y,-(z+0.2), Specular, 0.91, &sphere_color2);
	add_primitive(&p2, &scene);
	
	Vector sphere_color3;
	create_vector_ext(&sphere_color3, 255, 255, 20);
	Primitive p3;
	create_sphere(&p3, 6*r, 6*x,y,-(z+0.01), Emissive, 1, &sphere_color3);
	add_primitive(&p3, &scene);
	
	float r_ground = 1000.0;
	Vector sphere_color4;
	create_vector_ext(&sphere_color4, 193,168,154);
	Primitive p4;
	create_sphere(&p4, r_ground, 0,-r_ground-2*r-EPS,-z, Lambertian, 1, &sphere_color4);
	//add_primitive(&p4, &scene);
	
	Vector raybox_color;
	create_vector_ext(&raybox_color, 255, 255, 255);
	Primitive p5;
	create_cube(&p5, 2, 0, 1-2*r, 0, Lambertian, 1, &raybox_color);
	add_primitive(&p5, &scene);
	
	Vector box_color;
	create_vector_ext(&box_color, 255, 255, 255);
	Primitive p6;
	create_box(&p6, 3*EPS, 2, 2, -x-0.07, y, -z, Specular, 0.91, &box_color);
	add_primitive(&p6, &scene);


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
				color = path_trace(&cam, x1, y1, &scene, smpls, bounces);

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
				color = path_trace(&cam, x1, y1, &scene, smpls, bounces);
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
				color = path_trace(&cam, x1, y1, &scene, smpls, bounces);
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
	free_scene_objects(&scene);
	clock_gettime(CLOCK_MONOTONIC, &t1);
	
	char* path="performance/measures/runtime_by_samplings.csv";
	
	bool exists = (access(path, F_OK) == 0);
	
	FILE *f = fopen(path, "a");
	if (!f) {
		perror("fopen");
		return 1;
	}
	
	if (!exists) {
		fprintf(f, "nsamples,bounces,runtime\n");
	}
	double elapsed = (t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec) * 1e-9;
	fprintf(f, "%ld,%ld,%.6f\n", smpls, bounces, elapsed);
	fclose(f);
	return 0;
}

