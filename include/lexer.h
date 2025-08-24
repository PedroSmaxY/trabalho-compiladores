//
// Created by Pedro Novais on 23/08/25.
//

#ifndef TRABALHO_COMPILADORES_LEXER_H
#define TRABALHO_COMPILADORES_LEXER_H

#include <stdio.h>

typedef enum {
    TOKEN_ID,
    TOKEN_NUM,
    TOKEN_OP,
    TOKEN_KW,
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[64];
} Token;

Token getToken(FILE *fp);


#endif //TRABALHO_COMPILADORES_LEXER_H