#include "light.h"
#include <time.h>
#include <unistd.h>

#include "mpi.h"


int main(int argc, char** argv)
{
	setvbuf(stdout, NULL, _IONBF, 0);
	int tag = 1000;
	
	srand((unsigned int)time(NULL));
	if(argc < 4)
	{
		fprintf(stderr,"Error : Incomplete arguments.\n Please using: %s width height samples\n",argv[0]);
		exit(1);
	}
	
	

	const int width  = atoi(argv[1]);
	const int height = atoi(argv[2]);
	const size_t smpls = atoi(argv[3]);
	size_t limit = 1;
	int can_print_image = 1;
	if(argc > 4){
		limit = atoi(argv[4]);
		if(argc == 6) {
			if(strcmp(argv[5], "no_image")){
				printf("Error: please use \"no_image\" insted of \"%s\".", argv[5]);
				exit(1);
			}
			can_print_image = 0;
		}
	}
	
	size_t bounces = (size_t) get_bounces();

	
//######################### Create the entier scene ###########################
	Scene scene;
	benchmark1(&scene, width, height);

	Vector color;
//#############################################################################
	
	if(!limit) exit(1);
	size_t print_rate = smpls / limit;
	if(print_rate == 0) print_rate = 1;
	int first = 1;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
	
	Image_32bit *image = create_image_32bit(width, height, smpls);
	
	struct timespec t0, t1;
	if (mpi_rank == 0) {
		fprintf(stdout,"Using path tracing image %dx%d.\n", width, height);
		
		clock_gettime(CLOCK_MONOTONIC, &t0);
	}

	const size_t per_t_height = height / mpi_size;

	float *local_color_buffer = calloc(width * per_t_height * 3, sizeof(float));

	uint32_t *local_pixels_buffer = malloc(width * per_t_height * sizeof(uint32_t));

	
	for(size_t i = 1; i <= smpls; ++i) {
		path_trace(width, height, &scene, bounces, smpls, local_color_buffer);

		if (i % print_rate == 0) {
			float inv_samples = 255.f / (float)(i + 1);

			for(size_t y = 0; y < per_t_height; ++y) {
				for(size_t x = 0; x < width; ++x) {
					size_t idx = y * width + x;
					size_t idx_rgb = idx * 3;

					float r = local_color_buffer[idx_rgb]   * inv_samples;
					float g = local_color_buffer[idx_rgb+1] * inv_samples;
					float b = local_color_buffer[idx_rgb+2] * inv_samples;

					r = max(0.f, min(255.f, r));
					g = max(0.f, min(255.f, g));
					b = max(0.f, min(255.f, b));

					local_pixels_buffer[idx] = get_color_32bit(r, g, b, 0);
				}
			}

			if (mpi_size != 1){
				if (mpi_rank == 0) {
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
							fprintf(f, "MPI,OMP,nsamples,bounces,runtime\n");
						}
						
						double elapsed = (t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec) * 1e-9;
						fprintf(f, "%d,%d,%ld,%ld,%.6f\n",mpi_size, omp_get_num_threads(), i, bounces, elapsed);
						if(i == smpls) fprintf(f, "\n");
						fclose(f);
					}
					
					
					if((can_print_image)||(i==smpls)){
						MPI_Gather(local_pixels_buffer, width * per_t_height, MPI_INT32_T, image->buffer, width * per_t_height, MPI_INT32_T, 0, MPI_COMM_WORLD);
						write_image_file_32bit(image, i);
					}
					
				}
				else {
					if(can_print_image || i==smpls)
						MPI_Gather(local_pixels_buffer, width * per_t_height, MPI_INT32_T, NULL, width * per_t_height, MPI_INT32_T, 0, MPI_COMM_WORLD);
				}
			}
			else {
				memcpy(image->buffer, local_pixels_buffer, width * per_t_height * sizeof(uint32_t));
				write_image_file_32bit(image, i);
			}
		}
	}
	MPI_Finalize();
	
	
	
	free_scene_objects(&scene);
	free_image_32bit(image); free(local_pixels_buffer); free(local_color_buffer);
	return 0;
}

/*
make
BOUNCES=10 mpirun -np 8 ./ppm 32 800 600 10 && powershell.exe -Command "[console]::beep(250,300)"
display -geometry 800x600+1920+1 image/image_32bit10.ppm
*/
