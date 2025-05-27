//
// Created by ilya on 26.05.2025.
//

#include "../../../include/image-processor/pgm-ppm-parsers/pgm.h"

#include <stdlib.h>
#include <string.h>

#include "../../../include/core/utils.h"

PGMImage *read_pgm_stream(FILE *file) {
	char buffer[256];
	if (!fgets(buffer, sizeof(buffer), file) || strncmp(buffer, "P5", 2) != 0) {
		return nullptr;
	}

	do {
		if (!fgets(buffer, sizeof(buffer), file)) return nullptr;
	} while (buffer[0] == '#');

	PGMImage *image = malloc(sizeof(PGMImage));
	sscanf(buffer, "%d %d", &image->width, &image->height);
	if (!fgets(buffer, sizeof(buffer), file)) {
		free(image);
		return nullptr;
	}
	sscanf(buffer, "%d", &image->max);

	image->data = allocate_matrix(image->height, image->width);
	unsigned char *row = malloc(image->width);
	for (int i = 0; i < image->height; ++i) {
		fread(row, 1, image->width, file);
		for (int j = 0; j < image->width; ++j) {
			image->data[i][j] = row[j] / (double) image->max;
		}
	}

	free(row);
	return image;
}

PGMImage *read_pgm_image(const char *filename) {
	FILE *file = fopen(filename, "rb");
	if (!file) return nullptr;

	PGMImage *image = read_pgm_stream(file);

	fclose(file);
	return image;
}

int write_pgm(const char *filename, const PGMImage *image) {
	FILE *file = fopen(filename, "wb");
	if (!file) return 1;

	fprintf(file, "P5\n%d %d\n%d\n", image->width, image->height, image->max);

	unsigned char *row = malloc(image->width);
	for (int i = 0; i < image->height; ++i) {
		for (int j = 0; j < image->width; ++j) {
			int v = (int) (image->data[i][j] * image->max + 0.5);
			if (v < 0) v = 0;
			if (v > image->max) v = image->max;
			row[j] = v;
		}
		fwrite(row, 1, image->width, file);
	}

	free(row);
	fclose(file);
	return 0;
}

void free_pgm(PGMImage *image) {
	for (int i = 0; i < image->height; ++i) {
		free(image->data[i]);
	}
	free(image->data);
	free(image);
}
