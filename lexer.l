%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int linha = 1;
int coluna = 1;
int houve_erro = 0;

#define MAX_TOKENS 10000
char *tokens[MAX_TOKENS];
int token_index = 0;

void adicionar_token(const char *token);
void erro_lexico(const char *yytext);
%}

%option noyywrap

DIGITO      [0-9]
LETRA       [a-zA-Z]
ID          {LETRA}({LETRA}|{DIGITO})*
NUM         {DIGITO}+

%%
"def"        { adicionar_token("<DEF>"); coluna += yyleng; }
"int"        { adicionar_token("<INT>"); coluna += yyleng; }
"if"         { adicionar_token("<IF>"); coluna += yyleng; }
"else"       { adicionar_token("<ELSE>"); coluna += yyleng; }
"print"      { adicionar_token("<PRINT>"); coluna += yyleng; }
"return"     { adicionar_token("<RETURN>"); coluna += yyleng; }
"=="         { adicionar_token("<EQ>"); coluna += yyleng; }
"!="         { adicionar_token("<NEQ>"); coluna += yyleng; }
"<="         { adicionar_token("<LE>"); coluna += yyleng; }
">="         { adicionar_token("<GE>"); coluna += yyleng; }
"<"          { adicionar_token("<LT>"); coluna += yyleng; }
">"          { adicionar_token("<GT>"); coluna += yyleng; }
"+"          { adicionar_token("<PLUS>"); coluna += yyleng; }
"-"          { adicionar_token("<MINUS>"); coluna += yyleng; }
"*"          { adicionar_token("<MULT>"); coluna += yyleng; }
"/"          { adicionar_token("<DIV>"); coluna += yyleng; }
"="          { adicionar_token("<ASSIGN>"); coluna += yyleng; }
"("          { adicionar_token("<LPAREN>"); coluna += yyleng; }
")"          { adicionar_token("<RPAREN>"); coluna += yyleng; }
"{"          { adicionar_token("<LBRACE>"); coluna += yyleng; }
"}"          { adicionar_token("<RBRACE>"); coluna += yyleng; }
","          { adicionar_token("<COMMA>"); coluna += yyleng; }
";"          { adicionar_token("<SEMI>"); coluna += yyleng; }

{ID}         { 
                char buffer[256];
                snprintf(buffer, sizeof(buffer), "<ID:%s>", yytext);
                adicionar_token(buffer);
                coluna += yyleng;
             }

{NUM}        { 
                char buffer[256];
                snprintf(buffer, sizeof(buffer), "<NUM:%s>", yytext);
                adicionar_token(buffer);
                coluna += yyleng;
             }

[\t ]+       { coluna += yyleng; }
\n           { linha++; coluna = 1; }

.            { erro_lexico(yytext); }

%%

void adicionar_token(const char *token) {
    if (houve_erro) return;
    if (token_index < MAX_TOKENS) {
        tokens[token_index++] = strdup(token);
    }
}

void erro_lexico(const char *yytext) {
    fprintf(stderr, "\nErro léxico: caractere inválido '%s' na linha %d, coluna %d\n", yytext, linha, coluna);
    houve_erro = 1;
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <arquivo_de_entrada>\n", argv[0]);
        exit(1);
    }
    
    FILE *arquivo = fopen(argv[1], "r");
    if (!arquivo) {
        perror("Erro ao abrir arquivo");
        exit(1);
    }
    
    yyin = arquivo;
    yylex();
    fclose(arquivo);

    if (!houve_erro) {
        for (int i = 0; i < token_index; i++) {
            printf("%s ", tokens[i]);
            free(tokens[i]);
        }
        printf("\n");
    }

    return houve_erro ? 1 : 0;
}
