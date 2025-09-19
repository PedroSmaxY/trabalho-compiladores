#ifndef AST_H
#define AST_H

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
    NODE_OP_NE  // Not Equal
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
} Node;

Node* createNode(NodeType type, Node* left, Node* right);
Node* createIntNode(int value);
Node* createFloatNode(float value);
Node* createIdNode(const char* sval);
Node* createIfElseNode(Node* condition, Node* if_branch, Node* else_branch);
Node* createAtribuicaoNode(Node* id, Node* expr);
Node* createOpNode(NodeType type, Node* left, Node* right);

void printAst(const Node* node, int level);
void printSemanticAst(const Node* node, int level);

#endif // AST_H