cmake_minimum_required(VERSION 4.0)
project(trabalho C)

set(CMAKE_C_STANDARD 11)

# Copia arquivo de entrada para o build
configure_file(${CMAKE_SOURCE_DIR}/codigo.pm ${CMAKE_BINARY_DIR}/codigo.pm COPYONLY)

# Diretório de headers
include_directories(include)

# Biblioteca do lexer
add_library(lexer STATIC src/lexer.c)
target_include_directories(lexer PUBLIC include)

# Executável principal
add_executable(main src/main.c)
target_link_libraries(main PRIVATE lexer)

add_executable(test_lexer tests/test_lexer.c)
target_link_libraries(test_lexer PRIVATE lexer)
target_include_directories(test_lexer PRIVATE include)