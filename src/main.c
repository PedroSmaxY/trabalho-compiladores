#include "lexer.h"
#include <stdio.h>

int main() {
    FILE *fp = fopen("../codigo.txt", "r");
    if (!fp) {
        perror("Erro ao abrir arquivo");
        return 1;
    }

    Token token;
    do {
        token = getToken(fp);
        printf("Token: %-10s Tipo: %d\n", token.lexeme, token.type);
    } while (token.type != TOKEN_EOF);

    fclose(fp);
    return 0;
}
