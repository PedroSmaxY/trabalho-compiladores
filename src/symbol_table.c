#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 101 // Tamanho da tabela hash (número primo)

// Função hash simples
unsigned int hash(const char *name, int table_size) {
    unsigned int hash_value = 0;
    for (int i = 0; name[i] != '\0'; i++) {
        hash_value = hash_value * 31 + name[i];
    }
    return hash_value % table_size;
}

// Cria uma nova tabela de símbolos
TabelaSimbolos* criarTabelaSimbolos(int size) {
    TabelaSimbolos *table = malloc(sizeof(TabelaSimbolos));
    if (!table) {
        fprintf(stderr, "Erro de alocacao de memoria para tabela de simbolos\n");
        return NULL;
    }
    
    table->size = size;
    table->count = 0;
    table->entries = calloc(size, sizeof(EntradaSimbolo*));
    if (!table->entries) {
        fprintf(stderr, "Erro de alocacao de memoria para entradas da tabela\n");
        free(table);
        return NULL;
    }
    
    return table;
}

// Destrói a tabela de símbolos
void destruirTabelaSimbolos(TabelaSimbolos *table) {
    if (!table) return;
    
    for (int i = 0; i < table->size; i++) {
        EntradaSimbolo *entry = table->entries[i];
        while (entry) {
            EntradaSimbolo *next = entry->next;
            free(entry->name);
            free(entry);
            entry = next;
        }
    }
    
    free(table->entries);
    free(table);
}

// Insere um símbolo na tabela
bool inserirSimbolo(TabelaSimbolos *table, const char *name, DataType type, int line) {
    if (!table || !name) return false;
    
    // Verifica se já existe
    if (simboloExiste(table, name)) {
        return false; // Símbolo já existe (declaração duplicada)
    }
    
    unsigned int index = hash(name, table->size);
    
    // Cria nova entrada
    EntradaSimbolo *entry = malloc(sizeof(EntradaSimbolo));
    if (!entry) {
        fprintf(stderr, "Erro de alocacao de memoria para entrada de simbolo\n");
        return false;
    }
    
    entry->name = strdup(name);
    entry->type = type;
    entry->line = line;
    entry->next = table->entries[index];
    table->entries[index] = entry;
    table->count++;
    
    return true;
}

// Busca um símbolo na tabela
EntradaSimbolo* buscarSimbolo(TabelaSimbolos *table, const char *name) {
    if (!table || !name) return NULL;
    
    unsigned int index = hash(name, table->size);
    EntradaSimbolo *entry = table->entries[index];
    
    while (entry) {
        if (strcmp(entry->name, name) == 0) {
            return entry;
        }
        entry = entry->next;
    }
    
    return NULL; // Símbolo não encontrado
}

// Verifica se um símbolo existe
bool simboloExiste(TabelaSimbolos *table, const char *name) {
    return buscarSimbolo(table, name) != NULL;
}

// Imprime a tabela de símbolos (para debug)
void imprimirTabelaSimbolos(TabelaSimbolos *table) {
    if (!table) {
        printf("Tabela de simbolos vazia\n");
        return;
    }
    
    printf("\n--- Tabela de Simbolos ---\n");
    printf("Total de simbolos: %d\n\n", table->count);
    
    for (int i = 0; i < table->size; i++) {
        EntradaSimbolo *entry = table->entries[i];
        while (entry) {
            const char *type_str = (entry->type == TYPE_INTEIRO) ? "inteiro" : "real";
            printf("  %s: %s (linha %d)\n", entry->name, type_str, entry->line);
            entry = entry->next;
        }
    }
    printf("\n");
}

