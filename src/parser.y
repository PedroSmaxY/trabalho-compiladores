%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

int yylex(void);
void yyerror(const char *s);
extern int yylineno;

Node *ast_root = NULL;
%}

%union {
    int ival;
    float fval;
    char *sval;
    Node *node;
}

// Associa os tokens aos tipos da união
%token <ival> T_NUMERO_INTEIRO
%token <fval> T_NUMERO_REAL
%token <sval> T_ID

// Tokens simples (sem valor associado)
%token T_INTEIRO T_REAL
%token T_SE T_ENTAO T_SENAO T_ENQUANTO T_REPITA T_ATE
%token T_LER T_MOSTRAR
%token T_AND T_OR T_LE T_GE T_EQ T_NE

// Associa os não-terminais ao tipo 'node' da união
%type <node> declaracao_lista declaracao tipo id_lista
%type <node> comando_lista comando
%type <node> comando_se comando_enquanto comando_repita
%type <node> comando_ler comando_mostrar comando_atribuicao
%type <node> expressao
%type <node> bloco

// Precedência e associatividade dos operadores
%left T_OR
%left T_AND
%nonassoc T_LE T_GE T_EQ T_NE '<' '>'
%left '+' '-'
%left '*' '/'
%precedence T_ENTAO
%precedence T_SENAO

%%

// --- Regras da Gramática ---

programa:
    /* vazio */              { ast_root = NULL; }
    | declaracao_lista         { ast_root = createNode(NODE_PROGRAMA, $1, NULL); }
    | comando_lista            { ast_root = createNode(NODE_PROGRAMA, NULL, $1); }
    | declaracao_lista comando_lista { ast_root = createNode(NODE_PROGRAMA, $1, $2); }
    ;

declaracao_lista:
    declaracao               { $$ = $1; }
    | declaracao_lista declaracao { $$ = createNode(NODE_DECL_LISTA, $1, $2); }
    ;

declaracao:
    tipo id_lista ';'        { $$ = createNode(NODE_DECLARACAO, $1, $2); }
    ;

tipo:
    T_INTEIRO                { $$ = createNode(NODE_TIPO_INTEIRO, NULL, NULL); }
    | T_REAL                 { $$ = createNode(NODE_TIPO_REAL, NULL, NULL); }
    ;

id_lista:
    T_ID                     { $$ = createIdNode($1); }
    | id_lista ',' T_ID      { $$ = createNode(NODE_ID_LISTA, $1, createIdNode($3)); }
    ;

comando_lista:
    comando                  { $$ = $1; }
    | comando_lista comando  { $$ = createNode(NODE_CMD_LISTA, $1, $2); }
    ;

comando:
    comando_se               { $$ = $1; }
    | comando_enquanto       { $$ = $1; }
    | comando_repita         { $$ = $1; }
    | comando_ler            { $$ = $1; }
    | comando_mostrar        { $$ = $1; }
    | comando_atribuicao     { $$ = $1; }
    | bloco                  { $$ = $1; }
    | error ';'              { yyerrok; $$ = NULL; /* Recuperação de erro simples */ }
    ;

bloco:
    '{' '}'                                { $$ = createNode(NODE_BLOCO, NULL, NULL); }
    | '{' declaracao_lista '}'                { $$ = createNode(NODE_BLOCO, $2, NULL); }
    | '{' comando_lista '}'                   { $$ = createNode(NODE_BLOCO, NULL, $2); }
    | '{' declaracao_lista comando_lista '}' { $$ = createNode(NODE_BLOCO, $2, $3); }
    ;

comando_se:
    T_SE expressao T_ENTAO comando %prec T_ENTAO       { $$ = createNode(NODE_SE, $2, $4); }
    | T_SE expressao T_ENTAO comando T_SENAO comando    { $$ = createIfElseNode($2, $4, $6); }
    ;

comando_enquanto:
    T_ENQUANTO '(' expressao ')' comando { $$ = createNode(NODE_ENQUANTO, $3, $5); }
    ;

comando_repita:
    T_REPITA comando T_ATE expressao ';' { $$ = createNode(NODE_REPITA_ATE, $4, $2); }
    ;

comando_ler:
    T_LER '(' id_lista ')' ';'   { $$ = createNode(NODE_LER, $3, NULL); }
    ;

comando_mostrar:
    T_MOSTRAR '(' expressao ')' ';' { $$ = createNode(NODE_MOSTRAR, $3, NULL); }
    ;

comando_atribuicao:
    T_ID '=' expressao ';'   { $$ = createAtribuicaoNode(createIdNode($1), $3); }
    ;

expressao:
    T_ID                                      { $$ = createIdNode($1); }
    | T_NUMERO_INTEIRO                        { $$ = createIntNode($1); }
    | T_NUMERO_REAL                           { $$ = createFloatNode($1); }
    | '(' expressao ')'                       { $$ = $2; }
    | expressao '+' expressao                 { $$ = createOpNode(NODE_OP_SOMA, $1, $3); }
    | expressao '-' expressao                 { $$ = createOpNode(NODE_OP_SUB, $1, $3); }
    | expressao '*' expressao                 { $$ = createOpNode(NODE_OP_MULT, $1, $3); }
    | expressao '/' expressao                 { $$ = createOpNode(NODE_OP_DIV, $1, $3); }
    | expressao '<' expressao                 { $$ = createOpNode(NODE_OP_LT, $1, $3); }
    | expressao T_LE expressao                { $$ = createOpNode(NODE_OP_LE, $1, $3); }
    | expressao '>' expressao                 { $$ = createOpNode(NODE_OP_GT, $1, $3); }
    | expressao T_GE expressao                { $$ = createOpNode(NODE_OP_GE, $1, $3); }
    | expressao T_EQ expressao                { $$ = createOpNode(NODE_OP_EQ, $1, $3); }
    | expressao T_NE expressao                { $$ = createOpNode(NODE_OP_NE, $1, $3); }
    | expressao T_AND expressao               { $$ = createOpNode(NODE_OP_AND, $1, $3); }
    | expressao T_OR expressao                { $$ = createOpNode(NODE_OP_OR, $1, $3); }
    ;

%%

// --- Código C Adicional ---
void yyerror(const char *s) {
    fprintf(stderr, "Erro sintatico na linha %d: %s\n", yylineno, s);
}
