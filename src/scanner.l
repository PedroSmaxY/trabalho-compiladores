%{
#include <stdio.h>
#include <string.h>
#include "ast.h"
#include "parser.h"

void yyerror(const char *s);

%}

%option noyywrap
%option yylineno

DIGITO      [0-9]
LETRA       [a-zA-Z_]
ID          {LETRA}({LETRA}|{DIGITO})*
NUM_INT     {DIGITO}+
NUM_REAL    {DIGITO}+\.{DIGITO}+

%%
[ \t\r]+                   { /* Ignora espaços em branco */ }
\n                         { /* Ignora quebras de linha */ }
\/\/.*                     { /* Ignora comentários de linha única */ }
"/*"([^*]|\*+[^*/])*\*+"/" { /* Ignora comentários */ }

"inteiro"                  { return T_INTEIRO; }
"real"                     { return T_REAL; }
"se"                       { return T_SE; }
"entao"                    { return T_ENTAO; }
"senao"                    { return T_SENAO; }
"enquanto"                 { return T_ENQUANTO; }
"repita"                   { return T_REPITA; }
"ate"                      { return T_ATE; }
"ler"                      { return T_LER; }
"mostrar"                  { return T_MOSTRAR; }

"&&"                       { return T_AND; }
"||"                       { return T_OR; }
"<="                       { return T_LE; }
">="                       { return T_GE; }
"=="                       { return T_EQ; }
"!="                       { return T_NE; }
"+"                        { return '+'; }
"-"                        { return '-'; }
"*"                        { return '*'; }
"/"                        { return '/'; }
"="                        { return '='; }
"<"                        { return '<'; }
">"                        { return '>'; }
";"                        { return ';'; }
","                        { return ','; }
"("                        { return '('; }
")"                        { return ')'; }
"{"                        { return '{'; }
"}"                        { return '}'; }

{NUM_REAL} {
    yylval.fval = atof(yytext);
    return T_NUMERO_REAL;
}

{NUM_INT} {
    yylval.ival = atoi(yytext);
    return T_NUMERO_INTEIRO;
}

{ID} {
    yylval.sval = strdup(yytext);
    return T_ID;
}

. {
    fprintf(stderr, "Erro lexico na linha %d: Caractere invalido '%s'\n", yylineno, yytext);
}
%%

