#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "utils.h"
#include "parser.h"

extern int yyparse(void);
extern FILE *yyin;
extern Node *ast_root;
extern SymbolTable *global_symbol_table;

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <arquivo_de_entrada>\n", argv[0]);
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror("Erro ao abrir o arquivo de entrada");
        return 1;
    }

    if (yyparse() != 0) {
        fprintf(stderr, "Análise sintática falhou.\n");
        return 1;
    }
    printf("Análise sintática concluída com sucesso.\n");

    FILE *output_file = fopen("analise_semantica.txt", "w");
    if (!output_file) {
        perror("Não foi possível criar o arquivo de saída da análise semântica");
        return 1;
    }

    if (ast_root) {
        fprintf(output_file, "--- Árvore Sintática Abstrata (Antes da Análise Semântica) ---\n");
        printAstToFile(output_file, ast_root, 0);

        printf("Executando análise semântica...\n");
        semantic_check(ast_root);
        printf("Análise semântica concluída.\n");

        fprintf(output_file, "\n\n--- Árvore Sintática Abstrata (Após Análise Semântica) ---\n");
        printSemanticAstToFile(output_file, ast_root, 0);

        fprintf(output_file, "\n\n--- Tabela de Símbolos ---\n");
        printSymbolTableToFile(output_file, global_symbol_table);
    } else {
        fprintf(output_file, "Nenhuma árvore sintática foi gerada.\n");
    }

    printf("Resultados da análise semântica foram escritos em 'analise_semantica.txt'.\n");

    fclose(output_file);
    if (yyin) fclose(yyin);
    free_symbol_table(global_symbol_table);
    // TODO: free_ast(ast_root);

    return 0;
}