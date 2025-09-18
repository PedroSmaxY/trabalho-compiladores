#include "lexer.h"
#include "utils.h"
#include <stdio.h>

int main(const int argc, char *argv[]) {
    const char *file_path = (argc > 1) ? argv[1] : "codigo.txt";
    FILE *fp = fopen(file_path, "r");
    if (!fp) {
        perror("Erro ao abrir arquivo");
        fprintf(stderr, "Arquivo: %s\n", file_path);
        print_usage(argv[0]);
        return 1;
    }
    while (1) {
        Token tk = getToken(fp);
        printf("Linha %4d  Tipo %-10s  Lexema: %s\n",
               tk.line, tokenTypeToString(tk.type), tk.lexeme);
        if (tk.type == TOKEN_EOF) break;
    }
    fclose(fp);
    return 0;
}