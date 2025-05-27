//
// Created by ilya on 26.05.2025.
//

#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/cache.h"
#include "../include/convert.h"
#include "../include/image.h"
#include "../include/svd.h"
#include "../include/utils.h"

int main(const int argc, char **argv) {
	int color = 0, k = 0;
	const char *input;
	const char *output;

	int argi = 1;
	if (argi < argc && strncmp(argv[argi], "--color=", 8) == 0) {
		color = atoi(argv[argi] + 8);
		argi++;
	}
	if (argi + 3 != argc) {
		fprintf(stderr, "usage: %s [--color=1] input output rank\n", argv[0]);
		return 1;
	}

	input = argv[argi++];
	output = argv[argi++];
	k = atoi(argv[argi]);
	if (k <= 0) {
		fprintf(stderr, "invalid rank '%s'\n", argv[argi]);
	}

	Image *image = read_image(input, color);
	if (!image) {
		fprintf(stderr, "failed to read image '%s'\n", input);
		return 2;
	}

	const int m = image->height;
	const int n = image->width;

	char *in_dup = strdup(input);
	char *dir = dirname(in_dup);
	const char *base = basename(in_dup);

	char name_no_ext[256];
	strncpy(name_no_ext, base, sizeof(name_no_ext));

	char *dot = strrchr(name_no_ext, '.');
	if (dot) *dot = '\0';

	for (int c = 0; c < image->channels; ++c) {
		double **A = image->data[c];
		double **B = allocate_matrix(m, n);
		double *S = malloc(k * sizeof(double));
		double **U = malloc(k * sizeof(double *));
		double **V = malloc(k * sizeof(double *));
		for (int r = 0; r < k; ++r) {
			U[r] = malloc(m * sizeof(double));
			V[r] = malloc(n * sizeof(double));
		}

		char cache_file[512];
		snprintf(cache_file, sizeof(cache_file), "%s/%s_channel%d.cache", dir,
				 name_no_ext, c);

		const int loaded = load_cache(cache_file, m, n, U, V, S, k);
		printf("channel %d: loaded %d from cache\n", c, loaded);

		for (int r = 0; r < loaded; ++r) {
			for (int i = 0; i < m; ++i)
				for (int j = 0; j < n; ++j) B[i][j] += S[r] * U[r][i] * V[r][j];
			deflate(m, n, A, U[r], V[r], S[r]);
		}

		for (int r = loaded; r < k; ++r) {
			compute_singular(m, n, A, U[r], V[r], &S[r], 100);
			for (int i = 0; i < m; ++i)
				for (int j = 0; j < n; ++j) B[i][j] += S[r] * U[r][i] * V[r][j];
			deflate(m, n, A, U[r], V[r], S[r]);
			printf("channel %d sigma[%d] = %f\n", c + 1, r + 1, S[r]);
		}

		save_cache(cache_file, m, n, U, V, S, k);

		for (int r = 0; r < k; ++r) {
			free(U[r]);
			free(V[r]);
		}
		free(U);
		free(V);
		free(S);

		for (int i = 0; i < m; ++i) {
			free(A[i]);
		}
		free(A);
		image->data[c] = B;
	}

	free(in_dup);

	if (write_image(output, image)) {
		fprintf(stderr, "failed to write image '%s'\n", output);
		free_image(image);
		return 3;
	}

	free_image(image);
	return 0;
}
