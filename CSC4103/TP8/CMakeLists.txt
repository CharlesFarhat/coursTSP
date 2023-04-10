cmake_minimum_required(VERSION 3.25)
project(TP8 C)

set(CMAKE_C_STANDARD 17)

find_package(Threads REQUIRED)

add_executable(memory memory.c)

add_executable(sem sem.c)
target_link_libraries(sem PRIVATE Threads::Threads)

add_executable(bank bank.c)
target_link_libraries(bank PRIVATE Threads::Threads)