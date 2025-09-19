#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "parser.h"
#include "utils.h"

typedef struct yy_buffer_state *YY_BUFFER_STATE;
extern int yylex(void);
extern int yylineno;
extern char* yytext;
extern YY_BUFFER_STATE yy_scan_string(const char *str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);
extern void yy_switch_to_buffer(YY_BUFFER_STATE new_buffer);

extern Node *ast_root;

#define SHOW_LEXICAL_LOGS 1
#define SHOW_PARSER_LOGS 1

int main(const int argc, char **argv) {
    char *buffer = NULL;

    FILE *fp = fopen(argc > 1 ? argv[1] : "codigo.txt", "r");
    if (!fp) {
        printf("./main <caminho do codigo>\n");
        perror(argv[1]);
        return 1;
    }

    // Pegar tamanho do arquivo
    fseek(fp, 0, SEEK_END);
    const long file_size = ftell(fp);
    rewind(fp);

    buffer = (char *) malloc(file_size + 1);
    if (!buffer) {
        perror("Failed to allocate buffer");
        fclose(fp);
        return 1;
    }
    fread(buffer, 1, file_size, fp);
    buffer[file_size] = '\0';
    fclose(fp);

    // Análize léxica
    if (SHOW_LEXICAL_LOGS) {
        YY_BUFFER_STATE lex_buffer = yy_scan_string(buffer);
        yy_switch_to_buffer(lex_buffer);

        printf("\n--- Saida Lexica ---\n");

        int token;
        while ((token = yylex())) {
            printf("Linha %4d  Tipo %-16s Lexema: %s\n", yylineno, get_token_category(token), yytext);
        }
        printf("Linha %4d  Tipo EOF              Lexema: EOF\n", yylineno);
        yy_delete_buffer(lex_buffer);
    }

    if (SHOW_PARSER_LOGS) {
        // Resetando o lexer para o parser
        YY_BUFFER_STATE parse_buffer = yy_scan_string(buffer);
        yy_switch_to_buffer(parse_buffer);

        if (yyparse() != 0) {
            fprintf(stderr, "Erro de sintaxe\n");
        }

        printf("Analise sintatica concluida.\n");

        if (ast_root != NULL) {
            printf("\n--- Saida Semantica ---\n");
            printSemanticAst(ast_root, 0);
        } else {
            printf("Nao foi possivel gerar a arvore sintatica devido a erros.\n");
        }
        yy_delete_buffer(parse_buffer);
    }

    free(buffer);
    return 0;
}