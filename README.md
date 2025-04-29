# Compiladores

Integrantes: Betina Corazza Ferrão e Mariana Amaral.

Este programa é um analisador léxico construído manual e caracter por caracter, baseado em diagramas de transição.

O analisador reconhece os seguintes tokens:

- Palavras-chave: def, int, if, else, return, print
- Identificadores (variáveis e nomes de funções)
- Números inteiros
- Operadores relacionais: >, <, =, >=, <=, !=, ==
- Símbolos especiais: parênteses, chaves, vírgulas, ponto-e-vírgula, operadores aritméticos +, %
- Erros léxicos (como 123abc e símbolos inválidos)

Como Compilar e Executar

```
gcc -o analisador_lexico_parte_A analisador_lexico_parte_A.c
./analisador_lexico_parte_A entrada_correta.txt
./analisador_lexico_parte_A entrada_incorreta.txt
```

Saída

- Lista de tokens reconhecidos.
- Mensagens de erro (se existirem).
- Impressão da tabela de símbolos ao final.

Observações:

- Implementa estratégia Maximal Munch.
- Baseado em máquina de estados (diagrama de transição).

# Parte B

Como Compilar e Executar

```
flex analisador_lexico_parte_B.l
gcc -o lexer lex.yy.c -lfl
```

Depois, escolher uma das opções de teste:

```
./lexer entrada_invalida.lsi
./lexer entrada_valida.lsi

```

Saída

- Se a compilação funcionar, uma lista de tokens reconhecidos.
- Se um erro for encontrado, uma mensagem de erro e o caractere inesperado.

Observações:

- Identifica somente os tokens da linguagem LSI20251
