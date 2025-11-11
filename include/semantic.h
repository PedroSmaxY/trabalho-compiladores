#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <stdbool.h>

// Enumeração para os tipos de dados da linguagem P-
typedef enum {
    TYPE_UNDEFINED,
    TYPE_INTEGER,
    TYPE_REAL,
    TYPE_BOOLEAN
} DataType;

// Estrutura para um símbolo na tabela de símbolos
typedef struct Symbol {
    char *name;
    DataType type;
    int memory_address;
    int line;
    struct Symbol *next;
} Symbol;

// Estrutura para a tabela de símbolos (escopo)
typedef struct SymbolTable {
    char *scope_name;
    struct SymbolTable *parent;
    Symbol *symbols;
    int current_address;
} SymbolTable;

// Funções de gerenciamento da tabela de símbolos
SymbolTable* create_symbol_table(char* scope_name, SymbolTable* parent);
void free_symbol_table(SymbolTable* table);
void insert_symbol(SymbolTable* table, const char* name, DataType type, int line);
Symbol* lookup_symbol(SymbolTable* table, const char* name);

// Funções auxiliares
const char* datatype_to_string(DataType type);
void print_symbol_table(SymbolTable* table);

#endif //SEMANTIC_H