//
// Created by ilya on 26.05.2025.
//

#include "../include/svd.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

static double dot(const int n, const double *x, const double *y) {
	double s = 0;
	for (int i = 0; i < n; ++i) s += x[i] * y[i];
	return s;
}

static double norm(const int n, const double *x) { return sqrt(dot(n, x, x)); }

static void normalise(const int n, double *x) {
	const double v = norm(n, x);
	if (v == 0) return;
	for (int i = 0; i < n; ++i) x[i] /= v;
}

void compute_singular(const int m, const int n, double **A, double *u,
					  double *v, double *sigma, const int iters) {
	for (int j = 0; j < n; ++j) v[j] = rand() / (double) RAND_MAX;
	normalise(n, v);

	double *w = malloc(m * sizeof(double));
	double *z = malloc(n * sizeof(double));

	for (int it = 0; it < iters; ++it) {
		for (int i = 0; i < m; ++i) {
			w[i] = 0;
			for (int j = 0; j < n; ++j) w[i] += A[i][j] * v[j];
		}
		normalise(m, w);
		for (int j = 0; j < n; ++j) {
			z[j] = 0;
			for (int i = 0; i < m; ++i) z[j] += A[i][j] * w[i];
		}
		normalise(n, z);
		memcpy(u, w, m * sizeof(double));
		memcpy(v, z, n * sizeof(double));
	}

	for (int i = 0; i < m; ++i) w[i] = 0;
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			w[i] += A[i][j] * v[j];
		}
	}

	*sigma = norm(m, w);

	free(w);
	free(z);
}

void deflate(const int m, const int n, double **A, const double *u,
			 const double *v, const double sigma) {
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			A[i][j] -= u[i] * sigma * v[j];
		}
	}
}
