//
// Created by Pedro Novais on 18/09/25.
//

#include "parser.h"
#include "lexer.h"
#include "utils.h"

SyntaxTreeNode *parse(FILE *fp) {
    Token token = getToken(fp);
    while (token.type != TOKEN_EOF) {
        printf("Token: %s, Lexeme: %s, Line: %d\n", tokenTypeToString(token.type), token.lexeme, token.line);
        token = getToken(fp);
    }
    // Placeholder for parser implementation
    printf("Parsing the input file...\n");

    return NULL;
}
