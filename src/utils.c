#include <stdio.h>

#include "utils.h"

#include "ast.h"
#include "parser.h"

const char* node_type_names[] = {
    "NODE_PROGRAMA" ,   "NODE_BLOCO"     , "NODE_DECL_LISTA", "NODE_DECLARACAO" , "NODE_TIPO_INTEIRO",
    "NODE_TIPO_REAL",   "NODE_ID_LISTA"  , "NODE_CMD_LISTA" , "NODE_SE"         ,
    "NODE_SE_SENAO" ,   "NODE_ENQUANTO"  , "NODE_REPITA_ATE", "NODE_LER"        ,
    "NODE_MOSTRAR"  ,   "NODE_ATRIBUICAO", "NODE_ID"        , "NODE_NUM_INTEIRO",
    "NODE_NUM_REAL" ,   "NODE_OP_SOMA"   , "NODE_OP_SUB"    , "NODE_OP_MULT"    ,
    "NODE_OP_DIV"   ,   "NODE_OP_AND"    , "NODE_OP_OR"     , "NODE_OP_LT"      , "NODE_OP_LE"       ,
    "NODE_OP_GT"    ,   "NODE_OP_GE"     , "NODE_OP_EQ"     , "NODE_OP_NE"
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
    if (node == NULL) {
        return;
    }

    for (int i = 0; i < level; i++) {
        printf("  ");
    }

    printf("Node Type: %s", node_type_names[node->type]);

    if (node->type == NODE_ID || node->type == NODE_NUM_INTEIRO || node->type == NODE_NUM_REAL) {
        printf(", Value: ");
        if (node->type == NODE_ID) printf("%s", node->value.sval);
        else if (node->type == NODE_NUM_INTEIRO) printf("%d", node->value.ival);
        else if (node->type == NODE_NUM_REAL) printf("%f", node->value.fval);
    }
    printf("\n");

    printAst(node->left, level + 1);
    printAst(node->right, level + 1);
}

void printSemanticAst(const Node* node, const int level) {
    if (node == NULL) return;
    for (int i = 0; i < level; i++) printf("  ");

    switch(node->type) {
        case NODE_PROGRAMA:
        case NODE_BLOCO:
        case NODE_DECL_LISTA:
        case NODE_CMD_LISTA:
            printSemanticAst(node->left, level);
            printSemanticAst(node->right, level);
            break;
        case NODE_ATRIBUICAO:
            printf("Assign to: %s\n", node->left->value.sval);
            printSemanticAst(node->right, level + 1);
            break;
        case NODE_SE:
            printf("If\n");
            printSemanticAst(node->left, level + 1);
            if (node->right && node->right->type == NODE_SE_SENAO) {
                printSemanticAst(node->right->left, level + 1);
                for (int i = 0; i < level; i++) printf("  ");
                printf("Else\n");
                printSemanticAst(node->right->right, level + 1);
            } else {
                printSemanticAst(node->right, level + 1);
            }
            break;
        case NODE_ENQUANTO:
            printf("While\n");
            printSemanticAst(node->left, level + 1);
            printSemanticAst(node->right, level + 1);
            break;
        case NODE_REPITA_ATE:
            printf("Repeat\n");
            printSemanticAst(node->left, level + 1);
            for (int i = 0; i < level; i++) printf("  ");
            printf("Until\n");
            printSemanticAst(node->right, level + 1);
            break;
        case NODE_LER:
            printf("Read: ");
            const Node* current = node->left;
            while(current) {
                if (current->type == NODE_ID_LISTA) {
                    const Node* id_node = current->right;
                    if(id_node) printf("%s", id_node->value.sval);
                    current = current->left;
                    if(current) printf(", ");
                } else if (current->type == NODE_ID) {
                    printf("%s", current->value.sval);
                    current = NULL;
                }
            }
            printf("\n");
            break;
        case NODE_MOSTRAR:
            printf("Write\n");
            printSemanticAst(node->left, level + 1);
            break;
        case NODE_ID:
            printf("Id: %s\n", node->value.sval);
            break;
        case NODE_NUM_INTEIRO:
            printf("Const: %d\n", node->value.ival);
            break;
        case NODE_NUM_REAL:
            printf("Const: %f\n", node->value.fval);
            break;
        case NODE_OP_SOMA: printf("Op: +\n");  printSemanticAst(node->left, level+1); printSemanticAst(node->right, level+1); break;
        case NODE_OP_SUB:  printf("Op: -\n");  printSemanticAst(node->left, level+1); printSemanticAst(node->right, level+1); break;
        case NODE_OP_MULT: printf("Op: *\n");  printSemanticAst(node->left, level+1); printSemanticAst(node->right, level+1); break;
        case NODE_OP_DIV:  printf("Op: /\n");  printSemanticAst(node->left, level+1); printSemanticAst(node->right, level+1); break;
        case NODE_OP_LT:   printf("Op: <\n");  printSemanticAst(node->left, level+1); printSemanticAst(node->right, level+1); break;
        case NODE_OP_GT:   printf("Op: >\n");  printSemanticAst(node->left, level+1); printSemanticAst(node->right, level+1); break;
        case NODE_OP_LE:   printf("Op: <=\n"); printSemanticAst(node->left, level+1); printSemanticAst(node->right, level+1); break;
        case NODE_OP_GE:   printf("Op: >=\n"); printSemanticAst(node->left, level+1); printSemanticAst(node->right, level+1); break;
        case NODE_OP_EQ:   printf("Op: ==\n"); printSemanticAst(node->left, level+1); printSemanticAst(node->right, level+1); break;
        case NODE_OP_NE:   printf("Op: !=\n"); printSemanticAst(node->left, level+1); printSemanticAst(node->right, level+1); break;
        case NODE_OP_AND:  printf("Op: &&\n"); printSemanticAst(node->left, level+1); printSemanticAst(node->right, level+1); break;
        case NODE_OP_OR:   printf("Op: ||\n"); printSemanticAst(node->left, level+1); printSemanticAst(node->right, level+1); break;
        default:
            printSemanticAst(node->left, level);
            printSemanticAst(node->right, level);
    }
}