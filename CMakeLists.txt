cmake_minimum_required(VERSION 4.1)
project(CompSysA0 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_library(common OBJECT
        coord_query.c
        coord_query.h
        id_query.c
        id_query.h
        record.c
        record.h
        timing.h)

add_executable(coord_query_naive coord_query_naive.c)
target_link_libraries(coord_query_naive PRIVATE common)

add_executable(id_query_naive id_query_naive.c)
target_link_libraries(id_query_naive PRIVATE common)

add_executable(random_ids random_ids.c)
target_link_libraries(random_ids PRIVATE common)

add_executable(id_query_indexed id_query_indexed.c)
target_link_libraries(id_query_indexed PRIVATE common)

add_executable(id_query_binsort id_query_binsort.c)
target_link_libraries(id_query_binsort PRIVATE common)