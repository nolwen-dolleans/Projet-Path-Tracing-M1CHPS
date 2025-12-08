#include "image.h"
#include "ray.h"
#include "light.h"
#include "scene.h"
#include "vector.h"

int main(int argc, char** argv)
{

	srand((unsigned int)time(NULL));
	if(argc != 4)
	{
		fprintf(stderr,"Error : Icomplete arguments.\n");
		exit(1);
	}

	const size_t width  = atoi(argv[2]);
	const size_t height = atoi(argv[3]);

	if(strcmp(argv[1], "32") == 0)
	{
		Image_32bit* image = create_image_32bit(width, height);
		fprintf(stdout,"Using 32-bit image %ldx%ld.\n",image->width,image->height);

		//clear_frame_sky_color_32bit(image);
		clear_frame_color_32bit(image, 0, 0, 0, 0);
		
		const float x0 = 0;
		const float y0 = 0;
		const float z0 = 8;
		const float fov = 100;

		Camera cam;
		Sphere sphere;
		AABB box;

		create_camera(&cam, width, height, fov, x0, y0, z0);
		create_sphere(&sphere, 0,0,2,1);
		create_ray_box(&box, BLU,RED,GRN,RED | BLU,RED | GRN, BLU | GRN);
		

		for(size_t y1 = 0; y1 < height; ++y1)
		{
			for(size_t x1 = 0; x1 < width; ++x1)
			{
				trace_ray(&cam, x1, y1);
				if(box_intersection(&cam, &box)) put_color_at_32bit(image, x1,y1,box.color_hit_r,box.color_hit_g, box.color_hit_b, 0);
				if(sphere_intersection(&cam, &sphere)) put_color_at_32bit(image, x1,y1, sphere.color.Data[0], sphere.color.Data[1], sphere.color.Data[2], 0);


			}
		}

		write_image_file_32bit(image);

		free_image_32bit(image);
	}
	else if(strcmp(argv[1], "24") == 0)
	{
		fprintf(stdout,"Using 24-bit image %ldx%ld.\n",width,height);

		Image_24bit* image = create_image_24bit(width, height);

		clear_frame_sky_color_24bit(image);

		write_image_file_24bit(image);

		free_image_24bit(image);
	}
	else if(strcmp(argv[1], "24ptr") == 0)
	{
		fprintf(stdout,"Using 24-bit ptr image %ldx%ld.\n",width,height);

		Image_24bit_ptr* image = create_image_24bit_ptr(width, height);

		clear_frame_sky_color_24bit_ptr(image);

		write_image_file_24bit_ptr(image);

		free_image_24bit_ptr(image);
	}
	else if(strcmp(argv[1], "path") == 0)
	{
		Image_24bit* image = create_image_24bit(width, height);
		fprintf(stdout,"Using path tracing image %ldx%ld.\n",image->width,image->height);

		//clear_frame_sky_color_32bit(image);
		clear_frame_color_24bit(image, 0, 0, 0);
		
		const float x0 = 0;
		const float y0 = 0;
		const float z0 = 8;
		const float fov = 100;

		Camera cam;
		Sphere * sphere1 = malloc(sizeof(Sphere));
		Sphere * sphere2 = malloc(sizeof(Sphere));
		AABB box;

		create_camera(&cam, width, height, fov, x0, y0, z0);
		
		create_sphere(sphere1, 0.1,-0.05,-0.3,0.05);
		Vector sphere_color1;
		create_vector_ext(&sphere_color1, 255, 0, 0);
		sphere1->color = sphere_color1;
		sphere1->emited = false;
		
		create_sphere(sphere2, 0,-0.05,-0.3,0.05);
		Vector sphere_color2;
		create_vector_ext(&sphere_color2, 255, 255, 255);
		sphere2->color = sphere_color2;
		sphere2->emited = true;
		
		int pixels = 0;
		uint24_t bg;
		set_color_24bit(&bg, 0, 120, 255);
		Scene * scene = create_scene_ptr(2, 0, bg);
		scene->objects[0] = sphere1;
		scene->objects[1] = sphere2;
		Vector color;
		

		for(size_t y1 = 0; y1 < height; ++y1)
		{
			for(size_t x1 = 0; x1 < width; ++x1)
			{
				color = path_trace(&cam, x1, y1, scene);
				put_color_at_24bit(image, x1, y1, (uint8_t)color.Data[0], (uint8_t)color.Data[1], (uint8_t)color.Data[2]);
			}
		}
		write_image_file_24bit(image);
		free_scene(scene);
		free_image_24bit(image);

	}
	else
	{
		fprintf(stderr, "Erreur : argument incorect");
	}

	return 0;
}
