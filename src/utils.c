//
// Created by ilya on 27.05.2025.
//

#include "../include/utils.h"

#include <stdlib.h>
#include <string.h>

double **allocate_matrix(const int height, const int width) {
	double **Matrix = malloc(height * sizeof(double *));
	for (int i = 0; i < height; ++i) {
		Matrix[i] = calloc(width, sizeof(double));
	}
	return Matrix;
}

int extension(const char *filename, const char *ext) {
	const size_t l1 = strlen(filename), l2 = strlen(ext);
	return (l1 > l2 && strcmp(filename + l1 - l2, ext) == 0);
}
