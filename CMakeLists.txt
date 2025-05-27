cmake_minimum_required(VERSION 3.30)
project(SVD C)

set(CMAKE_C_STANDARD 23)

add_executable(svd
        src/image-processor/pgm-ppm-parsers/pgm.c
        include/image-processor/pgm-ppm-parsers/pgm.h
        src/image-processor/convert.c
        include/image-processor/convert.h
        src/linear-algebra/svd.c
        include/linear-algebra/svd.h
        src/main.c
        src/cache.c
        include/cache.h
        include/image-processor/pgm-ppm-parsers/ppm.h
        src/image-processor/pgm-ppm-parsers/ppm.c
        include/utils.h
        src/utils.c
        include/image-processor/image.h
        src/image-processor/image.c
        include/cli.h
        src/cli.c
)
