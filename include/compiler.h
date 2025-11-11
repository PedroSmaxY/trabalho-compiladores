#ifndef COMPILER_H
#define COMPILER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// --- From semantic.h ---

typedef enum {
    TYPE_UNDEFINED,
    TYPE_INTEGER,
    TYPE_REAL,
    TYPE_BOOLEAN
} DataType;

typedef struct Symbol {
    char *name;
    DataType type;
    int memory_address;
    int line;
    struct Symbol *next;
} Symbol;

typedef struct SymbolTable {
    char *scope_name;
    struct SymbolTable *parent;
    Symbol *symbols;
    int current_address;
} SymbolTable;

// --- From ast.h ---

typedef enum {
    NODE_PROGRAMA, NODE_BLOCO, NODE_DECL_LISTA, NODE_DECLARACAO, NODE_TIPO_INTEIRO,
    NODE_TIPO_REAL, NODE_ID_LISTA, NODE_CMD_LISTA, NODE_SE, NODE_SE_SENAO,
    NODE_ENQUANTO, NODE_REPITA_ATE, NODE_LER, NODE_MOSTRAR, NODE_ATRIBUICAO,
    NODE_ID, NODE_NUM_INTEIRO, NODE_NUM_REAL, NODE_OP_SOMA, NODE_OP_SUB,
    NODE_OP_MULT, NODE_OP_DIV, NODE_OP_AND, NODE_OP_OR, NODE_OP_LT,
    NODE_OP_LE, NODE_OP_GT, NODE_OP_GE, NODE_OP_EQ, NODE_OP_NE,
    NODE_TYPE_CONVERSION
} NodeType;

typedef struct Node {
    NodeType type;
    struct Node *left;
    struct Node *right;
    union {
        int ival;
        float fval;
        char *sval;
    } value;
    int line;
    DataType data_type;
    struct Symbol *symbol_entry;
} Node;

// --- Function Prototypes ---

// From ast.c
Node* createNode(NodeType type, Node* left, Node* right, int line);
Node* createIntNode(int value, int line);
Node* createFloatNode(float value, int line);
Node* createIdNode(const char* sval, int line);
Node* createIfElseNode(Node* condition, Node* if_branch, Node* else_branch, int line);
Node* createAtribuicaoNode(Node* id, Node* expr, int line);
Node* createOpNode(NodeType type, Node* left, Node* right, int line);

// From semantic.c
void semantic_check(Node *ast_root);
SymbolTable* create_symbol_table(char* scope_name, SymbolTable* parent);
void free_symbol_table(SymbolTable* table);
void insert_symbol(SymbolTable* table, const char* name, DataType type, int line);
Symbol* lookup_symbol(SymbolTable* table, const char* name);
const char* datatype_to_string(DataType type);
void print_symbol_table(SymbolTable* table);

// From utils.c
const char* get_token_category(int token);
void printAst(const Node* node, int level);
void printSemanticAst(const Node* node, int level);

// From parser.y
int yyparse(void);

// From scanner.l
int yylex(void);

// --- Global Variables ---
extern Node *ast_root;
extern SymbolTable *global_symbol_table;
extern int yylineno;
extern char* yytext;

#endif // COMPILER_H
