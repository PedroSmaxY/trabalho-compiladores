#include "semantic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylineno; // Linha atual do analisador léxico

// Função auxiliar para obter nome do tipo de dados
const char* obterNomeTipoDados(DataType type) {
    switch (type) {
        case TYPE_INTEIRO: return "inteiro";
        case TYPE_REAL: return "real";
        case TYPE_UNDEFINED: return "indefinido";
        default: return "desconhecido";
    }
}

// Promove tipos (inteiro para real quando necessário)
DataType promoverTipo(DataType type1, DataType type2) {
    if (type1 == TYPE_REAL || type2 == TYPE_REAL) {
        return TYPE_REAL;
    }
    return TYPE_INTEIRO;
}

// Verifica compatibilidade de tipos para operações
bool verificarCompatibilidadeTipo(DataType type1, DataType type2, NodeType op) {
    // Operadores lógicos (&&, ||) requerem tipos compatíveis
    if (op == NODE_OP_AND || op == NODE_OP_OR) {
        // Em P-, operadores lógicos podem trabalhar com inteiros
        return (type1 == TYPE_INTEIRO || type1 == TYPE_REAL) &&
               (type2 == TYPE_INTEIRO || type2 == TYPE_REAL);
    }
    
    // Operadores de comparação retornam inteiro (booleano)
    if (op == NODE_OP_LT || op == NODE_OP_LE || op == NODE_OP_GT || 
        op == NODE_OP_GE || op == NODE_OP_EQ || op == NODE_OP_NE) {
        return (type1 == TYPE_INTEIRO || type1 == TYPE_REAL) &&
               (type2 == TYPE_INTEIRO || type2 == TYPE_REAL);
    }
    
    // Operadores aritméticos
    return (type1 == TYPE_INTEIRO || type1 == TYPE_REAL) &&
           (type2 == TYPE_INTEIRO || type2 == TYPE_REAL);
}

// Obtém o tipo de dados de um nó
DataType obterTipoDadosNo(Node *node, TabelaSimbolos *table) {
    if (!node) return TYPE_UNDEFINED;
    
    switch (node->type) {
        case NODE_NUM_INTEIRO:
            return TYPE_INTEIRO;
        
        case NODE_NUM_REAL:
            return TYPE_REAL;
        
        case NODE_ID: {
            EntradaSimbolo *entry = buscarSimbolo(table, node->value.sval);
            if (entry) {
                return entry->type;
            }
            return TYPE_UNDEFINED;
        }
        
        case NODE_TIPO_INTEIRO:
            return TYPE_INTEIRO;
        
        case NODE_TIPO_REAL:
            return TYPE_REAL;
        
        case NODE_OP_AND:
        case NODE_OP_OR:
        case NODE_OP_LT:
        case NODE_OP_LE:
        case NODE_OP_GT:
        case NODE_OP_GE:
        case NODE_OP_EQ:
        case NODE_OP_NE:
            // Operadores lógicos e de comparação retornam inteiro (booleano)
            return TYPE_INTEIRO;
        
        case NODE_OP_SOMA:
        case NODE_OP_SUB:
        case NODE_OP_MULT:
        case NODE_OP_DIV: {
            DataType left_type = obterTipoDadosNo(node->left, table);
            DataType right_type = obterTipoDadosNo(node->right, table);
            return promoverTipo(left_type, right_type);
        }
        
        default:
            return node->data_type;
    }
}

// Analisa uma expressão e retorna seu tipo
DataType analisarExpressao(Node *node, TabelaSimbolos *table, ResultadoSemantico *result) {
    if (!node) return TYPE_UNDEFINED;
    
    switch (node->type) {
        case NODE_ID: {
            const char *var_name = node->value.sval;
            EntradaSimbolo *entry = buscarSimbolo(table, var_name);
            
            if (!entry) {
                fprintf(stderr, "Erro semantico na linha %d: variavel '%s' nao foi declarada\n", 
                        node->line > 0 ? node->line : yylineno, var_name);
                result->error_count++;
                return TYPE_UNDEFINED;
            }
            
            node->data_type = entry->type;
            return entry->type;
        }
        
        case NODE_NUM_INTEIRO:
            node->data_type = TYPE_INTEIRO;
            return TYPE_INTEIRO;
        
        case NODE_NUM_REAL:
            node->data_type = TYPE_REAL;
            return TYPE_REAL;
        
        case NODE_OP_SOMA:
        case NODE_OP_SUB:
        case NODE_OP_MULT:
        case NODE_OP_DIV:
        case NODE_OP_AND:
        case NODE_OP_OR:
        case NODE_OP_LT:
        case NODE_OP_LE:
        case NODE_OP_GT:
        case NODE_OP_GE:
        case NODE_OP_EQ:
        case NODE_OP_NE: {
            DataType left_type = analisarExpressao(node->left, table, result);
            DataType right_type = analisarExpressao(node->right, table, result);
            
            if (left_type == TYPE_UNDEFINED || right_type == TYPE_UNDEFINED) {
                node->data_type = TYPE_UNDEFINED;
                return TYPE_UNDEFINED;
            }
            
            if (!verificarCompatibilidadeTipo(left_type, right_type, node->type)) {
                fprintf(stderr, "Erro semantico na linha %d: tipos incompatíveis na operacao\n", 
                        node->line > 0 ? node->line : yylineno);
                result->error_count++;
                node->data_type = TYPE_UNDEFINED;
                return TYPE_UNDEFINED;
            }
            
            // Operadores lógicos e de comparação retornam inteiro
            if (node->type == NODE_OP_AND || node->type == NODE_OP_OR ||
                node->type == NODE_OP_LT || node->type == NODE_OP_LE ||
                node->type == NODE_OP_GT || node->type == NODE_OP_GE ||
                node->type == NODE_OP_EQ || node->type == NODE_OP_NE) {
                node->data_type = TYPE_INTEIRO;
                return TYPE_INTEIRO;
            }
            
            // Operadores aritméticos
            node->data_type = promoverTipo(left_type, right_type);
            return node->data_type;
        }
        
        default:
            // Para outros nós, analisa recursivamente
            if (node->left) {
                analisarExpressao(node->left, table, result);
            }
            if (node->right) {
                analisarExpressao(node->right, table, result);
            }
            return node->data_type;
    }
}

// Analisa lista de identificadores (para ler) - verifica se estão declarados
void analisarListaIds(Node *node, TabelaSimbolos *table, ResultadoSemantico *result) {
    if (!node) return;
    
    if (node->type == NODE_ID) {
        const char *var_name = node->value.sval;
        EntradaSimbolo *entry = buscarSimbolo(table, var_name);
        
        if (!entry) {
            fprintf(stderr, "Erro semantico na linha %d: variavel '%s' nao foi declarada\n", 
                    node->line > 0 ? node->line : yylineno, var_name);
            result->error_count++;
            return;
        }
    } else if (node->type == NODE_ID_LISTA) {
        // Processa recursivamente a lista
        analisarListaIds(node->left, table, result);
        analisarListaIds(node->right, table, result);
    }
}

// Processa lista de IDs recursivamente
void processarListaIds(Node *id_list, DataType var_type, TabelaSimbolos *table, ResultadoSemantico *result, int line) {
    if (!id_list) return;
    
    if (id_list->type == NODE_ID) {
        // ID único
        const char *var_name = id_list->value.sval;
        if (simboloExiste(table, var_name)) {
            fprintf(stderr, "Erro semantico na linha %d: variavel '%s' ja foi declarada\n", 
                    line > 0 ? line : yylineno, var_name);
            result->error_count++;
        } else {
            if (!inserirSimbolo(table, var_name, var_type, line)) {
                fprintf(stderr, "Erro semantico na linha %d: nao foi possivel inserir variavel '%s'\n", 
                        line > 0 ? line : yylineno, var_name);
                result->error_count++;
            }
        }
    } else if (id_list->type == NODE_ID_LISTA) {
        // Lista de IDs: left é a lista anterior, right é o próximo ID
        // Processa recursivamente a lista à esquerda
        processarListaIds(id_list->left, var_type, table, result, line);
        // Processa o ID à direita
        processarListaIds(id_list->right, var_type, table, result, line);
    }
}

// Analisa declarações
void analisarDeclaracoes(Node *node, TabelaSimbolos *table, ResultadoSemantico *result) {
    if (!node) return;
    
    if (node->type == NODE_DECL_LISTA) {
        // Processa lista de declarações
        analisarDeclaracoes(node->left, table, result);
        analisarDeclaracoes(node->right, table, result);
    } else if (node->type == NODE_DECLARACAO) {
        // node->left é o tipo, node->right é a lista de IDs
        Node *tipo_node = node->left;
        Node *id_list = node->right;
        
        if (!tipo_node || !id_list) {
            fprintf(stderr, "Erro semantico na linha %d: declaracao incompleta\n", 
                    node->line > 0 ? node->line : yylineno);
            result->error_count++;
            return;
        }
        
        DataType var_type = TYPE_UNDEFINED;
        if (tipo_node->type == NODE_TIPO_INTEIRO) {
            var_type = TYPE_INTEIRO;
        } else if (tipo_node->type == NODE_TIPO_REAL) {
            var_type = TYPE_REAL;
        } else {
            fprintf(stderr, "Erro semantico na linha %d: tipo invalido na declaracao\n", 
                    node->line > 0 ? node->line : yylineno);
            result->error_count++;
            return;
        }
        
        // Processa lista de identificadores
        processarListaIds(id_list, var_type, table, result, node->line);
    }
}

// Analisa comandos
void analisarComandos(Node *node, TabelaSimbolos *table, ResultadoSemantico *result) {
    if (!node) return;
    
    if (node->type == NODE_CMD_LISTA) {
        analisarComandos(node->left, table, result);
        analisarComandos(node->right, table, result);
    } else if (node->type == NODE_ATRIBUICAO) {
        // Verifica atribuição: node->left é o ID, node->right é a expressão
        Node *id_node = node->left;
        Node *expr_node = node->right;
        
        if (id_node->type == NODE_ID) {
            const char *var_name = id_node->value.sval;
            EntradaSimbolo *entry = buscarSimbolo(table, var_name);
            
            if (!entry) {
                fprintf(stderr, "Erro semantico na linha %d: variavel '%s' nao foi declarada\n", 
                        node->line > 0 ? node->line : yylineno, var_name);
                result->error_count++;
            } else {
                DataType expr_type = analisarExpressao(expr_node, table, result);
                
                if (expr_type != TYPE_UNDEFINED) {
                    // Em P-, podemos atribuir inteiro a real (promoção implícita)
                    if (entry->type == TYPE_REAL && expr_type == TYPE_INTEIRO) {
                        // OK - promoção implícita
                    } else if (entry->type != expr_type) {
                        fprintf(stderr, "Erro semantico na linha %d: tipo incompativel na atribuicao de '%s' (esperado: %s, encontrado: %s)\n",
                                node->line > 0 ? node->line : yylineno, var_name, obterNomeTipoDados(entry->type), obterNomeTipoDados(expr_type));
                        result->error_count++;
                    }
                }
            }
        }
    } else if (node->type == NODE_LER) {
        // Analisa lista de IDs para ler
        analisarListaIds(node->left, table, result);
    } else if (node->type == NODE_MOSTRAR) {
        // Analisa expressão para mostrar
        analisarExpressao(node->left, table, result);
    } else if (node->type == NODE_SE) {
        // Analisa condição do if
        DataType cond_type = analisarExpressao(node->left, table, result);
        if (cond_type != TYPE_UNDEFINED && cond_type != TYPE_INTEIRO && cond_type != TYPE_REAL) {
            fprintf(stderr, "Erro semantico na linha %d: condicao do 'se' deve ser do tipo inteiro ou real\n", 
                    node->line > 0 ? node->line : yylineno);
            result->error_count++;
        }
        
        // Analisa comando do if
        if (node->right) {
            if (node->right->type == NODE_SE_SENAO) {
                analisarComandos(node->right->left, table, result);
                analisarComandos(node->right->right, table, result);
            } else {
                analisarComandos(node->right, table, result);
            }
        }
    } else if (node->type == NODE_ENQUANTO) {
        // Analisa condição do enquanto
        DataType cond_type = analisarExpressao(node->left, table, result);
        if (cond_type != TYPE_UNDEFINED && cond_type != TYPE_INTEIRO && cond_type != TYPE_REAL) {
            fprintf(stderr, "Erro semantico na linha %d: condicao do 'enquanto' deve ser do tipo inteiro ou real\n", 
                    node->line > 0 ? node->line : yylineno);
            result->error_count++;
        }
        
        // Analisa comando do enquanto
        analisarComandos(node->right, table, result);
    } else if (node->type == NODE_REPITA_ATE) {
        // Analisa comando do repita
        analisarComandos(node->right, table, result);
        
        // Analisa condição do até
        DataType cond_type = analisarExpressao(node->left, table, result);
        if (cond_type != TYPE_UNDEFINED && cond_type != TYPE_INTEIRO && cond_type != TYPE_REAL) {
            fprintf(stderr, "Erro semantico na linha %d: condicao do 'ate' deve ser do tipo inteiro ou real\n", 
                    node->line > 0 ? node->line : yylineno);
            result->error_count++;
        }
    } else if (node->type == NODE_BLOCO) {
        // Analisa declarações do bloco (mesmo escopo global por enquanto)
        if (node->left) {
            analisarDeclaracoes(node->left, table, result);
        }
        
        // Analisa comandos do bloco
        if (node->right) {
            analisarComandos(node->right, table, result);
        }
    } else {
        // Para outros tipos de comandos, analisa recursivamente
        analisarComandos(node->left, table, result);
        analisarComandos(node->right, table, result);
    }
}

// Analisa o programa completo
void analisarPrograma(Node *node, TabelaSimbolos *table, ResultadoSemantico *result) {
    if (!node) return;
    
    if (node->type == NODE_PROGRAMA) {
        // Primeiro, analisa todas as declarações
        if (node->left) {
            analisarDeclaracoes(node->left, table, result);
        }
        
        // Depois, analisa todos os comandos
        if (node->right) {
            analisarComandos(node->right, table, result);
        }
    }
}

// Função principal de análise semântica
// Retorna a tabela de símbolos através do parâmetro table_ptr (pode ser NULL se não precisar)
ResultadoSemantico analiseSemantica(Node *ast_root, TabelaSimbolos **table_ptr) {
    ResultadoSemantico result = {0, 0};
    
    if (!ast_root) {
        fprintf(stderr, "Erro: arvore sintatica vazia\n");
        result.error_count++;
        if (table_ptr) *table_ptr = NULL;
        return result;
    }
    
    // Cria tabela de símbolos
    TabelaSimbolos *table = criarTabelaSimbolos(101);
    
    if (!table) {
        fprintf(stderr, "Erro: nao foi possivel criar tabela de simbolos\n");
        result.error_count++;
        if (table_ptr) *table_ptr = NULL;
        return result;
    }
    
    // Analisa o programa
    analisarPrograma(ast_root, table, &result);
    
    // Retorna tabela através do ponteiro (se solicitado)
    if (table_ptr) {
        *table_ptr = table;
    } else {
        // Se não foi solicitado retornar a tabela, imprime e libera
        if (result.error_count == 0) {
            printf("\n--- Tabela de Simbolos ---\n");
            imprimirTabelaSimbolos(table);
        }
        destruirTabelaSimbolos(table);
    }
    
    return result;
}

