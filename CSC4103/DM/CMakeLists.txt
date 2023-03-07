cmake_minimum_required(VERSION 3.24)
project(DM C)

set(CMAKE_C_STANDARD 11)

include_directories(.)


add_library(libcmocka STATIC IMPORTED)
set_property(TARGET libcmocka PROPERTY IMPORTED_LOCATION "C:\\Users\\charl\\Sources\\coursTSP\\CSC4103\\DM\\cmocka\\cmocka.lib")



add_executable(DM
        memory_alloc.c
        memory_alloc.h)
target_link_libraries(DM libcmocka)
