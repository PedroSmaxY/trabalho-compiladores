//
// Created by Pedro Novais on 18/09/25.
//

#include "utils.h"
#include <stdio.h>

void print_usage(const char *prog) {
    fprintf(stderr,
            "Uso: %s [arquivo-fonte]\n"
            "Sem argumento, tenta abrir 'codigo.txt'.\n",
            prog);
}

const char *tokenTypeToString(const ETokenType t) {
    switch (t) {
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_INTEGER:    return "INTEGER";
        case TOKEN_FLOAT:      return "FLOAT";
        case TOKEN_OPERATOR:   return "OPERATOR";
        case TOKEN_KEYWORD:    return "KEYWORD";
        case TOKEN_SEPARATOR:  return "SEPARATOR";
        case TOKEN_EOF:        return "EOF";
        default:               return "UNKNOWN";
    }
}
