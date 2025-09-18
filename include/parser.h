//
// Created by Pedro Novais on 18/09/25.
//

#ifndef TRABALHO_COMPILADORES
#define TRABALHO_COMPILADORES

#define MAX_CHILDREN 3

#include "utils.h"
#include <stdio.h>

typedef enum { ENodeStatement, ENodeExpression } ENodeKind;
typedef enum { EStmtIf, EStmtRepeat, EStmtAssign, EStmtRead, EStmtWrite } EStatementKind;
typedef enum { EExprOp, EExprConst, EExprId } EExpressionKind;
typedef enum { ETypeVoid, ETypeInteger, ETypeBoolean } EExpressionType;

typedef struct syntaxTreeNode {
    struct syntaxTreeNode* children[MAX_CHILDREN];
    struct syntaxTreeNode* nextSibling;
    int lineNumber;
    ENodeKind nodeKind;
    union { EStatementKind stmt; EExpressionKind expr; } kind;
    union { ETokenType op; int value; char *identifier; } attribute;
    EExpressionType type;
} SyntaxTreeNode;

void parse(FILE *fp);

#endif //TRABALHO_COMPILADORES