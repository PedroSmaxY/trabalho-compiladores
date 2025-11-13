cmake_minimum_required(VERSION 3.20)
project(trabalho C)

set(CMAKE_C_STANDARD 11)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

include_directories(include ${CMAKE_CURRENT_BINARY_DIR})

# Define paths for generated files
set(BISON_OUTPUT_C ${CMAKE_CURRENT_BINARY_DIR}/parser.c)
set(BISON_OUTPUT_H ${CMAKE_CURRENT_BINARY_DIR}/parser.h)
set(FLEX_OUTPUT_C ${CMAKE_CURRENT_BINARY_DIR}/scanner.c)

# Custom command for Bison
add_custom_command(
    OUTPUT ${BISON_OUTPUT_C} ${BISON_OUTPUT_H}
    COMMAND ${BISON_EXECUTABLE} -d -o ${BISON_OUTPUT_C} ${CMAKE_CURRENT_SOURCE_DIR}/src/parser.y
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/parser.y
    COMMENT "Running Bison on parser.y"
)

# Custom command for Flex
add_custom_command(
    OUTPUT ${FLEX_OUTPUT_C}
    COMMAND ${FLEX_EXECUTABLE} -o ${FLEX_OUTPUT_C} ${CMAKE_CURRENT_SOURCE_DIR}/src/scanner.l
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/scanner.l
    COMMENT "Running Flex on scanner.l"
)

# Custom target to drive the generation
add_custom_target(GenerateParserScanner DEPENDS ${BISON_OUTPUT_C} ${BISON_OUTPUT_H} ${FLEX_OUTPUT_C})

# Main executable
add_executable(main src/main.c src/ast.c src/utils.c src/symbol_table.c src/semantic.c
    ${BISON_OUTPUT_C}
    ${FLEX_OUTPUT_C}
)

# Add dependency to ensure generated files are created before main is built
add_dependencies(main GenerateParserScanner)

configure_file(${CMAKE_SOURCE_DIR}/codigo.p- ${CMAKE_BINARY_DIR}/codigo.p- COPYONLY)