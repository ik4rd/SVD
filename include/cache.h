//
// Created by ilya on 26.05.2025.
//

#ifndef CACHE_H
#define CACHE_H

int load_cache(const char *cache_filename, int m, int n, double **U, double **V,
			   double *S, int max_k);
int save_cache(const char *cache_filename, int m, int n, double **U, double **V,
			   const double *S, int k);

#endif	// CACHE_H
