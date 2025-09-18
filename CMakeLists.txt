cmake_minimum_required(VERSION 3.20)
project(trabalho C)

set(CMAKE_C_STANDARD 11)

# Copia arquivo de entrada para o build
configure_file(${CMAKE_SOURCE_DIR}/codigo.txt ${CMAKE_BINARY_DIR}/codigo.txt COPYONLY)

# Diretório de headers
include_directories(include)

# Biblioteca de utilitários
add_library(utils src/utils.c include/utils.h)
target_include_directories(utils PUBLIC include)

# Biblioteca do lexer
add_library(lexer STATIC src/lexer.c)
target_include_directories(lexer PUBLIC include)
target_link_libraries(lexer PRIVATE utils)

# Executável principal
add_executable(main src/main.c)
target_link_libraries(main PRIVATE utils)
target_link_libraries(main PRIVATE lexer)

enable_testing()

add_executable(test_lexer tests/test_lexer.c)
target_link_libraries(test_lexer PRIVATE lexer)
target_include_directories(test_lexer PRIVATE include)

add_test(NAME lexer COMMAND test_lexer)
