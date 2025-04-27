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

%}

%option noyywrap

%%


"def"       { tokens[contador++] = "def"; }
"int"       { tokens[contador++] = "int"; }
"if"        { tokens[contador++] = "if"; }
"else"      { tokens[contador++] = "else"; }
"print"     { tokens[contador++] = "print"; }
"return"    { tokens[contador++] = "return"; }


"<="        { tokens[contador++] = "<="; }
">="        { tokens[contador++] = ">="; }
"=="        { tokens[contador++] = "=="; }
"!="        { tokens[contador++] = "!="; }
"<"         { tokens[contador++] = "<"; }
">"         { tokens[contador++] = ">"; }
"+"         { tokens[contador++] = "+"; }
"-"         { tokens[contador++] = "-"; }
"*"         { tokens[contador++] = "*"; }
"/"         { tokens[contador++] = "/"; }
"="         { tokens[contador++] = "="; }


"("         { tokens[contador++] = "("; }
")"         { tokens[contador++] = ")"; }
"{"         { tokens[contador++] = "{"; }
"}"         { tokens[contador++] = "}"; }
","         { tokens[contador++] = ","; }
";"         { tokens[contador++] = ";"; }

[ \t\r\n]+  { }


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
    printf("Erro léxico: caractere inesperado '%s'\n", yytext); 
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
