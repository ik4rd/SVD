//
// Created by ilya on 27.05.2025.
//

#include "core/cli.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *build_output_path(const char *output_base, const int color) {
	const char *ext = color ? ".ppm" : ".pgm";
	const size_t length = strlen(output_base) + strlen(ext) + 1;

	char *res = malloc(length);
	if (!res) return nullptr;

	strcpy(res, output_base);
	strcat(res, ext);

	return res;
}

int parse(const int argc, char **argv, CLIOptions *options) {
	options->color = 0;
	options->cache = 0;
	options->input_path = nullptr;
	options->output_path = nullptr;
	options->rank = 1;

	static struct option long_options[] = {
			{"color", required_argument, nullptr, 0},
			{"cache", required_argument, nullptr, 0},
			{nullptr, 0, nullptr, 0}};

	int index = 0;
	int c;

	while ((c = getopt_long(argc, argv, "", long_options, &index)) != -1) {
		if (c == 0) {
			const char *name = long_options[index].name;
			if (strcmp(name, "color") == 0) {
				options->color = atoi(optarg);
			} else if (strcmp(name, "cache") == 0) {
				options->cache = atoi(optarg);
			}
		} else {
			fprintf(stderr, "unknown option\n");
			return -1;
		}
	}

	if (optind + 3 != argc) {
		fprintf(stderr,
				"usage: %s [--color=0|1] [--cache=0|1] input.ext output rank\n",
				argv[0]);
		return -1;
	}

	options->input_path = strdup(argv[optind + 0]);

	const char *out_base = argv[optind + 1];
	options->output_path = build_output_path(out_base, options->color);

	if (!options->output_path) {
		perror("malloc");
		return -1;
	}

	options->rank = atoi(argv[optind + 2]);

	return 0;
}
