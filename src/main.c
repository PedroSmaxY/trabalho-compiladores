#include <stdio.h>
#include "ast.h"
#include "parser.h"

extern FILE *yyin;
extern Node *ast_root;

#define SHOW_LEXICAL_LOGS 0
#define SHOW_PARSER_LOGS 1

int main(const int argc, char **argv) {
    yyin = fopen(argc > 1 ? argv[1] : "codigo.txt", "r");
    if (!yyin) {
        printf("./main <codigo.txt path>\n");
        perror(argv[1]);
        return 1;
    }

    if (yyparse() != 0) {
        fprintf(stderr, "Erro de sintaxe\n");
    }

    printf("Analise sintatica concluida.\n");

    if (SHOW_LEXICAL_LOGS) {

    }

    if (SHOW_PARSER_LOGS) {
        if (ast_root != NULL) {
            printf("\n--- Saida Semantica ---\n");
            printSemanticAst(ast_root, 0);
            printf("----------------------------------------\n");
        } else {
            printf("Nao foi possivel gerar a arvore sintatica devido a erros.\n");
        }
    }


    fclose(yyin);
    return 0;
}