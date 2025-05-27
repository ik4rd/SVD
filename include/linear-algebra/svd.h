//
// Created by ilya on 26.05.2025.
//

#ifndef SVD_H
#define SVD_H

void compute_singular(int m, int n, double **A, double *u, double *v,
					  double *sigma, int iters);
void deflate(int m, int n, double **A, const double *u, const double *v,
			 double sigma);

#endif	// SVD_H
