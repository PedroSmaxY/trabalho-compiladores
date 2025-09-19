//
// Created by Pedro Novais on 18/09/25.
//

#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Node* createNode(const NodeType type, Node* left, Node* right) {
    Node* newNode = malloc(sizeof(Node));
    if (!newNode) {
        fprintf(stderr, "Erro de alocacao de memoria\n");
        exit(1);
    }
    newNode->type = type;
    newNode->left = left;
    newNode->right = right;
    return newNode;
}

Node* createIntNode(const int value) {
    Node* newNode = createNode(NODE_NUM_INTEIRO, NULL, NULL);
    newNode->value.ival = value;
    return newNode;
}

Node* createFloatNode(const float value) {
    Node* newNode = createNode(NODE_NUM_REAL, NULL, NULL);
    newNode->value.fval = value;
    return newNode;
}

Node* createIdNode(const char* sval) {
    Node* newNode = createNode(NODE_ID, NULL, NULL);
    newNode->value.sval = strdup(sval);
    return newNode;
}

Node* createIfElseNode(Node* condition, Node* if_branch, Node* else_branch) {
    Node* elseNode = createNode(NODE_SE_SENAO, if_branch, else_branch);
    return createNode(NODE_SE, condition, elseNode);
}

Node* createAtribuicaoNode(Node* id, Node* expr) {
    return createNode(NODE_ATRIBUICAO, id, expr);
}

Node* createOpNode(const NodeType type, Node* left, Node* right) {
    return createNode(type, left, right);
}