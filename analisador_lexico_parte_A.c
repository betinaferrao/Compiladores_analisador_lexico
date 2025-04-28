// Analisador_lexico
// Integrantes: Betina Corazza Ferrão e Mariana Amaral.

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_TOKEN_LEN 100
#define MAX_SIMBOLOS 1000
#define MAX_ERROS 100
#define TAMANHO_MSG 256

// Tipos de tokens reconhecidos
typedef enum
{
    TOKEN_IDENT,
    TOKEN_NUM,
    TOKEN_OP_RELACIONAL,
    TOKEN_PALAVRA_CHAVE,
    TOKEN_ERRO
} TipoToken;

// Estrutura para armazenar um token lido
typedef struct
{
    TipoToken tipo;
    char lexema[MAX_TOKEN_LEN];
} Token;

// Estrutura para tabela de símbolos (identificadores e palavras-chave)
typedef struct
{
    char lexema[MAX_TOKEN_LEN];
    TipoToken tipo;
} Simbolo;

// Tabela de símbolos global
Simbolo tabela_simbolos[MAX_SIMBOLOS];
char mensagem_erro[MAX_ERROS][TAMANHO_MSG];
int error_count = 0;
int num_simbolos = 0;

// Palavras-chave conhecidas
const char *palavras_chave[] = {
    "def", "int", "if", "else", "return", "print", "principal"};
const int num_palavras_chave = sizeof(palavras_chave) / sizeof(palavras_chave[0]);

// Verifica se uma palavra é palavra-chave
int eh_palavra_chave(const char *palavra)
{
    for (int i = 0; i < num_palavras_chave; i++)
    {
        if (strcmp(palavra, palavras_chave[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

// Adiciona identificador ou palavra-chave na tabela de símbolos
void adicionar_tabela_simbolos(const char *lexema, TipoToken tipo)
{
    for (int i = 0; i < num_simbolos; i++)
    {
        if (strcmp(tabela_simbolos[i].lexema, lexema) == 0)
        {
            return; // Já existe
        }
    }
    strcpy(tabela_simbolos[num_simbolos].lexema, lexema);
    tabela_simbolos[num_simbolos].tipo = tipo;
    num_simbolos++;
}

void resumo_dos_erros(Token token, int linha, int coluna)
{
    // printf("%s", token.lexema); // teste
    sprintf(mensagem_erro[error_count], "(%s): Linha: %d Coluna: %d\n", token.lexema, linha, coluna);
    // printf("%s", mensagem_erro[error_count]);
    error_count++;
}

void mostrar_tabela_simbolos()
{
    if (error_count > 0)
    {
        printf("\nResumo dos erros:\n");
        printf("-------------------\n");
        printf("\033[0;31m");
        for (int i = 0; i <= error_count; i++)
        {
            printf("%s", mensagem_erro[i]);
        }
        printf("\033[0m");
    }
    else
    {
        printf("\nTabela de Símbolos:\n");
        printf("-------------------\n");
        for (int i = 0; i < num_simbolos; i++)
        {
            printf("%s: %s\n", tabela_simbolos[i].lexema,
                   tabela_simbolos[i].tipo == TOKEN_PALAVRA_CHAVE ? "PALAVRA_CHAVE" : "IDENTIFICADOR");
        }
        printf("\033[0;32m");
        printf("\nProcessamento realizado com sucesso.\n");
        printf("\033[0m");
    }
}

// Função para imprimir um token
void imprimir_token(Token token, int linha, int coluna)
{
    const char *tipo_str;
    switch (token.tipo)
    {
    case TOKEN_IDENT:
        tipo_str = "IDENT";
        break;
    case TOKEN_NUM:
        tipo_str = "NUM";
        break;
    case TOKEN_OP_RELACIONAL:
        tipo_str = "OP_RELACIONAL";
        break;
    case TOKEN_PALAVRA_CHAVE:
        tipo_str = "PALAVRA_CHAVE";
        break;
    case TOKEN_ERRO:
        printf("\033[0;31m");
        resumo_dos_erros(token, linha, coluna);
        tipo_str = "ERRO";
        break;
    default:
        tipo_str = "DESCONHECIDO";
        break;
    }
    printf("%s(%s) - Linha %d, Coluna %d\n", tipo_str, token.lexema, linha, coluna);
    printf("\033[0m");
}

// Analisador léxico principal
void analisador_lexico(FILE *arquivo)
{
    char c;
    char palavra[MAX_TOKEN_LEN];
    int estado = 0; // 0: START, 1: IDENT, 2: NUM, 3: OP_REL
    int linha = 1, coluna = 0;
    int idx = 0;
    int coluna_inicio = 0; // salva coluna de início do token

    while ((c = fgetc(arquivo)) != EOF)
    {
        coluna++;
        if (c == '\n')
        {
            linha++;
            coluna = 0;
        }

        switch (estado)
        {
        case 0: // START
            if (isspace(c))
            {
                // Ignorar
            }
            else if (isalpha(c))
            {
                palavra[0] = c;
                idx = 1;
                estado = 1; // IDENT
                coluna_inicio = coluna;
            }
            else if (isdigit(c))
            {
                palavra[0] = c;
                idx = 1;
                estado = 2; // NUM
                coluna_inicio = coluna;
            }
            else if (c == '>' || c == '<' || c == '=' || c == '!')
            {
                palavra[0] = c;
                idx = 1;
                estado = 3; // OP_REL
                coluna_inicio = coluna;
            }
            else if (c == '(' || c == ')' || c == '{' || c == '}' || c == ',' || c == ';' || c == '+' || c == '-' || c == '%')
            {
                // Ignora esses símbolos
            }
            else
            {
                Token token;
                token.tipo = TOKEN_ERRO;
                token.lexema[0] = c;
                token.lexema[1] = '\0';
                imprimir_token(token, linha, coluna);
            }
            break;

        case 1: // IDENT
            if (isalnum(c))
            {
                if (idx < MAX_TOKEN_LEN - 1)
                {
                    palavra[idx++] = c;
                }
            }
            else
            {
                palavra[idx] = '\0';
                Token token;
                if (eh_palavra_chave(palavra))
                {
                    token.tipo = TOKEN_PALAVRA_CHAVE;
                    adicionar_tabela_simbolos(palavra, TOKEN_PALAVRA_CHAVE);
                }
                else
                {
                    token.tipo = TOKEN_IDENT;
                    adicionar_tabela_simbolos(palavra, TOKEN_IDENT);
                }
                strcpy(token.lexema, palavra);
                imprimir_token(token, linha, coluna_inicio);

                estado = 0;
                idx = 0;
                ungetc(c, arquivo);
                coluna--;
            }
            break;

        case 2: // NUM
            if (isdigit(c))
            {
                if (idx < MAX_TOKEN_LEN - 1)
                {
                    palavra[idx++] = c;
                }
            }
            else if (isalpha(c))
            {
                if (idx < MAX_TOKEN_LEN - 1)
                {
                    palavra[idx++] = c;
                }
                while ((c = fgetc(arquivo)) != EOF && (isalnum(c)))
                {
                    if (idx < MAX_TOKEN_LEN - 1)
                    {
                        palavra[idx++] = c;
                    }
                    coluna++;
                }
                palavra[idx] = '\0';

                Token token;
                token.tipo = TOKEN_ERRO;
                strcpy(token.lexema, palavra);
                imprimir_token(token, linha, coluna_inicio);

                estado = 0;
                idx = 0;

                if (c != EOF)
                {
                    ungetc(c, arquivo);
                    coluna--;
                }
            }
            else
            {
                palavra[idx] = '\0';
                Token token;
                token.tipo = TOKEN_NUM;
                strcpy(token.lexema, palavra);
                imprimir_token(token, linha, coluna_inicio);

                estado = 0;
                idx = 0;
                ungetc(c, arquivo);
                coluna--;
            }
            break;

        case 3: // OP_REL
            if (c == '=')
            {
                palavra[idx++] = c;
                palavra[idx] = '\0';
                Token token;
                token.tipo = TOKEN_OP_RELACIONAL;
                strcpy(token.lexema, palavra);
                imprimir_token(token, linha, coluna_inicio);
                estado = 0;
                idx = 0;
            }
            else if ((palavra[0] == '>' && c == '>') ||
                     (palavra[0] == '<' && (c == '<' || (c != ' ' && c != '='))) ||
                     (palavra[0] == '!' && (c != '=' && c != ' ')) ||
                     (palavra[0] == '=' && (c != '=' && c != ' ')))

            {
                palavra[1] = c;
                palavra[2] = '\0';
                Token token;
                token.tipo = TOKEN_ERRO;
                strcpy(token.lexema, palavra);
                imprimir_token(token, linha, coluna_inicio);
                estado = 0;
                idx = 0;
            }
            else
            {
                palavra[idx] = '\0';
                Token token;
                token.tipo = TOKEN_OP_RELACIONAL;
                strcpy(token.lexema, palavra);
                imprimir_token(token, linha, coluna_inicio);
                estado = 0;
                idx = 0;
                ungetc(c, arquivo);
                coluna--;
            }
            break;
        }
    }

    // Finaliza se terminar lendo token
    if (estado == 1)
    {
        palavra[idx] = '\0';
        Token token;
        if (eh_palavra_chave(palavra))
        {
            token.tipo = TOKEN_PALAVRA_CHAVE;
            adicionar_tabela_simbolos(palavra, TOKEN_PALAVRA_CHAVE);
        }
        else
        {
            token.tipo = TOKEN_IDENT;
            adicionar_tabela_simbolos(palavra, TOKEN_IDENT);
        }
        strcpy(token.lexema, palavra);
        imprimir_token(token, linha, coluna_inicio);
    }
    else if (estado == 2)
    {
        palavra[idx] = '\0';
        Token token;
        token.tipo = TOKEN_NUM;
        strcpy(token.lexema, palavra);
        imprimir_token(token, linha, coluna_inicio);
    }
    else if (estado == 3)
    {
        palavra[idx] = '\0';
        Token token;
        token.tipo = TOKEN_OP_RELACIONAL;
        strcpy(token.lexema, palavra);
        imprimir_token(token, linha, coluna_inicio);
    }

    mostrar_tabela_simbolos();
}

// Função principal
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Uso: %s caminho/do/arquivo\n", argv[0]);
        return 1;
    }

    FILE *arquivo = fopen(argv[1], "r");
    if (!arquivo)
    {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    printf("  Processamento das Linhas  \n");
    printf("----------------------------\n");
    analisador_lexico(arquivo);

    fclose(arquivo);
    return 0;
}
