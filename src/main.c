//
// Created by ilya on 26.05.2025.
//

#include <stdlib.h>
#include <string.h>

#include "../include/cache.h"
#include "../include/cli.h"
#include "../include/image-processor/convert.h"
#include "../include/image-processor/image.h"
#include "../include/processor.h"
#include "../include/utils.h"

int main(const int argc, char **argv) {
	CLIOptions options;
	if (parse(argc, argv, &options) != 0) {
		return 1;
	}

	Image *image = read_image(options.input_path, options.color);
	if (!image) {
		fprintf(stderr, "failed to read image '%s'\n", options.input_path);
		return 2;
	}

	char *dir, *name_no_ext;
	if (split_path_no_ext(options.input_path, &dir, &name_no_ext) != 0) {
		fprintf(stderr, "path split failed\n");
		return 3;
	}

	char *cache_dir = build_cache_dir(dir);

	for (int channel = 0; channel < image->channels; ++channel) {
		process_channel(channel, image, cache_dir, name_no_ext, &options);
	}

	if (write_image(options.output_path, image)) {
		fprintf(stderr, "failed to write image '%s'\n", options.output_path);
		free_image(image);
		return 4;
	}

	free(dir);
	free(name_no_ext);
	free(cache_dir);
	free(options.input_path);
	free(options.output_path);
	free_image(image);

	return 0;
}
