//
// Created by Pedro Novais on 23/08/25.
//

#ifndef TRABALHO_COMPILADORES_LEXER_H
#define TRABALHO_COMPILADORES_LEXER_H

#include <stdio.h>

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_INTEGER,
    TOKEN_FLOAT,
    TOKEN_OPERATOR,
    TOKEN_KEYWORD,
    TOKEN_SEPARATOR,
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[64];
    int line;
} Token;

Token getToken(FILE *fp);
const char *tokenTypeToString(TokenType t);

#endif // TRABALHO_COMPILADORES_LEXER_H