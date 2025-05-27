//
// Created by ilya on 27.05.2025.
//

#include "../include/ppm.h"

#include <stdlib.h>
#include <string.h>

#include "../include/utils.h"

PPMImage *read_ppm_stream(FILE *file) {
	char buffer[256];
	if (!fgets(buffer, sizeof(buffer), file) || strncmp(buffer, "P6", 2) != 0) {
		return nullptr;
	}

	do {
		if (!fgets(buffer, sizeof(buffer), file)) return nullptr;
	} while (buffer[0] == '#');

	PPMImage *image = malloc(sizeof(PPMImage));
	sscanf(buffer, "%d %d", &image->width, &image->height);
	if (!fgets(buffer, sizeof(buffer), file)) {
		free(image);
		return nullptr;
	}
	sscanf(buffer, "%d", &image->max);

	image->R = allocate_matrix(image->height, image->width);
	image->G = allocate_matrix(image->height, image->width);
	image->B = allocate_matrix(image->height, image->width);

	unsigned char *row = malloc(image->width * 3);
	for (int i = 0; i < image->height; ++i) {
		fread(row, 1, image->width * 3, file);
		for (int j = 0; j < image->width; ++j) {
			image->R[i][j] = row[3 * j + 0] / (double) image->max;
			image->G[i][j] = row[3 * j + 1] / (double) image->max;
			image->B[i][j] = row[3 * j + 2] / (double) image->max;
		}
	}

	free(row);
	return image;
}

PPMImage *read_ppm_image(const char *filename) {
	FILE *file = fopen(filename, "rb");
	if (!file) return nullptr;

	PPMImage *image = read_ppm_stream(file);

	fclose(file);
	return image;
}

int write_ppm(const char *filename, const PPMImage *image) {
	FILE *file = fopen(filename, "wb");
	if (!file) return 1;

	fprintf(file, "P6\n%d %d\n%d\n", image->width, image->height, image->max);

	unsigned char *row = malloc(image->width * 3);
	for (int i = 0; i < image->height; ++i) {
		for (int j = 0; j < image->width; ++j) {
			int r = (int) (image->R[i][j] * image->max + 0.5);
			int g = (int) (image->G[i][j] * image->max + 0.5);
			int b = (int) (image->B[i][j] * image->max + 0.5);
			if (r < 0) r = 0;
			if (r > image->max) r = image->max;
			if (g < 0) g = 0;
			if (g > image->max) g = image->max;
			if (b < 0) b = 0;
			if (b > image->max) b = image->max;
			row[3 * j + 0] = (unsigned char) r;
			row[3 * j + 1] = (unsigned char) g;
			row[3 * j + 2] = (unsigned char) b;
		}
		fwrite(row, 3, image->width, file);
	}
	free(row);
	fclose(file);
	return 0;
}

void free_ppm(PPMImage *image) {
	for (int i = 0; i < image->height; ++i) {
		free(image->R[i]);
		free(image->G[i]);
		free(image->B[i]);
	}
	free(image->R);
	free(image->G);
	free(image->B);
	free(image);
}
