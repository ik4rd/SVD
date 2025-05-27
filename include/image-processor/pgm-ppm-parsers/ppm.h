//
// Created by ilya on 27.05.2025.
//

#ifndef PPM_H
#define PPM_H

#include <stdio.h>

typedef struct {
	int width, height;
	int max;
	double **R;
	double **G;
	double **B;
} PPMImage;

PPMImage *read_ppm_stream(FILE *file);
PPMImage *read_ppm_image(const char *filename);

int write_ppm(const char *filename, const PPMImage *image);
void free_ppm(PPMImage *image);

#endif	// PPM_H
