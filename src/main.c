//
// Created by ilya on 26.05.2025.
//

#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/cache.h"
#include "../include/cli.h"
#include "../include/image-processor/convert.h"
#include "../include/image-processor/image.h"
#include "../include/linear-algebra/svd.h"
#include "../include/utils.h"

int main(const int argc, char **argv) {
	CLIOptions options;
	if (parse(argc, argv, &options) != 0) {
		return 1;
	}

	Image *image = read_image(options.input_path, options.color);
	if (!image) {
		fprintf(stderr, "failed to read image '%s'\n", options.input_path);
		return 2;
	}

	const int m = image->height;
	const int n = image->width;
	const int k = options.rank;

	char *in_dup = strdup(options.input_path);
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
		snprintf(cache_file, sizeof(cache_file),
				 "%s/.cache-svd/%s_channel%d.cache", dir, name_no_ext, c);

		int loaded = 0;
		if (options.cache) {
			loaded = load_cache(cache_file, m, n, U, V, S, k);
			printf("channel %d: loaded %d from cache\n", c + 1, loaded);
		}

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

		if (options.cache) {
			if (save_cache(cache_file, m, n, U, V, S, k) != 0) {
				fprintf(stderr, "warning: failed to save cache to '%s'\n",
						cache_file);
			}
		}

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

	if (write_image(options.output_path, image)) {
		fprintf(stderr, "failed to write image '%s'\n", options.output_path);
		free_image(image);
		return 3;
	}

	free_image(image);
	free(options.input_path);
	free(options.output_path);
	return 0;
}
