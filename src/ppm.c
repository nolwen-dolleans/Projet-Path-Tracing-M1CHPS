#include "image.h"
#include "ray.h"
#include "light.h"
#include "scene.h"
#include "vector.h"
#include <time.h>
#include <unistd.h>
#include <mpi.h>

int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
	setvbuf(stdout, NULL, _IONBF, 0);
	int tag = 1000;
	
	struct timespec t0, t1;
	clock_gettime(CLOCK_MONOTONIC, &t0);
	srand((unsigned int)time(NULL));
	if(argc != 5)
	{
		fprintf(stderr,"Error : Incomplete arguments.\n Please using: %s width height samples\n",argv[0]);
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
	const float z0 = 1.5;
	const float fov = 50;
	
	Camera cam;
	create_camera(&cam, width, height, fov, x0, y0, z0, 0, 0);

//############ Objects ############
	Vector bg;
	create_vector_ext(&bg, 255, 0, 0);
	Scene scene;
	create_scene_ext(15, &bg, &scene);
	
	
	const float r = 0.15;
	const float x = 0.21;
	const float y = -0.15;
	const float z = 0.5;
	
	Vector sphere_color3;
	create_vector_ext(&sphere_color3, 255, 255, 255);
	Primitive p3;
	create_sphere(&p3, 5*r, 7*x,y,-0.51, Emissive, 1, &sphere_color3);
	add_primitive(&p3, &scene);
	
	// Vector roof_light;
	// create_vector_ext(&roof_light, 255, 123, 123);
	// Primitive p1;
	// create_box(&p1, 0.05, 1.5, 0.05, -1.2, 0, -2, Lambertian, 1, &roof_light, 0, 0);
	// add_primitive(&p1, &scene);

	Vector raybox_color;
	create_vector_ext(&raybox_color, 255, 230, 230);
	Primitive p5;
	create_box(&p5, 2.85, 2, 5, 0, 0.25, 0, Lambertian, 1, &raybox_color, 0, 0);
	add_primitive(&p5, &scene);

	Vector pylone;
	create_vector_ext(&pylone, 255, 123, 123);
	Primitive p7;
	create_box(&p7, 0.05, 1.5, 0.05, -1.2, 0, -2, Lambertian, 1, &pylone, 0, 0);
	add_primitive(&p7, &scene);

	Vector pylone_base;
	create_vector_ext(&pylone_base, 255, 123, 123);
	Primitive p8;
	create_box(&p8, 0.4, 0.1, 0.4, -1.2, -0.7, -2, Lambertian, 1, &pylone_base, 0, 0);
	add_primitive(&p8, &scene);

	Vector pylone_up;
	create_vector_ext(&pylone_up, 255, 123, 123);
	Primitive p9;
	create_box(&p9, 0.17, 0.06, 0.17, -1.2, 0.72, -2, Lambertian, 1, &pylone_up, 0, 0);
	add_primitive(&p9, &scene);

	Vector light_bulb;
	create_vector_ext(&light_bulb, 240, 167, 0);
	Primitive p10;
	create_sphere(&p10, r, -1.2, 0.9, -2, Emissive, 4, &light_bulb);
	add_primitive(&p10, &scene);

	Vector mirror;
	create_vector_ext(&mirror, 255, 255, 255);
	Primitive p11;
	create_box(&p11, 0.3, 1, 0.01, -1, 0, -0.8, Specular, 1, &mirror, 90, 75);
	add_primitive(&p11, &scene);

	Vector color;
//#############################################################################
	
	if(strcmp(argv[1], "32") == 0)
	{
		const size_t per_t_height = height/mpi_size;
		Image_32bit * image = create_image_32bit(width, per_t_height, smpls);
		if (mpi_rank == 0) {
			fprintf(stdout,"Using path tracing image %ldx%ld.\n",image->width,image->height);
		}

		//clear_frame_sky_color_32bit(image);
		clear_frame_color_32bit(image, 0, 0, 0, 0);

		for(size_t y1 = per_t_height*mpi_rank; y1 < per_t_height*(mpi_rank+1); ++y1)
		{
			size_t local_y = y1 - per_t_height * mpi_rank;
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
				put_color_at_32bit(image, x1, local_y, (uint8_t)r, (uint8_t)g, (uint8_t)b, 0);
			}
		}

		if (mpi_rank == 0) {
			write_image_file_32bit(image);
			free_image_32bit(image);
			MPI_Send(NULL, 0, MPI_BYTE, mpi_rank+1, tag, MPI_COMM_WORLD);
		}
		else if (mpi_rank != mpi_size-1) {
			MPI_Recv(NULL, 0, MPI_BYTE, mpi_rank-1, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			write_image_file_32bit(image);
			free_image_32bit(image);
			MPI_Send(NULL, 0, MPI_BYTE, mpi_rank+1, tag, MPI_COMM_WORLD);
		}
		else {
			MPI_Recv(NULL, 0, MPI_BYTE, mpi_rank-1, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			write_image_file_32bit(image);
			free_image_32bit(image);
		}
	}
	else
	{
		fprintf(stderr, "Erreur : argument incorect\n");
	}
	free_scene_objects(&scene);
	// MPI_Bcast(NULL, 0, MPI_BYTE, 0, tag, MPI_COMM_WORLD);
	clock_gettime(CLOCK_MONOTONIC, &t1);
	
	if (mpi_rank == 0) {
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
	}
	MPI_Finalize();
	return 0;
}

/*
make
BOUNCES=10 mpirun -np 8 ./ppm 32 800 600 10 && powershell.exe -Command "[console]::beep(250,300)"
display -geometry 800x600+1920+1 image/image_32bit10.ppm
*/