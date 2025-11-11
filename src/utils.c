#include "utils.h"
#include "parser.h"

extern const char* node_type_names[];

const char* node_type_names[] = {
    "NODE_PROGRAMA", "NODE_BLOCO", "NODE_DECL_LISTA", "NODE_DECLARACAO", "NODE_TIPO_INTEIRO",
    "NODE_TIPO_REAL", "NODE_ID_LISTA", "NODE_CMD_LISTA", "NODE_SE", "NODE_SE_SENAO",
    "NODE_ENQUANTO", "NODE_REPITA_ATE", "NODE_LER", "NODE_MOSTRAR", "NODE_ATRIBUICAO",
    "NODE_ID", "NODE_NUM_INTEIRO", "NODE_NUM_REAL", "NODE_OP_SOMA", "NODE_OP_SUB",
    "NODE_OP_MULT", "NODE_OP_DIV", "NODE_OP_AND", "NODE_OP_OR", "NODE_OP_LT",
    "NODE_OP_LE", "NODE_OP_GT", "NODE_OP_GE", "NODE_OP_EQ", "NODE_OP_NE",
    "NODE_TYPE_CONVERSION"
};

const char* get_token_category(const int token) {
    switch (token) {
        case T_INTEIRO:         return "T_INTEIRO";
        case T_REAL:            return "T_REAL";
        case T_SE:              return "T_SE";
        case T_ENTAO:           return "T_ENTAO";
        case T_SENAO:           return "T_SENAO";
        case T_ENQUANTO:        return "T_ENQUANTO";
        case T_REPITA:          return "T_REPITA";
        case T_ATE:             return "T_ATE";
        case T_LER:             return "T_LER";
        case T_MOSTRAR:         return "T_MOSTRAR";
        case T_ID:              return "T_ID";
        case T_NUMERO_INTEIRO:  return "T_NUMERO_INTEIRO";
        case T_NUMERO_REAL:     return "T_NUMERO_REAL";
        case T_AND:             return "T_AND";
        case T_OR:              return "T_OR";
        case T_LE:              return "T_LE";
        case T_GE:              return "T_GE";
        case T_EQ:              return "T_EQ";
        case T_NE:              return "T_NE";
        case '+':               return "PLUS";
        case '-':               return "MINUS";
        case '*':               return "STAR";
        case '/':               return "SLASH";
        case '=':               return "EQUAL";
        case '<':               return "LT";
        case '>':               return "GT";
        case '(':               return "LPAREN";
        case ')':               return "RPAREN";
        case '{':               return "LBRACE";
        case '}':               return "RBRACE";
        case ',':               return "COMMA";
        case ';':               return "SEMICOLON";
        default:                return "UNKNOWN";
    }
}

void printAst(const Node* node, const int level) {
    printAstToFile(stdout, node, level);
}

void printSemanticAst(const Node* node, const int level) {
    printSemanticAstToFile(stdout, node, level);
}

void printAstToFile(FILE* fp, const Node* node, const int level) {
    if (node == NULL) return;
    for (int i = 0; i < level; i++) fprintf(fp, "  ");

    fprintf(fp, "Type: %s", node_type_names[node->type]);

    if (node->type == NODE_ID) fprintf(fp, ", Value: %s", node->value.sval);
    else if (node->type == NODE_NUM_INTEIRO) fprintf(fp, ", Value: %d", node->value.ival);
    else if (node->type == NODE_NUM_REAL) fprintf(fp, ", Value: %f", node->value.fval);

    fprintf(fp, "\n");

    printAstToFile(fp, node->left, level + 1);
    printAstToFile(fp, node->right, level + 1);
}

void printSemanticAstToFile(FILE* fp, const Node* node, const int level) {
    if (node == NULL) return;
    for (int i = 0; i < level; i++) fprintf(fp, "  ");

    const char* type_str = datatype_to_string(node->data_type);

    switch(node->type) {
        case NODE_ID:
            fprintf(fp, "Id: %s [Tipo: %s]\n", node->value.sval, type_str);
            break;
        case NODE_NUM_INTEIRO:
            fprintf(fp, "Const: %d [Tipo: %s]\n", node->value.ival, type_str);
            break;
        case NODE_NUM_REAL:
            fprintf(fp, "Const: %f [Tipo: %s]\n", node->value.fval, type_str);
            break;
        case NODE_ATRIBUICAO:
            fprintf(fp, "Atribuição [Tipo: %s]\n", type_str);
            break;
        case NODE_OP_SOMA: fprintf(fp, "Op: + [Tipo: %s]\n", type_str); break;
        case NODE_OP_SUB:  fprintf(fp, "Op: - [Tipo: %s]\n", type_str); break;
        case NODE_OP_MULT: fprintf(fp, "Op: * [Tipo: %s]\n", type_str); break;
        case NODE_OP_DIV:  fprintf(fp, "Op: / [Tipo: %s]\n", type_str); break;
        case NODE_OP_LT:   fprintf(fp, "Op: < [Tipo: %s]\n", type_str); break;
        case NODE_OP_GT:   fprintf(fp, "Op: > [Tipo: %s]\n", type_str); break;
        case NODE_OP_LE:   fprintf(fp, "Op: <= [Tipo: %s]\n", type_str); break;
        case NODE_OP_GE:   fprintf(fp, "Op: >= [Tipo: %s]\n", type_str); break;
        case NODE_OP_EQ:   fprintf(fp, "Op: == [Tipo: %s]\n", type_str); break;
        case NODE_OP_NE:   fprintf(fp, "Op: != [Tipo: %s]\n", type_str); break;
        case NODE_OP_AND:  fprintf(fp, "Op: E [Tipo: %s]\n", type_str); break;
        case NODE_OP_OR:   fprintf(fp, "Op: OU [Tipo: %s]\n", type_str); break;
        case NODE_SE:      fprintf(fp, "Se\n"); break;
        case NODE_ENQUANTO:fprintf(fp, "Enquanto\n"); break;
        case NODE_MOSTRAR: fprintf(fp, "Mostrar\n"); break;
        case NODE_LER:     fprintf(fp, "Ler\n"); break;
        case NODE_TYPE_CONVERSION:
            fprintf(fp, "Conversão para %s\n", type_str);
            break;
        default:
            break;
    }

    if (node->left) printSemanticAstToFile(fp, node->left, level + 1);
    if (node->right) printSemanticAstToFile(fp, node->right, level + 1);
}

void printSymbolTableToFile(FILE* fp, SymbolTable* table) {
    if (!table) return;
    fprintf(fp, "--- Tabela de Símbolos (Escopo: %s) ---\n", table->scope_name);
    fprintf(fp, "%-20s | %-10s | %-15s | %-5s\n", "Nome", "Tipo", "Endereço (bytes)", "Linha");
    fprintf(fp, "----------------------------------------------------------\n");
    for (Symbol* s = table->symbols; s != NULL; s = s->next) {
        fprintf(fp, "%-20s | %-10s | %-15d | %-5d\n", s->name, datatype_to_string(s->type), s->memory_address, s->line);
    }
    fprintf(fp, "----------------------------------------------------------\n\n");
}