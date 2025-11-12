#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"
#include "symbol_table.h"

// Estrutura para rastrear erros semânticos
typedef struct {
    int error_count;
    int warning_count;
} ResultadoSemantico;

// Função principal de análise semântica
ResultadoSemantico analiseSemantica(Node *ast_root);

// Funções auxiliares
void analisarPrograma(Node *node, TabelaSimbolos *table, ResultadoSemantico *result);
void analisarDeclaracoes(Node *node, TabelaSimbolos *table, ResultadoSemantico *result);
void analisarComandos(Node *node, TabelaSimbolos *table, ResultadoSemantico *result);
DataType analisarExpressao(Node *node, TabelaSimbolos *table, ResultadoSemantico *result);
DataType obterTipoDadosNo(Node *node, TabelaSimbolos *table);

// Funções de verificação
bool verificarCompatibilidadeTipo(DataType type1, DataType type2, NodeType op);
DataType promoverTipo(DataType type1, DataType type2);
const char* obterNomeTipoDados(DataType type);

#endif // SEMANTIC_H

