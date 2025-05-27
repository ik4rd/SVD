//
// Created by ilya on 26.05.2025.
//

#include "image-processor/convert.h"

#include <stdio.h>
#include <stdlib.h>

PGMImage *convert_to_pgm(const char *filename) {
	char cmd[512];
	snprintf(cmd, sizeof(cmd), "magick '%s' -colorspace Gray PGM:-", filename);

	FILE *pipe = popen(cmd, "r");
	if (!pipe) {
		perror("popen");
		return nullptr;
	}

	PGMImage *image = read_pgm_stream(pipe);

	pclose(pipe);
	return image;
}

PPMImage *convert_to_ppm(const char *filename) {
	char cmd[512];
	snprintf(cmd, sizeof(cmd), "magick '%s' PPM:-", filename);

	FILE *pipe = popen(cmd, "r");
	if (!pipe) {
		perror("popen");
		return nullptr;
	}

	PPMImage *image = read_ppm_stream(pipe);

	pclose(pipe);
	return image;
}
