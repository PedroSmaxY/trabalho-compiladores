#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include "ast.h"

const char* get_token_category(int token);

void printAst(const Node* node, int level);
void printSemanticAst(const Node* node, int level);

// Funções para imprimir em arquivo
void printAstToFile(FILE* fp, const Node* node, int level);
void printSemanticAstToFile(FILE* fp, const Node* node, int level);
void printSymbolTableToFile(FILE* fp, SymbolTable* table);

#endif //UTILS_H
