//
// Created by ilya on 27.05.2025.
//

#ifndef IMAGE_H
#define IMAGE_H

typedef struct {
	int width, height;
	int max;
	int channels;
	double ***data;
} Image;

Image *read_image(const char *filename, int color);

int write_image(const char *filename, const Image *image);
void free_image(Image *image);

#endif	// IMAGE_H
