%{

/*
Analisador Léxico B - Linguagem LSI20251
Alunas: Betina Corazza Ferrão e Mariana Amaral Steffen 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_MAX 1000
#define LEN_MAX 100

typedef struct {
    char nome[100];
    char tipo[20]; 
} Simbolo;

char *tokens[TAM_MAX];
Simbolo tabela_simbolos[TAM_MAX];
int num_simbolos = 0;
int contador = 0;
int linha = 1;
int coluna = 1;

void inserir_simbolo(const char *nome, const char *tipo) {
    strcpy(tabela_simbolos[num_simbolos].nome, nome);
    strcpy(tabela_simbolos[num_simbolos].tipo, tipo);
    num_simbolos++;
}

const char* buscar_simbolo(const char *nome) {
    for (int i = 0; i < num_simbolos; i++) {
        if (strcmp(tabela_simbolos[i].nome, nome) == 0) {
            return tabela_simbolos[i].tipo;
        }
    }
    return NULL; 
}


%}

%option noyywrap

%%

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
    const char* tipo = buscar_simbolo(yytext);
    if(tipo == NULL) {
        inserir_simbolo(yytext, "IDENTIFICADOR");
    } 
    tokens[contador++] = strdup(yytext);
    coluna += yyleng;
}


[0-9]+ { 
    char *num_token = (char *)malloc(strlen(yytext) + 1); 
    strcpy(num_token, yytext);
    tokens[contador++] = num_token; 
    coluna += yyleng;
}


. { 
    printf("Erro léxico na linha %d, coluna %d: caractere inesperado '%s'\n", linha, coluna, yytext);
    exit(1); 
}

%%


int main(int argc, char **argv) {

    inserir_simbolo("if", "PALAVRA_CHAVE");
    inserir_simbolo("else", "PALAVRA_CHAVE");
    inserir_simbolo("num", "PALAVRA_CHAVE");
    inserir_simbolo("print", "PALAVRA_CHAVE");
    inserir_simbolo("return", "PALAVRA_CHAVE");
    inserir_simbolo("def", "PALAVRA_CHAVE");
    inserir_simbolo("int", "PALAVRA_CHAVE");

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

    printf("Tabela de símbolos:");
    for (int i = 0; i < num_simbolos; i++) {
        printf("\n%s: %s", tabela_simbolos[i].nome, tabela_simbolos[i].tipo);
    }
    printf("\n");

    printf("\nTokens: [");
    for (int i = 0; i < contador; i++) {
        printf("%s", tokens[i]);
        if (i < contador - 1) {
            printf(", ");
        }
    }
    printf("]\n");

    return 0;
}
