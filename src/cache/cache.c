//
// Created by ilya on 26.05.2025.
//

#include "cache/cache.h"

#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/stat.h>

static int ensure_parent_dir(const char *filename) {
	char *dup = strdup(filename);
	if (!dup) return -1;

	const char *dir = dirname(dup);

	struct stat st;
	if (stat(dir, &st) == -1) {
		if (mkdir(dir, 0755) == -1 && errno != EEXIST) {
			free(dup);
			return -1;
		}
	}

	free(dup);
	return 0;
}

int load_cache(const char *cache_filename, const int m, const int n, double **U,
			   double **V, double *S, const int max_k) {
	FILE *file = fopen(cache_filename, "rb");
	if (!file) return 0;

	int cm, cn, ck;
	if (fread(&cm, sizeof(int), 1, file) != 1 ||
		fread(&cn, sizeof(int), 1, file) != 1 ||
		fread(&ck, sizeof(int), 1, file) != 1) {
		fclose(file);
		return 0;
	}
	if (cm != m || cn != n) {
		fclose(file);
		return 0;
	}

	const int r = (ck < max_k ? ck : max_k);

	if (fread(S, sizeof(double), r, file) != (size_t) r) {
		fclose(file);
		return 0;
	}
	for (int i = 0; i < r; ++i) {
		if (fread(U[i], sizeof(double), m, file) != (size_t) m) {
			fclose(file);
			return 0;
		}
	}
	for (int i = 0; i < r; ++i) {
		if (fread(V[i], sizeof(double), n, file) != (size_t) n) {
			fclose(file);
			return 0;
		}
	}

	fclose(file);
	return r;
}

int save_cache(const char *cache_filename, const int m, const int n, double **U,
			   double **V, const double *S, const int k) {
	if (ensure_parent_dir(cache_filename) != 0) {
		perror("save_cache: cannot create cache dir");
		return 1;
	}

	FILE *file = fopen(cache_filename, "wb");
	if (!file) {
		perror("save_cache: fopen");
		return 1;
	}

	if (fwrite(&m, sizeof(int), 1, file) != 1 ||
		fwrite(&n, sizeof(int), 1, file) != 1 ||
		fwrite(&k, sizeof(int), 1, file) != 1) {
		perror("save_cache: header write");
		fclose(file);
		return 1;
	}

	if (fwrite(S, sizeof(double), k, file) != (size_t) k) {
		perror("save_cache: S write");
		fclose(file);
		return 1;
	}
	for (int i = 0; i < k; ++i) {
		if (fwrite(U[i], sizeof(double), m, file) != (size_t) m) {
			perror("save_cache: U write");
			fclose(file);
			return 1;
		}
	}
	for (int i = 0; i < k; ++i) {
		if (fwrite(V[i], sizeof(double), n, file) != (size_t) n) {
			perror("save_cache: V write");
			fclose(file);
			return 1;
		}
	}

	fclose(file);
	return 0;
}
