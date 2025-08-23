cmake_minimum_required(VERSION 4.0)
project(trabalho C)

set(CMAKE_C_STANDARD 11)

add_executable(trabalho src/main.c
        src/lexer.c
        include/lexer.h)
