# Compiladores

Integrantes: Betina Corazza Ferrão e Mariana Amaral.

# Parte A
Analisador léxico baseado em diagrama de transição, que:

- Lê o arquivo caracter por caracter.
- Reconhece palavras-chave, identificadores, números inteiros e operadores relacionais (>, <, =, >=, <=, !=).
- Ignora parênteses, chaves, vírgulas, ponto e vírgula, +, -, %.

Detecta erros léxicos como:
- Caractere inválido (ex: @, #).
- Identificadores incorretos (ex: 123abc).
- Operadores inválidos (>>, <<).

Implementação de Tabela de símbolos para palavras-chave e identificadores.

Como Compilar e Executar
```
gcc -o analisador_lexico_parte_A analisador_lexico_manual_.c
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

# Parte 

