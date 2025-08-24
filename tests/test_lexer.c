//
// Created by Pedro Novais on 24/08/25.
//

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "lexer.h"

static FILE *open_memory(const char *src) {
    return fmemopen((void*)src, strlen(src), "r");
}

static void test_basico() {
    const char *code =
        "inteiro a, b;\n"
        "a = 10;\n"
        "b = a + 2;\n"
        "se (a >= b) entao { b = b - 1; }\n";
    FILE *fp = open_memory(code);
    assert(fp);

    Token t;

    t = getToken(fp); assert(t.type == TOKEN_KEYWORD && strcmp(t.lexeme,"inteiro")==0);
    t = getToken(fp); assert(t.type == TOKEN_IDENTIFIER && strcmp(t.lexeme,"a")==0);
    t = getToken(fp); assert(t.type == TOKEN_SEPARATOR && strcmp(t.lexeme,",")==0);
    t = getToken(fp); assert(t.type == TOKEN_IDENTIFIER && strcmp(t.lexeme,"b")==0);
    t = getToken(fp); assert(t.type == TOKEN_SEPARATOR && strcmp(t.lexeme,";")==0);

    // a = 10;
    t = getToken(fp); assert(t.type == TOKEN_IDENTIFIER && strcmp(t.lexeme,"a")==0);
    t = getToken(fp); assert(t.type == TOKEN_OPERATOR && strcmp(t.lexeme,"=")==0);
    t = getToken(fp); assert(t.type == TOKEN_INTEGER && strcmp(t.lexeme,"10")==0);
    t = getToken(fp); assert(t.type == TOKEN_SEPARATOR && strcmp(t.lexeme,";")==0);

    // b = a + 2 ;
    t = getToken(fp); assert(t.type == TOKEN_IDENTIFIER && strcmp(t.lexeme,"b")==0);
    t = getToken(fp); assert(t.type == TOKEN_OPERATOR && strcmp(t.lexeme,"=")==0);
    t = getToken(fp); assert(t.type == TOKEN_IDENTIFIER && strcmp(t.lexeme,"a")==0);
    t = getToken(fp); assert(t.type == TOKEN_OPERATOR && strcmp(t.lexeme,"+")==0);
    t = getToken(fp); assert(t.type == TOKEN_INTEGER && strcmp(t.lexeme,"2")==0);
    t = getToken(fp); assert(t.type == TOKEN_SEPARATOR && strcmp(t.lexeme,";")==0);

    // se ( a >= b ) entao { b = b - 1 ; }
    t = getToken(fp); assert(t.type == TOKEN_KEYWORD && strcmp(t.lexeme,"se")==0);
    t = getToken(fp); assert(t.type == TOKEN_SEPARATOR && strcmp(t.lexeme,"(")==0);
    t = getToken(fp); assert(t.type == TOKEN_IDENTIFIER && strcmp(t.lexeme,"a")==0);
    t = getToken(fp); assert(t.type == TOKEN_OPERATOR && strcmp(t.lexeme,">=")==0);
    t = getToken(fp); assert(t.type == TOKEN_IDENTIFIER && strcmp(t.lexeme,"b")==0);
    t = getToken(fp); assert(t.type == TOKEN_SEPARATOR && strcmp(t.lexeme,")")==0);
    t = getToken(fp); assert(t.type == TOKEN_KEYWORD && strcmp(t.lexeme,"entao")==0);
    t = getToken(fp); assert(t.type == TOKEN_SEPARATOR && strcmp(t.lexeme,"{")==0);
    t = getToken(fp); assert(t.type == TOKEN_IDENTIFIER && strcmp(t.lexeme,"b")==0);
    t = getToken(fp); assert(t.type == TOKEN_OPERATOR && strcmp(t.lexeme,"=")==0);
    t = getToken(fp); assert(t.type == TOKEN_IDENTIFIER && strcmp(t.lexeme,"b")==0);
    t = getToken(fp); assert(t.type == TOKEN_OPERATOR && strcmp(t.lexeme,"-")==0);
    t = getToken(fp); assert(t.type == TOKEN_INTEGER && strcmp(t.lexeme,"1")==0);
    t = getToken(fp); assert(t.type == TOKEN_SEPARATOR && strcmp(t.lexeme,";")==0);
    t = getToken(fp); assert(t.type == TOKEN_SEPARATOR && strcmp(t.lexeme,"}")==0);

    // EOF
    t = getToken(fp); assert(t.type == TOKEN_EOF);

    fclose(fp);
}

static void test_float() {
    const char *code = "real x; x = 2.5;";
    FILE *fp = open_memory(code);
    assert(fp);
    Token t;
    t = getToken(fp); assert(t.type == TOKEN_KEYWORD && strcmp(t.lexeme,"real")==0);
    t = getToken(fp); assert(t.type == TOKEN_IDENTIFIER && strcmp(t.lexeme,"x")==0);
    t = getToken(fp); assert(t.type == TOKEN_SEPARATOR && strcmp(t.lexeme,";")==0);
    t = getToken(fp); assert(t.type == TOKEN_IDENTIFIER && strcmp(t.lexeme,"x")==0);
    t = getToken(fp); assert(t.type == TOKEN_OPERATOR && strcmp(t.lexeme,"=")==0);
    t = getToken(fp); assert(t.type == TOKEN_FLOAT && strcmp(t.lexeme,"2.5")==0);
    t = getToken(fp); assert(t.type == TOKEN_SEPARATOR && strcmp(t.lexeme,";")==0);
    t = getToken(fp); assert(t.type == TOKEN_EOF);
    fclose(fp);
}

int main(void) {
    test_basico();
    test_float();
    printf("Todos os testes passaram.\n");
    return 0;
}