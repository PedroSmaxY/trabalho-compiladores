cmake_minimum_required(VERSION 4.0)
project(trabalho C)

set(CMAKE_C_STANDARD 11)

configure_file(${CMAKE_SOURCE_DIR}/codigo.txt ${CMAKE_BINARY_DIR}/codigo.txt COPYONLY)

include_directories(include)

add_executable(main src/main.c
        src/lexer.c)