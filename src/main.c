#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "parser.h"
#include "utils.h"
#include "semantic.h"
#include "symbol_table.h"

typedef struct yy_buffer_state *YY_BUFFER_STATE;
extern int yylex(void);
extern int yyparse(void);
extern int yylineno;
extern char* yytext;
extern YY_BUFFER_STATE yy_scan_string(const char *str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);
extern void yy_switch_to_buffer(YY_BUFFER_STATE new_buffer);

extern Node *ast_root;

#define SHOW_LEXICAL_LOGS 0
#define SHOW_PARSER_LOGS 1
#define SHOW_SEMANTIC_ANALYSIS 1

// Função para gerar nome do arquivo de saída baseado no arquivo de entrada
char* gerarNomeArquivoSaida(const char *arquivo_entrada) {
    char *nome_saida = malloc(strlen(arquivo_entrada) + 10);
    if (!nome_saida) return NULL;
    
    strcpy(nome_saida, arquivo_entrada);
    
    // Remove extensão se existir
    char *ponto = strrchr(nome_saida, '.');
    if (ponto) {
        *ponto = '\0';
    }
    
    // Adiciona extensão .semantico
    strcat(nome_saida, ".semantico");
    
    return nome_saida;
}

int main(const int argc, char **argv) {
    char *buffer = NULL;
    const char *arquivo_entrada = argc > 1 ? argv[1] : "codigo.p-";
    FILE *fp = NULL;
    FILE *arquivo_saida = NULL;
    char *nome_arquivo_saida = NULL;

    // Abre arquivo de entrada
    fp = fopen(arquivo_entrada, "r");
    if (!fp) {
        fprintf(stderr, "Erro: nao foi possivel abrir o arquivo '%s'\n", arquivo_entrada);
        fprintf(stderr, "Uso: ./main <arquivo.p->\n");
        return 1;
    }

    // Gera nome do arquivo de saída
    nome_arquivo_saida = gerarNomeArquivoSaida(arquivo_entrada);
    if (!nome_arquivo_saida) {
        fprintf(stderr, "Erro: nao foi possivel gerar nome do arquivo de saida\n");
        fclose(fp);
        return 1;
    }

    // Abre arquivo de saída
    arquivo_saida = fopen(nome_arquivo_saida, "w");
    if (!arquivo_saida) {
        fprintf(stderr, "Erro: nao foi possivel criar arquivo de saida '%s'\n", nome_arquivo_saida);
        free(nome_arquivo_saida);
        fclose(fp);
        return 1;
    }

    // Pegar tamanho do arquivo
    fseek(fp, 0, SEEK_END);
    const long file_size = ftell(fp);
    rewind(fp);

    buffer = (char *) malloc(file_size + 1);
    if (!buffer) {
        perror("Erro ao alocar buffer");
        fclose(fp);
        fclose(arquivo_saida);
        free(nome_arquivo_saida);
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
            fprintf(arquivo_saida, "Erro de sintaxe\n");
        }

        printf("Analise sintatica concluida.\n");
        fprintf(arquivo_saida, "=== RESULTADO DA ANALISE SEMANTICA ===\n\n");
        fprintf(arquivo_saida, "Arquivo analisado: %s\n\n", arquivo_entrada);

        if (ast_root != NULL) {
            // Mostra AST ANTES da análise semântica
            printf("\n--- Arvore Sintatica (ANTES da analise semantica) ---\n");
            fprintf(arquivo_saida, "--- Arvore Sintatica (ANTES da analise semantica) ---\n");
            printAst(ast_root, 0);
            printAstArquivo(ast_root, 0, arquivo_saida);
            
            if (SHOW_SEMANTIC_ANALYSIS) {
                printf("\n--- Analise Semantica ---\n");
                fprintf(arquivo_saida, "\n--- Analise Semantica ---\n");
                
                // Chama análise semântica e recebe a tabela de símbolos
                TabelaSimbolos *tabela = NULL;
                ResultadoSemantico resultado_semantico = analiseSemantica(ast_root, &tabela);
                
                if (resultado_semantico.error_count == 0) {
                    printf("Analise semantica concluida sem erros.\n");
                    fprintf(arquivo_saida, "Analise semantica concluida sem erros.\n");
                    
                    // Mostra tabela de símbolos
                    printf("\n--- Tabela de Simbolos ---\n");
                    imprimirTabelaSimbolos(tabela);
                    imprimirTabelaSimbolosArquivo(tabela, arquivo_saida);
                    
                    // Mostra AST DEPOIS da análise semântica (com tipos inferidos)
                    printf("\n--- Arvore Sintatica (DEPOIS da analise semantica) ---\n");
                    fprintf(arquivo_saida, "\n--- Arvore Sintatica (DEPOIS da analise semantica) ---\n");
                    printSemanticAst(ast_root, 0);
                    printSemanticAstArquivo(ast_root, 0, arquivo_saida);
                    
                    // Libera tabela de símbolos
                    destruirTabelaSimbolos(tabela);
                } else {
                    printf("Analise semantica concluida com %d erro(s).\n", resultado_semantico.error_count);
                    fprintf(arquivo_saida, "Analise semantica concluida com %d erro(s).\n", resultado_semantico.error_count);
                    if (resultado_semantico.warning_count > 0) {
                        printf("Avisos: %d\n", resultado_semantico.warning_count);
                        fprintf(arquivo_saida, "Avisos: %d\n", resultado_semantico.warning_count);
                    }
                    
                    // Mesmo com erros, mostra AST depois (pode ter tipos parciais)
                    printf("\n--- Arvore Sintatica (DEPOIS da analise semantica) ---\n");
                    fprintf(arquivo_saida, "\n--- Arvore Sintatica (DEPOIS da analise semantica) ---\n");
                    printSemanticAst(ast_root, 0);
                    printSemanticAstArquivo(ast_root, 0, arquivo_saida);
                    
                    if (tabela) {
                        destruirTabelaSimbolos(tabela);
                    }
                }
            } else {
                printf("\n--- Saida Semantica (AST) ---\n");
                printSemanticAst(ast_root, 0);
            }
        } else {
            printf("Nao foi possivel gerar a arvore sintatica devido a erros.\n");
            fprintf(arquivo_saida, "Nao foi possivel gerar a arvore sintatica devido a erros.\n");
        }
        yy_delete_buffer(parse_buffer);
    }

    fclose(arquivo_saida);
    printf("\nResultado da analise semantica salvo em: %s\n", nome_arquivo_saida);
    
    free(buffer);
    free(nome_arquivo_saida);
    return 0;
}