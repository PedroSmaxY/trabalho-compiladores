//
// Created by Pedro Novais on 18/09/25.
//

#include "parser.h"
#include "lexer.h"
#include "utils.h"

static SyntaxTreeNode *program(void);        // <declarações> <comandos>
static SyntaxTreeNode *decl_list(void);      // lista de declarações
static SyntaxTreeNode *decl(void);           // uma declaração

static SyntaxTreeNode *stmt_sequence(void);  // sequência de comandos
static SyntaxTreeNode *statement(void);      // comando genérico
static SyntaxTreeNode *if_stmt(void);        // se-entao-senao
static SyntaxTreeNode *while_stmt(void);     // enquanto (...)
static SyntaxTreeNode *repeat_stmt(void);    // repita ... ate ...
static SyntaxTreeNode *assign_stmt(void);    // identificador = expressão
static SyntaxTreeNode *read_stmt(void);      // ler(identificador)
static SyntaxTreeNode *write_stmt(void);     // mostrar(expressão)
static SyntaxTreeNode *block_stmt(void);     // { ... }

static SyntaxTreeNode *expr(void);           // expressão (lógica, relacional, aritmética)
static SyntaxTreeNode *simple_exp(void);     // expressão simples
static SyntaxTreeNode *term(void);           // termo
static SyntaxTreeNode *factor(void);         // fator

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


