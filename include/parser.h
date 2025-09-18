//
// Created by Pedro Novais on 18/09/25.
//

#ifndef TRABALHO_COMPILADORES
#define TRABALHO_COMPILADORES

#define MAX_CHILDREN 3

#include "utils.h"
#include <stdio.h>

typedef enum {
    ENodeDeclaration,   // Declarações de variáveis (inteiro, real)
    ENodeStatement,     // Comandos (if, while, etc.)
    ENodeExpression     // Expressões (aritméticas, relacionais, lógicas)
} ENodeKind;

typedef enum {
    EStmtIf,        // se-entao-senao
    EStmtWhile,     // enquanto
    EStmtRepeat,    // repita-ate
    EStmtAssign,    // atribuição
    EStmtRead,      // ler
    EStmtWrite,     // mostrar
    EStmtBlock      // bloco { ... }
} EStatementKind;

typedef enum {
    EExprOp,        // Operador (+, -, *, /, <, ==, etc.)
    EExprConst,     // Constante numérica
    EExprId         // Identificador (variável)
} EExpressionKind;

typedef enum {
    ETypeVoid,      // usado em comandos sem valor de retorno
    ETypeInteger,   // inteiro (variável e expressão)
    ETypeReal,      // real (variável e expressão)
    ETypeBoolean    // resultado de expressões relacionais/lógicas
} EExpressionType;

typedef struct syntaxTreeNode {
    struct syntaxTreeNode* children[MAX_CHILDREN];  // filhos diretos
    struct syntaxTreeNode* nextSibling;             // próximo irmão (lista encadeada)
    int lineNumber;                                 // número da linha do código fonte
    ENodeKind nodeKind;                             // tipo de nó (declaração, comando, expressão)

    union {
        EStatementKind stmt;
        EExpressionKind expr;
    } kind;

    union {
        ETokenType op;      // operador (quando for expressão com operador)
        int value;          // valor constante
        char *identifier;   // nome de variável
    } attribute;

    EExpressionType type;   // tipo do nó (void, inteiro, real)
} SyntaxTreeNode;

SyntaxTreeNode* parse(FILE *fp);

#endif //TRABALHO_COMPILADORES