//
// Created by Pedro Novais on 18/09/25.
//

#include "compiler.h"
#include <string.h>

// Função genérica para criar um nó
Node* createNode(NodeType type, Node* left, Node* right, int line) {
    Node* newNode = malloc(sizeof(Node));
    if (!newNode) {
        fprintf(stderr, "Erro de alocacao de memoria\n");
        exit(1);
    }
    newNode->type = type;
    newNode->left = left;
    newNode->right = right;
    newNode->line = line;
    newNode->data_type = TYPE_UNDEFINED; // Inicializa o tipo de dado como indefinido
    newNode->symbol_entry = NULL; // Inicializa a entrada da tabela de símbolos como NULL
    return newNode;
}

// Funções específicas para criar nós de tipos diferentes
Node* createIntNode(int value, int line) {
    Node* newNode = createNode(NODE_NUM_INTEIRO, NULL, NULL, line);
    newNode->value.ival = value;
    newNode->data_type = TYPE_INTEGER; // Define o tipo de dado como inteiro
    return newNode;
}

Node* createFloatNode(float value, int line) {
    Node* newNode = createNode(NODE_NUM_REAL, NULL, NULL, line);
    newNode->value.fval = value;
    newNode->data_type = TYPE_REAL; // Define o tipo de dado como real
    return newNode;
}

Node* createIdNode(const char* sval, int line) {
    Node* newNode = createNode(NODE_ID, NULL, NULL, line);
    newNode->value.sval = strdup(sval);
    // O tipo de dado será definido durante a análise semântica
    return newNode;
}

Node* createIfElseNode(Node* condition, Node* if_branch, Node* else_branch, int line) {
    // O nó 'else' é o filho direito do nó 'se-senao'
    Node* elseNode = else_branch ? createNode(NODE_SE_SENAO, if_branch, else_branch, line) : NULL;
    return createNode(NODE_SE, condition, elseNode ? elseNode : if_branch, line);
}

Node* createAtribuicaoNode(Node* id, Node* expr, int line) {
    return createNode(NODE_ATRIBUICAO, id, expr, line);
}

Node* createOpNode(NodeType type, Node* left, Node* right, int line) {
    return createNode(type, left, right, line);
}