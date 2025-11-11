#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Protótipos de funções internas
static void traverse_node(Node *node, SymbolTable *table);
static void process_declarations(Node *decl_node, SymbolTable *table);
static void process_id_list(Node *id_list_node, SymbolTable *table, DataType type);
static void process_assignment(Node *node, SymbolTable *table);
static void process_expression(Node *node, SymbolTable *table);
static void process_identifier(Node *node, SymbolTable *table);
static void adjust_node_type(Node *node, SymbolTable *table);
static Node* create_type_conversion_node(Node* child, DataType target_type);

SymbolTable *global_symbol_table = NULL;

SymbolTable* create_symbol_table(char* scope_name, SymbolTable* parent) {
    SymbolTable* table = malloc(sizeof(SymbolTable));
    table->scope_name = strdup(scope_name);
    table->parent = parent;
    table->symbols = NULL;
    table->current_address = (parent) ? parent->current_address : 0;
    return table;
}

void free_symbol_table(SymbolTable* table) {
    if (!table) return;
    free(table->scope_name);
    Symbol* current = table->symbols;
    while (current) {
        Symbol* next = current->next;
        free(current->name);
        free(current);
        current = next;
    }
    free(table);
}

void insert_symbol(SymbolTable* table, const char* name, DataType type, int line) {
    for (Symbol* s = table->symbols; s != NULL; s = s->next) {
        if (strcmp(s->name, name) == 0) {
            fprintf(stderr, "Erro Semântico (Linha %d): Variável '%s' já declarada.\n", line, name);
            exit(EXIT_FAILURE);
        }
    }
    Symbol* new_symbol = malloc(sizeof(Symbol));
    new_symbol->name = strdup(name);
    new_symbol->type = type;
    new_symbol->line = line;
    new_symbol->memory_address = table->current_address;
    table->current_address += (type == TYPE_INTEGER) ? 4 : 8;
    new_symbol->next = table->symbols;
    table->symbols = new_symbol;
}

Symbol* lookup_symbol(SymbolTable* table, const char* name) {
    for (SymbolTable* t = table; t != NULL; t = t->parent) {
        for (Symbol* s = t->symbols; s != NULL; s = s->next) {
            if (strcmp(s->name, name) == 0) return s;
        }
    }
    return NULL;
}

void semantic_check(Node *ast_root) {
    if (!ast_root) return;
    global_symbol_table = create_symbol_table("global", NULL);
    traverse_node(ast_root, global_symbol_table);
}

static void traverse_node(Node *node, SymbolTable *table) {
    if (!node) return;

    switch (node->type) {
        case NODE_PROGRAMA:
            traverse_node(node->left, table);
            traverse_node(node->right, table);
            break;
        case NODE_DECLARACAO:
            process_declarations(node, table);
            break;
        case NODE_ATRIBUICAO:
            process_assignment(node, table);
            break;
        case NODE_ID:
            process_identifier(node, table);
            break;
        case NODE_OP_SOMA:
        case NODE_OP_SUB:
        case NODE_OP_MULT:
        case NODE_OP_DIV:
        case NODE_OP_LT:
        case NODE_OP_LE:
        case NODE_OP_GT:
        case NODE_OP_GE:
        case NODE_OP_EQ:
        case NODE_OP_NE:
        case NODE_OP_AND:
        case NODE_OP_OR:
            process_expression(node, table);
            break;
        case NODE_SE:
        case NODE_SE_SENAO:
        case NODE_ENQUANTO:
        case NODE_REPITA_ATE:
            traverse_node(node->left, table);
            adjust_node_type(node->left, table);
            if (node->left->data_type != TYPE_INTEGER && node->left->data_type != TYPE_BOOLEAN) {
                fprintf(stderr, "Erro Semântico (Linha %d): Expressão de controle deve ser um valor booleano ou inteiro.\n", node->line);
                exit(EXIT_FAILURE);
            }
            traverse_node(node->right, table);
            break;
        default:
            traverse_node(node->left, table);
            traverse_node(node->right, table);
            break;
    }
    adjust_node_type(node, table);
}

static void process_declarations(Node *decl_node, SymbolTable *table) {
    DataType type = (decl_node->left->type == NODE_TIPO_INTEIRO) ? TYPE_INTEGER : TYPE_REAL;
    process_id_list(decl_node->right, table, type);
}

static void process_id_list(Node *id_list_node, SymbolTable *table, DataType type) {
    if (!id_list_node) return;
    if (id_list_node->type == NODE_ID_LISTA) {
        process_id_list(id_list_node->left, table, type);
        insert_symbol(table, id_list_node->right->value.sval, type, id_list_node->right->line);
    } else { // NODE_ID
        insert_symbol(table, id_list_node->value.sval, type, id_list_node->line);
    }
}

static void process_assignment(Node *node, SymbolTable *table) {
    traverse_node(node->left, table);  // ID
    traverse_node(node->right, table); // Expressão
    adjust_node_type(node->left, table);
    adjust_node_type(node->right, table);

    DataType target_type = node->left->data_type;
    DataType value_type = node->right->data_type;

    if (target_type == value_type) return;

    if (target_type == TYPE_REAL && value_type == TYPE_INTEGER) {
        node->right = create_type_conversion_node(node->right, TYPE_REAL);
        node->right->data_type = TYPE_REAL;
    } else if (target_type == TYPE_INTEGER && value_type == TYPE_BOOLEAN) {
        // Permitir atribuir booleano a inteiro
    }
    else {
        fprintf(stderr, "Erro Semântico (Linha %d): Tipos incompatíveis na atribuição. Não é possível atribuir %s a %s.\n",
                node->line, datatype_to_string(value_type), datatype_to_string(target_type));
        exit(EXIT_FAILURE);
    }
}

static void process_expression(Node *node, SymbolTable *table) {
    traverse_node(node->left, table);
    traverse_node(node->right, table);
    adjust_node_type(node->left, table);
    adjust_node_type(node->right, table);

    DataType left_type = node->left->data_type;
    DataType right_type = node->right->data_type;

    if (node->type == NODE_OP_AND || node->type == NODE_OP_OR) {
        if (left_type != TYPE_INTEGER && left_type != TYPE_BOOLEAN) {
            fprintf(stderr, "Erro Semântico (Linha %d): Operando esquerdo do operador lógico deve ser inteiro ou booleano.\n", node->line);
            exit(EXIT_FAILURE);
        }
        if (right_type != TYPE_INTEGER && right_type != TYPE_BOOLEAN) {
            fprintf(stderr, "Erro Semântico (Linha %d): Operando direito do operador lógico deve ser inteiro ou booleano.\n", node->line);
            exit(EXIT_FAILURE);
        }
        return;
    }

    if (left_type == right_type) return;

    if ((left_type == TYPE_INTEGER && right_type == TYPE_REAL)) {
        node->left = create_type_conversion_node(node->left, TYPE_REAL);
        node->left->data_type = TYPE_REAL;
    } else if (left_type == TYPE_REAL && right_type == TYPE_INTEGER) {
        node->right = create_type_conversion_node(node->right, TYPE_REAL);
        node->right->data_type = TYPE_REAL;
    }
}

static void process_identifier(Node *node, SymbolTable *table) {
    Symbol *symbol = lookup_symbol(table, node->value.sval);
    if (!symbol) {
        fprintf(stderr, "Erro Semântico (Linha %d): Variável '%s' não declarada.\n", node->line, node->value.sval);
        exit(EXIT_FAILURE);
    }
    node->symbol_entry = symbol;
    node->data_type = symbol->type;
}

static void adjust_node_type(Node *node, SymbolTable *table) {
    if (!node || node->data_type != TYPE_UNDEFINED) return;

    switch(node->type) {
        case NODE_NUM_INTEIRO: node->data_type = TYPE_INTEGER; break;
        case NODE_NUM_REAL: node->data_type = TYPE_REAL; break;
        case NODE_ID:
            if (node->symbol_entry) node->data_type = node->symbol_entry->type;
            break;
        case NODE_OP_SOMA: case NODE_OP_SUB: case NODE_OP_MULT: case NODE_OP_DIV:
            if (node->left->data_type == TYPE_REAL || node->right->data_type == TYPE_REAL)
                node->data_type = TYPE_REAL;
            else
                node->data_type = TYPE_INTEGER;
            break;
        case NODE_OP_LT: case NODE_OP_LE: case NODE_OP_GT: case NODE_OP_GE:
        case NODE_OP_EQ: case NODE_OP_NE: case NODE_OP_AND: case NODE_OP_OR:
            node->data_type = TYPE_INTEGER; // Result of logical/relational is integer (0 or 1)
            break;
        default: break;
    }
}

static Node* create_type_conversion_node(Node* child, DataType target_type) {
    Node* conv_node = createNode(NODE_TYPE_CONVERSION, child, NULL, child->line);
    conv_node->data_type = target_type;
    return conv_node;
}

const char* datatype_to_string(DataType type) {
    switch (type) {
        case TYPE_INTEGER: return "inteiro";
        case TYPE_REAL: return "real";
        case TYPE_BOOLEAN: return "booleano";
        default: return "indefinido";
    }
}

void print_symbol_table(SymbolTable* table) {
    if (!table) return;
    printf("--- Tabela de Símbolos (Escopo: %s) ---\n", table->scope_name);
    printf("% -20s | %-10s | %-15s | %-5s\n", "Nome", "Tipo", "Endereço (bytes)", "Linha");
    printf("----------------------------------------------------------\n");
    for (Symbol* s = table->symbols; s != NULL; s = s->next) {
        printf("% -20s | %-10s | %-15d | %-5d\n", s->name, datatype_to_string(s->type), s->memory_address, s->line);
    }
    printf("----------------------------------------------------------\n\n");
}