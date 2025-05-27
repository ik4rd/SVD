//
// Created by ilya on 27.05.2025.
//

#include "core/processor.h"

#include <stdio.h>
#include <stdlib.h>

#include "core/utils.h"
#include "linear-algebra/svd.h"

void process_channel(int channel, Image *image, const char *cache_dir,
					 const char *name_no_ext, const CLIOptions *options) {
	const int m = image->height;
	const int n = image->width;
	const int k = options->rank;

	double **A = image->data[channel];
	double **B = allocate_matrix(m, n);
	double *S = malloc(k * sizeof(double));
	double **U = malloc(k * sizeof(double *));
	double **V = malloc(k * sizeof(double *));

	for (int i = 0; i < k; ++i) {
		U[i] = malloc(m * sizeof(double));
		V[i] = malloc(n * sizeof(double));
	}

	char *cache_file = build_cache_filepath(cache_dir, name_no_ext, channel);

	int loaded = 0;
	if (options->cache) {
		loaded = load_cache(cache_file, m, n, U, V, S, k);
		printf("channel %d: loaded %d from cache\n", channel + 1, loaded);
	}

	for (int r = 0; r < loaded; ++r) {
		for (int i = 0; i < m; ++i) {
			for (int j = 0; j < n; ++j) {
				B[i][j] += S[r] * U[r][i] * V[r][j];
			}
		}
		deflate(m, n, A, U[r], V[r], S[r]);
	}

	for (int r = loaded; r < k; ++r) {
		compute_singular(m, n, A, U[r], V[r], &S[r], 100);
		for (int i = 0; i < m; ++i) {
			for (int j = 0; j < n; ++j) {
				B[i][j] += S[r] * U[r][i] * V[r][j];
			}
		}
		deflate(m, n, A, U[r], V[r], S[r]);
		printf("channel %d sigma[%d] = %f\n", channel + 1, r + 1, S[r]);
	}

	if (options->cache) {
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
	image->data[channel] = B;

	free(cache_file);
}
