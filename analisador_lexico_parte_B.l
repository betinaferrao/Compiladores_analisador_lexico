%{

/*
Analisador Léxico B - Linguagem LSI20251
Alunas: Betina Corazza Ferrão e Mariana Amaral Steffen 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_MAX 1000
char *tokens[TAM_MAX];
int contador = 0;
int linha = 1;
int coluna = 1;

%}

%option noyywrap

%%

"id"       { tokens[contador++] = "id"; coluna += yyleng; }
"def"      { tokens[contador++] = "def"; coluna += yyleng; }
"num"      { tokens[contador++] = "num"; coluna += yyleng; }
"int"      { tokens[contador++] = "int"; coluna += yyleng; }
"if"       { tokens[contador++] = "if"; coluna += yyleng; }
"else"     { tokens[contador++] = "else"; coluna += yyleng; }
"print"    { tokens[contador++] = "print"; coluna += yyleng; }
"return"   { tokens[contador++] = "return"; coluna += yyleng; }

"<="       { tokens[contador++] = "<="; coluna += yyleng; }
">="       { tokens[contador++] = ">="; coluna += yyleng; }
"=="       { tokens[contador++] = "=="; coluna += yyleng; }
"!="       { tokens[contador++] = "!="; coluna += yyleng; }
"<"        { tokens[contador++] = "<"; coluna += yyleng; }
">"        { tokens[contador++] = ">"; coluna += yyleng; }
"+"        { tokens[contador++] = "+"; coluna += yyleng; }
"-"        { tokens[contador++] = "-"; coluna += yyleng; }
"*"        { tokens[contador++] = "*"; coluna += yyleng; }
"/"        { tokens[contador++] = "/"; coluna += yyleng; }
"="        { tokens[contador++] = "="; coluna += yyleng; }

"("        { tokens[contador++] = "("; coluna += yyleng; }
")"        { tokens[contador++] = ")"; coluna += yyleng; }
"{"        { tokens[contador++] = "{"; coluna += yyleng; }
"}"        { tokens[contador++] = "}"; coluna += yyleng; }
","        { tokens[contador++] = ","; coluna += yyleng; }
";"        { tokens[contador++] = ";"; coluna += yyleng; }

[ \t\r]+   { coluna += yyleng; } 
\n         { linha++; coluna = 1; } 

[a-zA-Z_][a-zA-Z0-9_]* { 
    char *id_token = (char *)malloc(strlen(yytext) + 1); 
    strcpy(id_token, yytext);
    tokens[contador++] = id_token; 
}


[0-9]+ { 
    char *num_token = (char *)malloc(strlen(yytext) + 1); 
    strcpy(num_token, yytext);
    tokens[contador++] = num_token; 
}


. { 
    printf("Erro léxico na linha %d, coluna %d: caractere inesperado '%s'\n", linha, coluna, yytext);
    exit(1); 
}

%%


int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <arquivo_de_entrada>\n", argv[0]);
        exit(1);
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }

    yyin = file;
    yylex();
    fclose(file);

    printf("Tokens: [");
    for (int i = 0; i < contador; i++) {
        printf("%s", tokens[i]);
        if (i < contador - 1) {
            printf(", ");
        }
    }
    printf("]\n");

    return 0;
}
