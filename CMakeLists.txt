cmake_minimum_required(VERSION 4.0)
project(trabalho C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(trabalho_compiladores src/main.c
        src/lexer.c)