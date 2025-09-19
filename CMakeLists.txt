cmake_minimum_required(VERSION 3.20)
project(trabalho C)

set(CMAKE_C_STANDARD 11)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

FLEX_TARGET(Scanner src/scanner.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.c)
BISON_TARGET(Parser src/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.c DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.h)

include_directories(include)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

configure_file(${CMAKE_SOURCE_DIR}/codigo.txt ${CMAKE_BINARY_DIR}/codigo.txt COPYONLY)


add_executable(main src/main.c src/ast.c src/utils.c
        ${FLEX_Scanner_OUTPUTS}
        ${BISON_Parser_OUTPUTS}
        src/utils.c
)