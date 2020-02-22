/*********************
**  Color Map generator
** Skeleton by Justin Yokota
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "ColorMapInput.h"


/**************
**This function reads in a file name colorfile.
**It then uses the information in colorfile to create a color array, with each color represented by an int[3].
***************/
uint8_t** FileToColorMap(char* colorfile, int* colorcount)
{
	FILE* ptr = fopen(colorfile,"r"); 
	if (ptr == NULL) {
		return NULL;
	}
	uint8_t a; uint8_t b; uint8_t c;
	if (!fscanf(ptr, "%d", colorcount)) {
		fclose(ptr);
		return NULL;
	}
	uint8_t** output = malloc(*colorcount * sizeof(uint8_t*));
	if (output == NULL) {
		return NULL;
	}
	for(int i = 0; i < *colorcount; i++) {
		output[i] = malloc(3 * sizeof(uint8_t));
		if (output[i] == NULL) {
			free(output);
			fclose(ptr);
			return NULL;
		}
	}
	uint8_t** p = output;
	uint8_t* q;
	int count = 0;
	while (count < *colorcount) {
		if (!fscanf(ptr, "%hhu %hhu %hhu", &a, &b, &c)) {
			free(output);
			fclose(ptr);
			return NULL;
		}
		q = *p;
		*q = a; *(q + 1) = b; *(q + 2) = c;
		count++;
		p++;
	}
	fclose(ptr);
	return output;
}
/*int main(int argc, char* argv[]){
	char* colorfile = "defaultcolormap.txt";
	int* colorcount = malloc(sizeof(int*));
	uint8_t** out = FileToColorMap(colorfile, colorcount);
	printf("%d", *colorcount);
	for(int i = 0; i < 384; i++) {
		for (int j = 0; j < 3; j++) {
			printf("%d ", out[i][j]);
		}
		printf("\n");
	}
}*/

