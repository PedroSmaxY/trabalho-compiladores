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

Node* createIdNode(char* sval) {
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

Node* createOpNode(NodeType type, Node* left, Node* right) {
    return createNode(type, left, right);
}


const char* node_type_names[] = {
    "NODE_PROGRAMA", "NODE_BLOCO", "NODE_DECL_LISTA", "NODE_DECLARACAO", "NODE_TIPO_INTEIRO",
    "NODE_TIPO_REAL", "NODE_ID_LISTA", "NODE_CMD_LISTA", "NODE_SE",
    "NODE_SE_SENAO", "NODE_ENQUANTO", "NODE_REPITA_ATE", "NODE_LER",
    "NODE_MOSTRAR", "NODE_ATRIBUICAO", "NODE_ID", "NODE_NUM_INTEIRO",
    "NODE_NUM_REAL", "NODE_OP_SOMA", "NODE_OP_SUB", "NODE_OP_MULT",
    "NODE_OP_DIV", "NODE_OP_AND", "NODE_OP_OR", "NODE_OP_LT", "NODE_OP_LE",
    "NODE_OP_GT", "NODE_OP_GE", "NODE_OP_EQ", "NODE_OP_NE"
};

void printAst(Node* node, int level) {
    if (node == NULL) {
        return;
    }

    for (int i = 0; i < level; i++) {
        printf("  ");
    }

    printf("Node Type: %s", node_type_names[node->type]);

    if (node->type == NODE_ID || node->type == NODE_NUM_INTEIRO || node->type == NODE_NUM_REAL) {
        printf(", Value: ");
        if (node->type == NODE_ID) printf("%s", node->value.sval);
        else if (node->type == NODE_NUM_INTEIRO) printf("%d", node->value.ival);
        else if (node->type == NODE_NUM_REAL) printf("%f", node->value.fval);
    }
    printf("\n");

    printAst(node->left, level + 1);
    printAst(node->right, level + 1);
}
