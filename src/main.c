#include <stdio.h>
#include "ast.h"

extern int yyparse(void);
extern FILE *yyin;

extern Node *ast_root;

extern int yydebug;

int main(const int argc, char **argv) {
    yydebug = 1;
    yyin = fopen(argc > 1 ? argv[1] : "codigo.txt", "r");
    if (!yyin) {
        perror(argv[1]);
        return 1;
    }

    yyparse();

    printf("Analise sintatica concluida.\n");

    if (ast_root != NULL) {
        printf("\n--- Arvore Sintatica Abstrata Gerada ---\n");
        printAst(ast_root, 0);
        printf("----------------------------------------\n");
    } else {
        printf("Nao foi possivel gerar a arvore sintatica devido a erros.\n");
    }

    fclose(yyin);
    return 0;
}