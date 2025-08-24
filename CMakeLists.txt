cmake_minimum_required(VERSION 4.0)
project(trabalho C)

set(CMAKE_C_STANDARD 11)

configure_file(${CMAKE_SOURCE_DIR}/codigo.pm ${CMAKE_BINARY_DIR}/codigo.pm COPYONLY)

include_directories(include)

add_executable(main src/main.c
        src/lexer.c)