cmake_minimum_required(VERSION 3.30)
project(SVD C)

set(CMAKE_C_STANDARD 23)

add_executable(svd
        src/pgm.c
        include/pgm.h
        src/convert.c
        include/convert.h
        src/svd.c
        include/svd.h
        src/main.c
        src/cache.c
        include/cache.h
)
