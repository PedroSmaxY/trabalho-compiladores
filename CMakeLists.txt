cmake_minimum_required(VERSION 3.20)
project(trabalho C)

set(CMAKE_C_STANDARD 11)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

# Include directories
include_directories(include ${CMAKE_CURRENT_BINARY_DIR})

# Bison (parser)
set(BISON_OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.c)
set(BISON_HEADER_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.h)

add_custom_command(
    OUTPUT ${BISON_OUTPUT_FILE} ${BISON_HEADER_FILE}
    COMMAND ${BISON_EXECUTABLE} -o ${BISON_OUTPUT_FILE} --defines=${BISON_HEADER_FILE} -v --report=all ${CMAKE_CURRENT_SOURCE_DIR}/src/parser.y
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/parser.y
    COMMENT "Running Bison on parser.y"
)

# Flex (scanner)
set(FLEX_OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/scanner.c)
add_custom_command(
    OUTPUT ${FLEX_OUTPUT_FILE}
    COMMAND ${FLEX_EXECUTABLE} -o ${FLEX_OUTPUT_FILE} ${CMAKE_CURRENT_SOURCE_DIR}/src/scanner.l
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/scanner.l ${BISON_HEADER_FILE}
    COMMENT "Running Flex on scanner.l"
)

# Add the generated sources to a variable
set(GENERATED_SOURCES
    ${FLEX_OUTPUT_FILE}
    ${BISON_OUTPUT_FILE}
)

# Copy the input file
configure_file(${CMAKE_SOURCE_DIR}/codigo.txt ${CMAKE_BINARY_DIR}/codigo.txt COPYONLY)

# Executable
add_executable(main
    src/main.c
    src/ast.c
    src/utils.c
    ${GENERATED_SOURCES}
)