//
// Created by Pedro Novais on 23/08/25.
//

#include "lexer.h"
#include <ctype.h>
#include <string.h>

const char *keywords[] = {"if", "else", "while", "return", "int", "float"};

int isKeyword(const char *str) {
    for (int i = 0; i < sizeof(keywords)/sizeof(keywords[0]); i++) {
        if (strcmp(str, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

Token getToken(FILE *fp) {
    int c;
    Token token;
    token.lexeme[0] = '\0';

    // ignora espaços e quebras de linha
    while ((c = fgetc(fp)) != EOF && isspace(c));

    if (c == EOF) {
        token.type = TOKEN_EOF;
        strcpy(token.lexeme, "EOF");
        return token;
    }

    // identificador ou palavra-chave
    if (isalpha(c) || c == '_') {
        int i = 0;
        token.lexeme[i++] = c;
        while ((c = fgetc(fp)) != EOF && (isalnum(c) || c == '_')) {
            token.lexeme[i++] = c;
        }
        token.lexeme[i] = '\0';
        ungetc(c, fp);

        if (isKeyword(token.lexeme))
            token.type = TOKEN_KW;
        else
            token.type = TOKEN_ID;

        return token;
    }

    // número
    if (isdigit(c)) {
        int i = 0;
        token.lexeme[i++] = c;
        while ((c = fgetc(fp)) != EOF && isdigit(c)) {
            token.lexeme[i++] = c;
        }
        token.lexeme[i] = '\0';
        ungetc(c, fp);
        token.type = TOKEN_NUM;
        return token;
    }

    // operadores simples
    if (strchr("+-*/=<>", c)) {
        token.type = TOKEN_OP;
        token.lexeme[0] = c;
        token.lexeme[1] = '\0';
        return token;
    }

    // desconhecido
    token.type = TOKEN_UNKNOWN;
    token.lexeme[0] = c;
    token.lexeme[1] = '\0';
    return token;
}
