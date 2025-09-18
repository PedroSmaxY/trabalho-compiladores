//
// Created by Pedro Novais on 18/09/25.
//

#ifndef TRABALHO_COMPILADORES_H
#define TRABALHO_COMPILADORES_H

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_INTEGER,
    TOKEN_FLOAT,
    TOKEN_OPERATOR,
    TOKEN_KEYWORD,
    TOKEN_SEPARATOR,
    TOKEN_EOF,
    TOKEN_UNKNOWN
} ETokenType;

typedef struct {
    ETokenType type;
    char lexeme[64];
    int line;
} Token;

const char *tokenTypeToString(ETokenType t);
void print_usage(const char *prog);

#endif //TRABALHO_COMPILADORES_H