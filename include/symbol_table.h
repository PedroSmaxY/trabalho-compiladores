#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdbool.h>
#include <stdio.h>

// Tipos de dados na linguagem P-
typedef enum {
    TYPE_INTEIRO,
    TYPE_REAL,
    TYPE_UNDEFINED
} DataType;

// Estrutura de uma entrada na tabela de símbolos
typedef struct EntradaSimbolo {
    char *name;              // Nome da variável
    DataType type;           // Tipo da variável
    int line;                // Linha onde foi declarada
    int address;             // Endereço de memória (em bytes)
    struct EntradaSimbolo *next; // Próxima entrada (para tratamento de colisões)
} EntradaSimbolo;

// Estrutura da tabela de símbolos
typedef struct {
    EntradaSimbolo **entries;   // Array de ponteiros para entradas
    int size;                // Tamanho da tabela
    int count;               // Número de símbolos inseridos
    int next_address;        // Próximo endereço disponível (em bytes)
} TabelaSimbolos;

// Funções da tabela de símbolos
TabelaSimbolos* criarTabelaSimbolos(int size);
void destruirTabelaSimbolos(TabelaSimbolos *table);
bool inserirSimbolo(TabelaSimbolos *table, const char *name, DataType type, int line);
EntradaSimbolo* buscarSimbolo(TabelaSimbolos *table, const char *name);
bool simboloExiste(TabelaSimbolos *table, const char *name);
void imprimirTabelaSimbolos(TabelaSimbolos *table);
void imprimirTabelaSimbolosArquivo(TabelaSimbolos *table, FILE *file);

// Função auxiliar para hash
unsigned int hash(const char *name, int table_size);

#endif // SYMBOL_TABLE_H

