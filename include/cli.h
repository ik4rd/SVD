//
// Created by ilya on 27.05.2025.
//

#ifndef CLI_H
#define CLI_H

typedef struct {
	int color;
	int cache;
	char *input_path;
	char *output_path;
	int rank;
} CLIOptions;

int parse(int argc, char **argv, CLIOptions *options);

#endif	// CLI_H
