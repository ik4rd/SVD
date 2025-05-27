//
// Created by ilya on 27.05.2025.
//

#include "core/utils.h"

#include <libgen.h>
#include <stdio.h>
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

int split_path_no_ext(const char *fullpath, char **out_dir, char **out_name) {
	char *dup = strdup(fullpath);
	if (!dup) return -1;

	const char *dir = dirname(dup);
	*out_dir = strdup(dir);

	free(dup);

	dup = strdup(fullpath);
	if (!dup) {
		free(*out_dir);
		return -1;
	}

	const char *base = basename(dup);
	char *dot = strrchr(base, '.');
	if (dot) *dot = '\0';

	*out_name = strdup(base);

	free(dup);

	return (*out_dir && *out_name) ? 0 : 1;
}

char *build_cache_dir(const char *dir) {
	size_t length = strlen(dir) + 12 + 1; /*  "/.cache-svd" + 1  */

	char *res = malloc(length);
	if (!res) return nullptr;

	snprintf(res, length, "%s/.cache-svd", dir);

	return res;
}

char *build_cache_filepath(const char *cache_dir, const char *name_no_ext,
						   const int channel) {
	size_t length = strlen(cache_dir) + 1 + strlen(name_no_ext) + 16 + 1;

	char *res = malloc(length);
	if (!res) return nullptr;

	snprintf(res, length, "%s/%s_channel%d.cache", cache_dir, name_no_ext,
			 channel);

	return res;
}
