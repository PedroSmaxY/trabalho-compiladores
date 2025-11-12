cmake_minimum_required(VERSION 3.20)
project(trabalho C)

set(CMAKE_C_STANDARD 11)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

include_directories(include ${CMAKE_CURRENT_BINARY_DIR})

# Define caminhos para arquivos gerados
set(BISON_OUTPUT_C ${CMAKE_CURRENT_BINARY_DIR}/parser.c)
set(BISON_OUTPUT_H ${CMAKE_CURRENT_BINARY_DIR}/parser.h)
set(FLEX_OUTPUT_C ${CMAKE_CURRENT_BINARY_DIR}/scanner.c)

# Comando customizado para Bison
add_custom_command(
    OUTPUT ${BISON_OUTPUT_C} ${BISON_OUTPUT_H}
    COMMAND ${BISON_EXECUTABLE} -d -o ${BISON_OUTPUT_C} ${CMAKE_CURRENT_SOURCE_DIR}/src/parser.y
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/parser.y
    COMMENT "Executando Bison em parser.y"
)

# Comando customizado para Flex
add_custom_command(
    OUTPUT ${FLEX_OUTPUT_C}
    COMMAND ${FLEX_EXECUTABLE} -o ${FLEX_OUTPUT_C} ${CMAKE_CURRENT_SOURCE_DIR}/src/scanner.l
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/scanner.l
    COMMENT "Executando Flex em scanner.l"
)

# Target customizado para gerar parser e scanner
add_custom_target(GenerateParserScanner DEPENDS ${BISON_OUTPUT_C} ${BISON_OUTPUT_H} ${FLEX_OUTPUT_C})

# Executável principal
add_executable(main src/main.c src/ast.c src/utils.c src/symbol_table.c src/semantic.c
    ${BISON_OUTPUT_C}
    ${FLEX_OUTPUT_C}
)

# Adiciona dependência para garantir que os arquivos gerados sejam criados antes do main ser compilado
add_dependencies(main GenerateParserScanner)