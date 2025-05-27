//
// Created by ilya on 26.05.2025.
//

#ifndef PGM_H
#define PGM_H

#include <stdio.h>

typedef struct {
	int width, height;
	int max;
	double **data;
} PGMImage;

PGMImage *read_pgm_stream(FILE *file);
PGMImage *read_pgm_image(const char *filename);

int write_pgm(const char *filename, const PGMImage *image);
void free_pgm(PGMImage *image);

#endif	// PGM_H
