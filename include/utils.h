#ifndef UTILS_H
#define UTILS_H

#include "ast.h"

const char* get_token_category(int token);

void printAst(const Node* node, int level);
void printSemanticAst(const Node* node, int level);

#endif //UTILS_H
