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


add_executable(main src/main.c src/ast.c
        ${FLEX_Scanner_OUTPUTS}
        ${BISON_Parser_OUTPUTS}
)

enable_testing()

add_executable(test_lexer tests/test_lexer.c
        ${FLEX_Scanner_OUTPUTS}
)
target_include_directories(test_lexer PRIVATE include)

add_test(NAME lexer COMMAND test_lexer)
