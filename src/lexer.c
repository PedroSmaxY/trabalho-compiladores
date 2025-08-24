//
// Created by Pedro Novais on 23/08/25.
//

#include "lexer.h"
#include <ctype.h>
#include <string.h>

static int current_line = 1;

static const char *keywords[] = {
    "inteiro", "real", "se", "entao", "senao", "enquanto",
    "repita", "ate", "ler", "mostrar"
};

const char *tokenTypeToString(TokenType t) {
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

static int isKeyword(const char *s) {
    for (size_t i = 0; i < sizeof(keywords)/sizeof(keywords[0]); i++)
        if (strcmp(s, keywords[i]) == 0) return 1;
    return 0;
}

// Avança espaços e comentários /* ... */ preservando contagem de linhas
static int skipWhitespaceAndComments(FILE *fp) {
    while (1) {
        int c = fgetc(fp);
        // consumir espaços/brancos
        while (c != EOF && isspace(c)) {
            if (c == '\n') current_line++;
            c = fgetc(fp);
        }
        if (c == '/') {
            int next = fgetc(fp);
            if (next == '*') {
                // comentário de bloco
                int prev = 0;
                int d;
                while ((d = fgetc(fp)) != EOF) {
                    if (d == '\n') current_line++;
                    if (prev == '*' && d == '/') break;
                    prev = d;
                }
                if (d == EOF) return EOF;
                continue; // volta para consumir mais brancos/comentários
            } else {
                if (next != EOF) ungetc(next, fp);
                return c;
            }
        }
        return c;
    }
}

Token getToken(FILE *fp) {
    Token token;
    token.lexeme[0] = '\0';
    token.line = current_line;

    int c = skipWhitespaceAndComments(fp);
    if (c == EOF) {
        token.type = TOKEN_EOF;
        strcpy(token.lexeme, "EOF");
        token.line = current_line;
        return token;
    }

    // identificador / keyword
    if (isalpha(c) || c == '_') {
        int i = 0;
        token.lexeme[i++] = (char)c;
        while ((c = fgetc(fp)) != EOF && (isalnum(c) || c == '_')) {
            token.lexeme[i++] = (char)c;
        }
        if (c != EOF) ungetc(c, fp);
        token.lexeme[i] = '\0';
        token.type = isKeyword(token.lexeme) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
        token.line = current_line;
        return token;
    }

    // número
    if (isdigit(c)) {
        int i = 0;
        int is_real = 0;
        token.lexeme[i++] = (char)c;
        while ((c = fgetc(fp)) != EOF && isdigit(c))
            token.lexeme[i++] = (char)c;
        if (c == '.') {
            int peek = fgetc(fp);
            if (peek != EOF && isdigit(peek)) {
                is_real = 1;
                token.lexeme[i++] = '.';
                token.lexeme[i++] = (char)peek;
                while ((c = fgetc(fp)) != EOF && isdigit(c))
                    token.lexeme[i++] = (char)c;
            } else {
                if (peek != EOF) ungetc(peek, fp);
                ungetc('.', fp);
            }
        }
        if (c != EOF) ungetc(c, fp);
        token.lexeme[i] = '\0';
        token.type = is_real ? TOKEN_FLOAT : TOKEN_INTEGER;
        token.line = current_line;
        return token;
    }

    // operadores compostos
    if (strchr("=<>!&|", c)) {
        int i = 0;
        token.lexeme[i++] = (char)c;
        int next = fgetc(fp);
        int composed = 0;
        if ((c == '=' && next == '=') ||
            (c == '!' && next == '=') ||
            (c == '<' && next == '=') ||
            (c == '>' && next == '=') ||
            (c == '&' && next == '&') ||
            (c == '|' && next == '|')) {
            token.lexeme[i++] = (char)next;
            composed = 1;
        } else {
            if (next != EOF) ungetc(next, fp);
        }
        token.lexeme[i] = '\0';
        token.type = (!composed && (token.lexeme[0] == '&' || token.lexeme[0] == '|'))
                     ? TOKEN_UNKNOWN : TOKEN_OPERATOR;
        token.line = current_line;
        return token;
    }

    // operadores aritméticos simples
    if (strchr("+-*/", c)) {
        token.type = TOKEN_OPERATOR;
        token.lexeme[0] = (char)c;
        token.lexeme[1] = '\0';
        token.line = current_line;
        return token;
    }

    // separadores
    if (strchr(";,(){}", c)) {
        token.type = TOKEN_SEPARATOR;
        token.lexeme[0] = (char)c;
        token.lexeme[1] = '\0';
        token.line = current_line;
        if (c == '\n') current_line++; // (não ocorre aqui, apenas por completude)
        return token;
    }

    // desconhecido
    token.type = TOKEN_UNKNOWN;
    token.lexeme[0] = (char)c;
    token.lexeme[1] = '\0';
    token.line = current_line;
    return token;
}