/*********************
**  Mandelbrot fractal movie generator
** clang -Xpreprocessor -fopenmp -lomp -o Mandelbrot Mandelbrot.c
** by Dan Garcia <ddgarcia@cs.berkeley.edu>
** Modified for this class by Justin Yokota and Chenyu Shi
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "ComplexNumber.h"
#include "Mandelbrot.h"
#include "ColorMapInput.h"
#include <sys/types.h>
#include <string.h>

void printUsage(char* argv[])
{
  printf("Usage: %s <threshold> <maxiterations> <center_real> <center_imaginary> <initialscale> <finalscale> <framecount> <resolution> <output_folder> <colorfile>\n", argv[0]);
  printf("    This program simulates the Mandelbrot Fractal, and creates an iteration map of the given center, scale, and resolution, then saves it in output_file\n");
}


/*
This function calculates the threshold values of every spot on a sequence of frames. The center stays the same throughout the zoom. First frame is at initialscale, and last frame is at finalscale scale.
The remaining frames form a geometric sequence of scales, so 
if initialscale=1024, finalscale=1, framecount=11, then your frames will have scales of 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1.
As another example, if initialscale=10, finalscale=0.01, framecount=5, then your frames will have scale 10, 10 * (0.01/10)^(1/4), 10 * (0.01/10)^(2/4), 10 * (0.01/10)^(3/4), 0.01 .
*/
void MandelMovie(double threshold, u_int64_t max_iterations, ComplexNumber* center, double initialscale, double finalscale, int framecount, u_int64_t resolution, u_int64_t ** output){
	double scale = initialscale;
    for (int i = 0; i < framecount; i++) {
		scale = initialscale * exp(i * log(finalscale / initialscale) / (framecount - 1));
		Mandelbrot(threshold, max_iterations, center, scale, resolution, output[i]);
	}
	
}

int iterToColor(uint8_t** colormap, int colorcount, u_int64_t *arr, u_int64_t size, char* filename) {
		FILE* in = fopen(filename, "w");
		if (in == NULL) {
			return 1;
		}
		if (fprintf(in, "%s %llu %llu %d\n", "P6", size, size, 255) < 0) {
			fclose(in);
			return 1;
		}
		uint8_t* color;
		for (int i = 0; i < size * size; i++) {
			if (arr[i] == 0) {
				uint8_t zero[3] = {0, 0, 0};
				fwrite(zero, sizeof(uint8_t), 3, in);
				continue;
			}
			color = colormap[(arr[i] + colorcount - 1) % colorcount];
			fwrite(color, sizeof(uint8_t), 3, in);
		}
		fclose(in);
		return 0;
}

/**************
**This main function converts command line inputs into the format needed to run MandelMovie.
**It then uses the color array from FileToColorMap to create PPM images for each frame, and stores it in output_folder
***************/
int main(int argc, char* argv[])
{
	//Tips on how to get started on main function: 
	//MandelFrame also follows a similar sequence of steps; it may be useful to reference that.
	//Mayke you complete the steps below in order. 

	//STEP 1: Convert command line inputs to local variables, and ensure that inputs are valid.
	/*
	Check the spec for examples of invalid inputs.
	Remember to use your solution to B.1.1 to process colorfile.
	*/

	if (argc != 11) {
		printf("%s: Wrong number of arguments, expecting 10\n", argv[0]);
		printUsage(argv);
		return 1;
	}
	double threshold, center_real, center_imaginary, initialscale, finalscale; 
	int framecount; ComplexNumber* center;
	u_int64_t max_iterations, resolution;

	threshold = atof(argv[1]);
	max_iterations = (u_int64_t)atoi(argv[2]);
	center = newComplexNumber(atof(argv[3]), atof(argv[4]));
	initialscale = atof(argv[5]);
	finalscale = atof(argv[6]);
	framecount = (int)atoi(argv[7]);
	resolution = (u_int64_t)atoi(argv[8]);

	u_int64_t size = 2 * resolution + 1;

	if (threshold <= 0 || initialscale <= 0 || finalscale <= 0 || max_iterations <= 0) {
		printf("The threshold, scale, and max_iterations must be > 0");
		printUsage(argv);
		return 1;
	}
	if (framecount <= 0 || framecount > 10000) {
		printf("The number of frames exceeds 10,000, or is 0 or less");
		printUsage(argv);
		return 1;
	}
	if (framecount == 1 && initialscale != finalscale) {
		printf("The number of frames is 1 AND initialscale != finalscale");
		printUsage(argv);
		return 1;
	}
	if (resolution < 0) {
		printf("The resolution >= 0");
		printUsage(argv);
		return 1;
	}
	if (initialscale <= finalscale) {
		finalscale = initialscale;
	}
	//STEP 2: Run MandelMovie on the correct arguments.
	/*
	MandelMovie requires an output array, so make sure you allocate the proper amount of space. 
	If allocation fails, free all the space you have already allocated (including colormap), then return with exit code 1.
	*/
	u_int64_t **ar = (u_int64_t **)malloc(framecount * sizeof(u_int64_t*));
	if (ar == NULL) {
		printf("Unable to allocate %lu bytes\n", framecount * sizeof(u_int64_t*));
		return 1;
	}
	for (int i = 0; i < framecount; i++) {
		ar[i] = (u_int64_t *)malloc(size * size * sizeof(u_int64_t));
		if (ar[i] == NULL) {
			printf("Unable to allocate %llu bytes\n", size * size * sizeof(u_int64_t));
			free(ar);
			return 1;
		}
	}

	MandelMovie(threshold, max_iterations, center, initialscale, finalscale, framecount, resolution, ar);
/*	for (int i = 0; i < framecount; i++) {
		u_int64_t *temp;
		temp = ar[i];
		for (int j = 0; j < 25; j++) {
			printf("%llu ", temp[j]);
		}
		printf("\n");
	}
*/


	//STEP 3: Output the results of MandelMovie to .ppm files.
	/*
	Convert from iteration count to colors, and output the results into output files.
	Use what we showed you in Part B.1.2, create a seqeunce of ppm files in the output folder.
	Feel free to create your own helper function to complete this step.
	As a reminder, we are using P6 format, not P3.
	*/
	
	int* colorcount = malloc(sizeof(int*));
	if (colorcount == NULL) {
		free(ar);
		return 1;
	}
	uint8_t** colormap = FileToColorMap(argv[10], colorcount);
	if (colormap == NULL) {
		free(ar);
		return 1;
	}
	printf("Calculation complete, outputting to file %s\n", argv[9]);

	char filename[15];
	char str[41] = "";
	u_int64_t *temp;
	for (int i = 0; i < framecount; i++) {
		sprintf(filename, "frame%05d.ppm", i);
		temp = ar[i];
		iterToColor(colormap, *colorcount, temp, size, strcat(strcat(strcat(str, argv[9]), "/"), filename));
		strcpy(filename, "");
		strcpy(str, "");
	}

	//STEP 4: Free all allocated memory
	/*
	Make sure there's no memory leak.
	*/
	free(ar);
	free(colormap);
	free(colorcount);
	freeComplexNumber(center);

	return 0;
}

