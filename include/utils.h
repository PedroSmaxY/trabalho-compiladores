#ifndef UTILS_H
#define UTILS_H

#include "ast.h"

const char* get_token_category(int token);

void printAst(const Node* node, int level);
void printSemanticAst(const Node* node, int level);
void printAstArquivo(const Node* node, int level, FILE *file);
void printSemanticAstArquivo(const Node* node, int level, FILE *file);

#endif //UTILS_H
