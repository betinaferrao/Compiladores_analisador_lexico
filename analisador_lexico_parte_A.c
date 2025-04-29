// Analisador Léxico A
// Integrantes: Betina Corazza Ferrão e Mariana Amaral Steffen 

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
    TOKEN_PARENTESES,
    TOKEN_CHAVES,
    TOKEN_SEPARADOR,
    TOKEN_OPERADOR,
    TOKEN_ERRO
} TipoToken;


// Estrutura para armazenar um token lido
typedef struct
{
    TipoToken tipo;
    char lexema[MAX_TOKEN_LEN];
} Token;

// Estrutura para tabela de símbolos
typedef struct
{
    char lexema[MAX_TOKEN_LEN];
    TipoToken tipo;
} Simbolo;

Simbolo tabela_simbolos[MAX_SIMBOLOS];
char mensagem_erro[MAX_ERROS][TAMANHO_MSG];
int error_count = 0;
int num_simbolos = 0;

// Palavras-chave conhecidas
const char *palavras_chave[] = {
    "def", "int", "if", "else", "return", "print"};
const int num_palavras_chave = sizeof(palavras_chave) / sizeof(palavras_chave[0]);


// Adiciona os tokens na tabela de simbolos
void adicionar_tabela_simbolos(const char *lexema, TipoToken tipo)
{
    for (int i = 0; i < num_simbolos; i++)
    {
        if (strcmp(tabela_simbolos[i].lexema, lexema) == 0 && tabela_simbolos[i].tipo == tipo)
        {
            return;
        }
    }
    strcpy(tabela_simbolos[num_simbolos].lexema, lexema);
    tabela_simbolos[num_simbolos].tipo = tipo;
    num_simbolos++;
}

void resumo_dos_erros(Token token, int linha, int coluna)
{
    sprintf(mensagem_erro[error_count], "(%s): Linha: %d Coluna: %d\n", token.lexema, linha, coluna);
    error_count++;
}

// Mostra a tabela de simbolos
void mostrar_tabela_simbolos()
{
    if (error_count > 0)
    {
        printf("\nResumo dos erros:\n");
        printf("-------------------\n");
        printf("\033[0;31m");
        for (int i = 0; i < error_count; i++)
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
            const char *tipo_str;
            switch (tabela_simbolos[i].tipo)
            {
            case TOKEN_IDENT:
                tipo_str = "IDENTIFICADOR";
                break;
            case TOKEN_PALAVRA_CHAVE:
                tipo_str = "PALAVRA_CHAVE";
                break;
            case TOKEN_NUM:
                tipo_str = "NUM";
                break;
            case TOKEN_OP_RELACIONAL:
                tipo_str = "OP_RELACIONAL";
                break;
            case TOKEN_PARENTESES:
                tipo_str = "PARENTESES";
                break;
            case TOKEN_CHAVES:
                tipo_str = "CHAVES";
                break;
            case TOKEN_SEPARADOR:
                tipo_str = "SEPARADOR";
                break;
            case TOKEN_OPERADOR:
                tipo_str = "OPERADOR";
                break;
            default:
                tipo_str = "DESCONHECIDO";
                break;
            }

            printf("%s: %s\n", tabela_simbolos[i].lexema, tipo_str);

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
    case TOKEN_PARENTESES:
        tipo_str = "PARENTESES";
        break;
    case TOKEN_CHAVES:
        tipo_str = "CHAVES";
        break;
    case TOKEN_SEPARADOR:
        tipo_str = "SEPARADOR";
        break;
    case TOKEN_OPERADOR:
        tipo_str = "OPERADOR";
        break;
    case TOKEN_ERRO:
        tipo_str = "ERRO";
        break;
    default:
        tipo_str = "DESCONHECIDO";
        break;
    }

    if (token.tipo == TOKEN_ERRO)
    {
        printf("\033[0;31m");
        printf("%s(%s) - Linha %d, Coluna %d\n", tipo_str, token.lexema, linha, coluna);
        printf("\033[0m");
        resumo_dos_erros(token, linha, coluna);
    }
    else
    {
        printf("%s(%s)\n", tipo_str, token.lexema);

        if (token.tipo == TOKEN_IDENT ||
            token.tipo == TOKEN_PALAVRA_CHAVE ||
            token.tipo == TOKEN_NUM ||
            token.tipo == TOKEN_OP_RELACIONAL)
        {
            adicionar_tabela_simbolos(token.lexema, token.tipo);
        }
    }
}


// Analisador léxico principal baseado nos diagramas de transição
void analisador_lexico(FILE *arquivo)
{
    char c;
    char palavra[MAX_TOKEN_LEN];
    int estado = 0; // 0: START, 1: IDENT, 2: NUM, 3: OP_REL
    int linha = 1, coluna = 0;
    int idx = 0;
    int coluna_inicio = 0;

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
            else if (c == '(' || c == ')')
            {
                Token token;
                token.tipo = TOKEN_PARENTESES;
                token.lexema[0] = c;
                token.lexema[1] = '\0';
                imprimir_token(token, linha, coluna);
            }
            else if (c == '{' || c == '}')
            {
                Token token;
                token.tipo = TOKEN_CHAVES;
                token.lexema[0] = c;
                token.lexema[1] = '\0';
                imprimir_token(token, linha, coluna);
            }
            else if (c == ',' || c == ';')
            {
                Token token;
                token.tipo = TOKEN_SEPARADOR;
                token.lexema[0] = c;
                token.lexema[1] = '\0';
                imprimir_token(token, linha, coluna);
            }
            else if (c == '+' || c == '%')
            {
                Token token;
                token.tipo = TOKEN_OPERADOR;
                token.lexema[0] = c;
                token.lexema[1] = '\0';
                imprimir_token(token, linha, coluna);
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
                int encontrado = 0;
                for (int i = 0; i < num_simbolos; i++)
                {
                    if (strcmp(tabela_simbolos[i].lexema, palavra) == 0)
                    {
                        token.tipo = tabela_simbolos[i].tipo;
                        encontrado = 1;
                        break;
                    }
                }
                if (!encontrado)
                {
                    token.tipo = TOKEN_IDENT;
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
            if (c == '=' && (palavra[0] == '>' || palavra[0] == '<' || palavra[0] == '!' || palavra[0] == '=')) {
                palavra[idx++] = c;
                palavra[idx] = '\0';
                Token token;
                token.tipo = TOKEN_OP_RELACIONAL;
                strcpy(token.lexema, palavra);
                imprimir_token(token, linha, coluna_inicio);
                estado = 0;
                idx = 0;
            } else if (palavra[0] == '!' && c != '=' ) {
                palavra[idx] = '\0';
                Token token;
                token.tipo = TOKEN_ERRO;
                strcpy(token.lexema, palavra);
                imprimir_token(token, linha, coluna_inicio);
                estado = 0;
                idx = 0;
                ungetc(c, arquivo);
                coluna--;
            } else {
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
        int encontrado = 0;
        for (int i = 0; i < num_simbolos; i++)
        {
            if (strcmp(tabela_simbolos[i].lexema, palavra) == 0)
            {
                token.tipo = tabela_simbolos[i].tipo;
                encontrado = 1;
                break;
            }
        }
        if (!encontrado)
        {
            token.tipo = TOKEN_IDENT;
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

void inicializar_tabela_simbolos()
{
    for (int i = 0; i < num_palavras_chave; i++)
    {
        adicionar_tabela_simbolos(palavras_chave[i], TOKEN_PALAVRA_CHAVE);
    }
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
    inicializar_tabela_simbolos();
    analisador_lexico(arquivo);

    fclose(arquivo);
    return 0;
}
