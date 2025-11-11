#ifndef AST_H
#define AST_H

#include "semantic.h" // Inclui as definições de DataType e Symbol

// Enumeração para todos os tipos de nós possíveis na AST
typedef enum {
    NODE_PROGRAMA,
    NODE_BLOCO,
    NODE_DECL_LISTA,
    NODE_DECLARACAO,
    NODE_TIPO_INTEIRO,
    NODE_TIPO_REAL,
    NODE_ID_LISTA,
    NODE_CMD_LISTA,
    NODE_SE,
    NODE_SE_SENAO,
    NODE_ENQUANTO,
    NODE_REPITA_ATE,
    NODE_LER,
    NODE_MOSTRAR,
    NODE_ATRIBUICAO,
    NODE_ID,
    NODE_NUM_INTEIRO,
    NODE_NUM_REAL,
    NODE_OP_SOMA,
    NODE_OP_SUB,
    NODE_OP_MULT,
    NODE_OP_DIV,
    NODE_OP_AND,
    NODE_OP_OR,
    NODE_OP_LT, // Less Than
    NODE_OP_LE, // Less or Equal
    NODE_OP_GT, // Greater Than
    NODE_OP_GE, // Greater or Equal
    NODE_OP_EQ, // Equal
    NODE_OP_NE,  // Not Equal
    NODE_TYPE_CONVERSION // Novo tipo de nó para conversão de tipo
} NodeType;

// Estrutura do nó da AST
typedef struct Node {
    NodeType type;
    struct Node *left;
    struct Node *right;
    union {
        int ival;
        float fval;
        char *sval;
    } value;
    int line; // Linha do código fonte
    DataType data_type; // Tipo de dado do nó (após análise semântica)
    struct Symbol *symbol_entry; // Ponteiro para a entrada na tabela de símbolos
} Node;

// --- Protótipos de Funções ---

// Funções de criação de nós da AST
Node* createNode(NodeType type, Node* left, Node* right, int line);
Node* createIntNode(int value, int line);
Node* createFloatNode(float value, int line);
Node* createIdNode(const char* sval, int line);
Node* createIfElseNode(Node* condition, Node* if_branch, Node* else_branch, int line);
Node* createAtribuicaoNode(Node* id, Node* expr, int line);
Node* createOpNode(NodeType type, Node* left, Node* right, int line);

// Função principal da análise semântica
void semantic_check(Node *ast_root);

#endif // AST_H