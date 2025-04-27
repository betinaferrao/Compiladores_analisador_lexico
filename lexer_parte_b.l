/* lexer_parte_b.l */
/* Integrantes: Betina Corazza Ferrão e Mariana Amaral */

%{
#include <stdio.h>
int linha = 1;
int coluna = 0;
int coluna_inicio = 0;
%}

digit      [0-9]
letter     [a-zA-Z]
id         {letter}({letter}|{digit})*
num        {digit}+

%%

"def"               { printf("PALAVRA_CHAVE(def) - Linha %d, Coluna %d\n", linha, coluna_inicio); }
"int"               { printf("PALAVRA_CHAVE(int) - Linha %d, Coluna %d\n", linha, coluna_inicio); }
"if"                { printf("PALAVRA_CHAVE(if) - Linha %d, Coluna %d\n", linha, coluna_inicio); }
"else"              { printf("PALAVRA_CHAVE(else) - Linha %d, Coluna %d\n", linha, coluna_inicio); }
"return"            { printf("PALAVRA_CHAVE(return) - Linha %d, Coluna %d\n", linha, coluna_inicio); }
"print"             { printf("PALAVRA_CHAVE(print) - Linha %d, Coluna %d\n", linha, coluna_inicio); }
"principal"         { printf("PALAVRA_CHAVE(principal) - Linha %d, Coluna %d\n", linha, coluna_inicio); }

">="|"<="|"=="|"!=" { printf("OP_RELACIONAL(%s) - Linha %d, Coluna %d\n", yytext, linha, coluna_inicio); }
">" | "<" | "="     { printf("OP_RELACIONAL(%s) - Linha %d, Coluna %d\n", yytext, linha, coluna_inicio); }

{id}                { printf("IDENT(%s) - Linha %d, Coluna %d\n", yytext, linha, coluna_inicio); }
{num}               { printf("NUM(%s) - Linha %d, Coluna %d\n", yytext, linha, coluna_inicio); }

[\(\)\{\},;\+\-\*/] { printf("SIMBOLO(%s) - Linha %d, Coluna %d\n", yytext, linha, coluna_inicio); }

[ \t]+              { coluna += yyleng; /* ignora espaços e tabulações */ }
\n                  { linha++; coluna = 0; }

.                   { printf("Erro léxico: '%s' - Linha %d, Coluna %d\n", yytext, linha, coluna_inicio); }

%%

int main() {
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}
