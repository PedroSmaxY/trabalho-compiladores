//
// Created by Pedro Novais on 18/09/25.
//

#ifndef TRABALHO_COMPILADORES
#define TRABALHO_COMPILADORES

#define MAX_CHILDREN 3

#include "utils.h"
#include <stdio.h>

typedef enum { EnumStatementKind, EnumExpressionKind } NodeKind;
typedef enum { IfKind, RepeatKind, AssignKind, ReadKind, WriteKind } StatementKind;
typedef enum { OpKind, ConstKind, IdKind } ExpressionKind;

typedef enum { Void, Integer, Boolean } ExpresionType;

typedef struct treeNode {
    struct treeNode* child[MAX_CHILDREN];
    struct treeNode* sibling;
    int lineNumber;
    NodeKind nodeKind;
    union { StatementKind statement; ExpressionKind expression; } kind;
    union { ETokenType operation; int val; char *name; } attribute;
    ExpresionType type; // for type checking of expressions
} TreeNode;

void parse(FILE *fp);

#endif //TRABALHO_COMPILADORES