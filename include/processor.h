//
// Created by ilya on 27.05.2025.
//

#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "cache/cache.h"
#include "cli.h"
#include "image-processor/image.h"

void process_channel(int channel, Image *image, const char *cache_dir,
					 const char *name_no_ext, const CLIOptions *options);

#endif	// PROCESSOR_H
