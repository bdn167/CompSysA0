cmake_minimum_required(VERSION 4.1)
project(CompSysA0 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(CompSysA0
        coord_query.c
        coord_query.h
        coord_query_naive.c
        id_query.c
        id_query.h
        id_query_naive.c
        random_ids.c
        record.c
        record.h
        timing.h)
