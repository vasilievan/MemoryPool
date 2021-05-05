cmake_minimum_required(VERSION 3.19)
project(laba5 C)

set(CMAKE_C_STANDARD 99)

add_executable(laba5 src/main.c src/cell.h src/memoryPool.h src/memoryPool.c)