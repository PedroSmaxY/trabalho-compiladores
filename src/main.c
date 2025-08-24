#include "lexer.h"
#include <stdio.h>

int main(const int argc, char *argv[]) {
    const char *file_path = argc > 1 ? argv[1] : "codigo.txt";

    FILE *fp = fopen(file_path, "r");
    if (!fp) {
        perror("Erro ao abrir arquivo");
        return 1;
    }

    while (1) {
        Token token = getToken(fp);
        printf("Token: %-10s Tipo: %d\n", token.lexeme, token.type);
        if (token.type == TOKEN_EOF)
            break;
    }

    fclose(fp);
    return 0;
}