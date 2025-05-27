//
// Created by ilya on 27.05.2025.
//

#include "../include/utils.h"

#include <stdlib.h>

double **allocate_matrix(const int height, const int width) {
	double **Matrix = malloc(height * sizeof(double *));
	for (int i = 0; i < height; ++i) {
		Matrix[i] = calloc(width, sizeof(double));
	}
	return Matrix;
}
