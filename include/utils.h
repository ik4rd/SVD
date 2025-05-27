//
// Created by ilya on 27.05.2025.
//

#ifndef UTILS_H
#define UTILS_H

double **allocate_matrix(const int height, const int width);
int extension(const char *filename, const char *ext);

int split_path_no_ext(const char *fullpath, char **out_dir, char **out_name);
char *build_cache_dir(const char *dir);
char *build_cache_filepath(const char *cache_dir, const char *name_no_ext,
						   int channel);

#endif	// UTILS_H
