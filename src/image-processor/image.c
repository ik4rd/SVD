//
// Created by ilya on 27.05.2025.
//

#include "../../include/image-processor/image.h"

#include <stdlib.h>

#include "../../include/image-processor/convert.h"
#include "../../include/image-processor/pgm-ppm-parsers/pgm.h"
#include "../../include/image-processor/pgm-ppm-parsers/ppm.h"
#include "../../include/utils.h"

Image *read_image(const char *filename, const int color) {
	Image *image = malloc(sizeof(Image));
	if (color) {
		PPMImage *ppm;
		if (extension(filename, ".ppm"))
			ppm = read_ppm_image(filename);
		else
			ppm = convert_to_ppm(filename);
		if (!ppm) {
			free(image);
			return nullptr;
		}
		image->width = ppm->width;
		image->height = ppm->height;
		image->max = ppm->max;
		image->channels = 3;
		image->data = malloc(3 * sizeof(double **));
		image->data[0] = ppm->R;
		image->data[1] = ppm->G;
		image->data[2] = ppm->B;
		free(ppm);
	} else {
		PGMImage *pgm;
		if (extension(filename, ".pgm")) {
			pgm = read_pgm_image(filename);
		} else {
			pgm = convert_to_pgm(filename);
		}
		if (!pgm) {
			free(image);
			return nullptr;
		}
		image->width = pgm->width;
		image->height = pgm->height;
		image->max = pgm->max;
		image->channels = 1;
		image->data = malloc(1 * sizeof(double **));
		image->data[0] = pgm->data;
		free(pgm);
	}
	return image;
}

int write_image(const char *filename, const Image *image) {
	if (image->channels == 3) {
		const PPMImage temp = {.width = image->width,
							   .height = image->height,
							   .max = image->max,
							   .R = image->data[0],
							   .G = image->data[1],
							   .B = image->data[2]};
		return write_ppm(filename, &temp);
	} else if (image->channels == 1) {
		const PGMImage temp = {.width = image->width,
							   .height = image->height,
							   .max = image->max,
							   .data = image->data[0]};
		return write_pgm(filename, &temp);
	}
	return 1;
}

void free_image(Image *image) {
	for (int c = 0; c < image->channels; ++c) {
		for (int i = 0; i < image->height; ++i) free(image->data[c][i]);
		free(image->data[c]);
	}
	free(image->data);
	free(image);
}
