//
// Created by ilya on 26.05.2025.
//

#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/cache.h"
#include "../include/convert.h"
#include "../include/pgm.h"
#include "../include/svd.h"

static int extension(const char *filename, const char *ext) {
	const size_t l1 = strlen(filename), l2 = strlen(ext);
	return (l1 > l2 && strcmp(filename + l1 - l2, ext) == 0);
}

int main(const int argc, char **argv) {
	if (argc != 4) {
		fprintf(stderr, "usage: %s input output.pgm rank\n", argv[0]);
		return 1;
	}

	const char *input = argv[1];
	const char *output = argv[2];
	const int k = atoi(argv[3]);

	PGMImage *image;
	if (extension(input, ".pgm")) {
		image = read_pgm_image(input);
	} else {
		image = convert_to_pgm(input);
	}

	if (!image) {
		fprintf(stderr, "could not read %s\n", input);
		return 2;
	}

	const int m = image->height;
	const int n = image->width;

	double **A = malloc(m * sizeof(double *));
	for (int i = 0; i < m; ++i) {
		A[i] = malloc(n * sizeof(double));
		memcpy(A[i], image->data[i], n * sizeof(double));
	}

	double *S = malloc(k * sizeof(double));
	double **U = malloc(k * sizeof(double *));
	double **V = malloc(k * sizeof(double *));

	for (int r = 0; r < k; ++r) {
		U[r] = malloc(m * sizeof(double));
		V[r] = malloc(n * sizeof(double));
	}

	char *in_dup = strdup(input);
	char *dir = dirname(in_dup);
	const char *base = basename(in_dup);

	char name_no_ext[256];
	strncpy(name_no_ext, base, sizeof(name_no_ext));

	char *dot = strrchr(name_no_ext, '.');
	if (dot) *dot = '\0';

	char cache_file[512];
	snprintf(cache_file, sizeof(cache_file), "%s/%s.cache", dir, name_no_ext);
	free(in_dup);

	const int loaded = load_cache(cache_file, m, n, U, V, S, k);
	printf("loaded %d from cache\n", loaded);

	for (int r = 0; r < loaded; ++r) {
		deflate(m, n, A, U[r], V[r], S[r]);
	}

	for (int r = loaded; r < k; ++r) {
		compute_singular(m, n, A, U[r], V[r], &S[r], 100);
		deflate(m, n, A, U[r], V[r], S[r]);
		printf("sigma[%d] = %f\n", r + 1, S[r]);
	}
	save_cache(cache_file, m, n, U, V, S, k);

	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			double sum = 0;
			for (int r = 0; r < k; ++r) {
				sum += U[r][i] * S[r] * V[r][j];
			}
			image->data[i][j] = sum;
		}
	}

	write_pgm(output, image);

	free_pgm(image);
	for (int i = 0; i < m; ++i) free(A[i]);
	free(A);
	for (int r = 0; r < k; ++r) {
		free(U[r]);
		free(V[r]);
	}
	free(U);
	free(V);
	free(S);

	return 0;
}
