//
// Created by ilya on 26.05.2025.
//

#ifndef CONVERT_H
#define CONVERT_H

#include "pgm-ppm-parsers/pgm.h"
#include "pgm-ppm-parsers/ppm.h"

PGMImage *convert_to_pgm(const char *filename);
PPMImage *convert_to_ppm(const char *filename);

#endif	// CONVERT_H
