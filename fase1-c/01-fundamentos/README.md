# 01 — Fundamentos de C

> *"First, solve the problem. Then, write the code."*

---

## Antes de começar

Certifique-se de que você já:

- [ ] Sabe usar o terminal básico: navegar diretórios, criar arquivos (`fase0-fundacao/04-terminal-linux`)
- [ ] Entende o que é compilação: pré-processador → compilador → linker (`fase0-fundacao/06-compilacao`)
- [ ] Tem `gcc` instalado: `gcc --version` deve retornar algo

---

## O que você vai aprender

Ao final deste módulo você será capaz de:

- Escrever, compilar e executar um programa C do zero
- Usar os tipos primitivos corretos para cada situação
- Controlar o fluxo do programa com if, for, while, switch
- Escrever funções com parâmetros e retorno, incluindo recursão
- Manipular arrays e strings sem bibliotecas externas
- Ler e escrever dados com `printf`, `scanf`, `fgets`, `getchar`

---

## 1. Estrutura de um Programa C

Todo programa C começa com `main`. O sistema operacional chama `main` quando você executa o programa.

```c
#include <stdio.h>   // inclui declarações de printf, scanf, etc.

int main(void)       // retorna int para o SO; void = sem argumentos
{
    printf("Hello, World!\n");
    return 0;        // 0 = sucesso; qualquer outro valor = erro
}
```

```bash
gcc -Wall -Wextra -Werror main.c -o hello
./hello
echo $?   # imprime o valor retornado por main (exit code)
```

**→ Exercício:** [`ex01-hello/`](ex01-hello/main.c)

---

## 2. Tipos Primitivos

### Inteiros

| Tipo | Tamanho típico | Range (signed) |
|------|---------------|----------------|
| `char` | 1 byte | -128 a 127 |
| `short` | 2 bytes | -32768 a 32767 |
| `int` | 4 bytes | ~-2 bilhões a ~2 bilhões |
| `long` | 8 bytes (64-bit Linux) | muito grande |
| `long long` | 8 bytes | muito grande |

Prefixo `unsigned` elimina negativos, dobra o range positivo:
```c
unsigned int x = 4294967295;  // 2^32 - 1
```

### Ponto Flutuante

| Tipo | Tamanho | Precisão |
|------|---------|----------|
| `float` | 4 bytes | ~7 dígitos decimais |
| `double` | 8 bytes | ~15 dígitos decimais |

```c
double pi = 3.14159265358979;
float  pi_f = 3.14159f;   // sufixo f para float literal
```

> ⚠️ Nunca compare floats com `==`. Use `fabs(a - b) < EPSILON`.

### Char

`char` é apenas um inteiro de 1 byte. Letras são números na tabela ASCII:
```c
char c = 'A';        // equivalente a: char c = 65;
printf("%c %d\n", c, c);  // imprime: A 65
c = c + 1;           // agora é 'B'
```

### sizeof

```c
printf("int:    %zu bytes\n", sizeof(int));
printf("double: %zu bytes\n", sizeof(double));
printf("char:   %zu bytes\n", sizeof(char));  // sempre 1
```

**→ Exercício:** [`ex02-tipos/`](ex02-tipos/main.c)

---

## 3. Operadores

### Aritméticos
```c
int a = 17, b = 5;
a + b   // 22
a - b   // 12
a * b   // 85
a / b   // 3   (divisão inteira — trunca!)
a % b   // 2   (resto da divisão)
```

> Divisão inteira: `17 / 5 = 3`, não `3.4`. Se quiser float: `(double)17 / 5`

### Relacionais (retornam 0 ou 1)
```c
a == b  // igual
a != b  // diferente
a >  b  // maior
a <  b  // menor
a >= b  // maior ou igual
a <= b  // menor ou igual
```

### Lógicos
```c
a && b  // E lógico (ambos verdadeiros)
a || b  // OU lógico (pelo menos um verdadeiro)
!a      // NÃO lógico (inverte)
```

> Em C, **0 é falso**, **qualquer outro valor é verdadeiro**.

### Bitwise (bit a bit)
```c
a & b   // AND bit a bit
a | b   // OR bit a bit
a ^ b   // XOR bit a bit
~a      // NOT bit a bit (inverte todos os bits)
a << n  // shift left (multiplica por 2^n)
a >> n  // shift right (divide por 2^n)
```

```c
// Exemplo prático: verificar se número é par
if ((n & 1) == 0)   // bit menos significativo é 0 → par
```

### Atribuição
```c
a = 5;    // atribuição simples
a += 3;   // a = a + 3
a -= 3;   // a = a - 3
a *= 2;   // a = a * 2
a /= 2;   // a = a / 2
a %= 3;   // a = a % 3
a++;      // pós-incremento
++a;      // pré-incremento
```

**→ Exercício:** [`ex03-operadores/`](ex03-operadores/main.c)

---

## 4. Controle de Fluxo

### if / else if / else
```c
if (temperatura > 35) {
    printf("Muito quente\n");
} else if (temperatura > 20) {
    printf("Agradável\n");
} else {
    printf("Frio\n");
}
```

### switch
```c
switch (opcao) {
    case 1:
        printf("Um\n");
        break;        // sem break → cai para o próximo case (fall-through)
    case 2:
        printf("Dois\n");
        break;
    default:
        printf("Outro\n");
}
```

### while
```c
int i = 0;
while (i < 10) {
    printf("%d ", i);
    i++;
}
```

### for
```c
for (int i = 0; i < 10; i++) {
    printf("%d ", i);
}
// equivalente ao while acima, mas mais compacto
```

### do-while
```c
int n;
do {
    printf("Digite um número positivo: ");
    scanf("%d", &n);
} while (n <= 0);   // garante que o corpo executa pelo menos uma vez
```

### break e continue
```c
for (int i = 0; i < 20; i++) {
    if (i % 2 == 0) continue;  // pula para próxima iteração
    if (i > 15)     break;     // sai do loop
    printf("%d ", i);           // imprime: 1 3 5 7 9 11 13 15
}
```

**→ Exercício:** [`ex04-controle-fluxo/`](ex04-controle-fluxo/main.c)

---

## 5. Funções

### Declaração e Definição

```c
// Declaração (prototype) — diz ao compilador que a função existe
int soma(int a, int b);

int main(void)
{
    int resultado = soma(3, 4);
    printf("%d\n", resultado);
    return 0;
}

// Definição — a implementação real
int soma(int a, int b)
{
    return a + b;
}
```

### Passagem por Valor

C passa argumentos **por valor** — a função recebe uma cópia:

```c
void dobra(int x)
{
    x = x * 2;  // modifica a CÓPIA, não o original
}

int main(void)
{
    int n = 5;
    dobra(n);
    printf("%d\n", n);  // imprime 5, não 10!
    return 0;
}
```

Para modificar o original, precisa de ponteiro (módulo 02).

### Recursão

Uma função que chama a si mesma. Precisa de:
1. Caso base (condição de parada)
2. Redução do problema a cada chamada

```c
int fatorial(int n)
{
    if (n <= 1)          // caso base
        return 1;
    return n * fatorial(n - 1);  // reduz o problema
}
// fatorial(5) = 5 * fatorial(4)
//             = 5 * 4 * fatorial(3)
//             = 5 * 4 * 3 * 2 * 1 = 120
```

> ⚠️ Recursão sem caso base = stack overflow (loop infinito na call stack).

**→ Exercício:** [`ex05-funcoes/`](ex05-funcoes/main.c)

---

## 6. Arrays

Array = bloco contíguo de memória com elementos do mesmo tipo.

```c
int notas[5];                      // declara array de 5 ints (não inicializado!)
int primos[5] = {2, 3, 5, 7, 11}; // declara e inicializa
int zeros[10] = {0};               // inicializa tudo com 0

primos[0];   // 2 (índices começam em 0)
primos[4];   // 11 (último elemento: tamanho - 1)
// primos[5]; → UNDEFINED BEHAVIOR (acesso fora dos limites!)
```

### Percorrer array
```c
int tamanho = 5;
for (int i = 0; i < tamanho; i++) {
    printf("primos[%d] = %d\n", i, primos[i]);
}
```

### Array 2D (matriz)
```c
int matriz[3][4];  // 3 linhas, 4 colunas

for (int i = 0; i < 3; i++)
    for (int j = 0; j < 4; j++)
        matriz[i][j] = 0;
```

> C não verifica limites de array. Acessar `arr[n]` quando `n >= tamanho` é **undefined behavior** — pode crashar, pode corromper memória, pode não fazer nada aparente. É assim que surgem vulnerabilidades de buffer overflow.

**→ Exercício:** [`ex06-arrays/`](ex06-arrays/main.c)

---

## 7. Strings

Em C, strings são **arrays de `char` terminados com `'\0'`** (null terminator).

```c
char nome[6] = {'J', 'o', 'a', 'o', '\0'};  // manual
char nome[6] = "Joao";   // o compilador adiciona '\0' automaticamente
char nome[]  = "Joao";   // tamanho inferido: 5 (4 chars + '\0')
```

```
memoria:  'J' 'o' 'a' 'o' '\0'
índices:   0    1    2    3    4
```

O `'\0'` tem valor inteiro 0. É ele que marca o fim da string.

### Funções de string.h

```c
#include <string.h>

strlen(s)             // comprimento (sem contar o '\0')
strcpy(dest, src)     // copia src para dest (dest deve ter espaço!)
strncpy(dest, src, n) // copia no máximo n chars (mais seguro)
strcmp(s1, s2)        // 0 se iguais, <0 se s1<s2, >0 se s1>s2
strcat(dest, src)     // concatena src ao final de dest
strchr(s, c)          // ponteiro para primeira ocorrência de c em s
strstr(s, sub)        // ponteiro para primeira ocorrência de sub em s
```

> ⚠️ `strcmp` não retorna bool. Strings iguais → retorna **0** (falso em C!).
> Sempre use `strcmp(s1, s2) == 0` para checar igualdade.

### Percorrer string

```c
char s[] = "hello";
int i = 0;
while (s[i] != '\0') {
    printf("%c", s[i]);
    i++;
}
```

**→ Exercício:** [`ex07-strings/`](ex07-strings/main.c)

---

## 8. Input/Output

### printf — Especificadores de Formato

| Especificador | Tipo |
|--------------|------|
| `%d` ou `%i` | int |
| `%u` | unsigned int |
| `%ld` | long |
| `%f` | float/double |
| `%.2f` | double com 2 casas decimais |
| `%c` | char |
| `%s` | string (char*) |
| `%p` | ponteiro (endereço em hex) |
| `%x` | inteiro em hexadecimal |
| `%zu` | size_t (sizeof) |
| `%%` | o caractere `%` |

```c
printf("%-10s %5d %8.2f\n", "item", 42, 3.14);
//      ^ alinha esq, 10 chars
//                   ^ alinha dir, 5 chars
//                          ^ 8 chars total, 2 decimais
```

### scanf

```c
int n;
double x;
char nome[50];

scanf("%d", &n);      // & = endereço de n
scanf("%lf", &x);     // %lf para double (não %f!)
scanf("%49s", nome);  // lê palavra; %49s evita overflow
```

> `scanf` com `%s` não lê strings com espaços. Para isso: `fgets(nome, sizeof(nome), stdin)`.

### getchar / putchar

```c
int c = getchar();    // lê um char do stdin (retorna int para detectar EOF)
putchar(c);           // escreve um char no stdout

while ((c = getchar()) != '\n')  // consome até o enter
    putchar(c);
```

**→ Exercício:** [`ex08-io/`](ex08-io/main.c)

---

## Knowledge Check

Responda sem consultar o material. Se travar, releia a seção correspondente.

1. Por que `main` retorna `int` e não `void`? O que o valor de retorno representa?
2. Qual a diferença entre `int` e `unsigned int`? Quando você usaria cada um?
3. O que acontece em `int x = 17 / 5;`? Como obter `3.4` ao invés de `3`?
4. Por que `strcmp(s1, s2) == 0` e não apenas `strcmp(s1, s2)`?
5. Qual a diferença entre pré-incremento (`++i`) e pós-incremento (`i++`) dentro de uma expressão?
6. O que é o `'\0'` e por que ele é obrigatório em strings C?
7. O que acontece se você acessar `arr[10]` num array de tamanho 10?
8. Por que a função `dobra(int x)` não modifica o valor original fora dela?
9. Qual a diferença entre `while` e `do-while`? Dê um exemplo onde `do-while` é mais natural.
10. O que `-Wall -Wextra -Werror` fazem no gcc? Por que são importantes?

---

## Projeto — Calculadora de Terminal

Implemente uma calculadora interativa em C com os seguintes requisitos:

**Funcionalidades:**
- Loop principal que lê expressões do tipo `<número> <operador> <número>` até o usuário digitar `q`
- Operadores suportados: `+`, `-`, `*`, `/`, `%`
- Divisão por zero deve exibir mensagem de erro e continuar (não crashar)
- Divisão entre inteiros mostra resultado inteiro **e** o resto separados
- Histórico: ao digitar `h`, exibe as últimas 10 operações com resultados

**Requisitos técnicos:**
- Compilar sem warnings com `gcc -Wall -Wextra -Werror`
- Cada operação em uma função separada (`soma`, `subtrai`, `multiplica`, `divide`)
- Função `exibir_historico` separada
- Sem variáveis globais — passe o histórico por parâmetro

**Exemplo de execução:**
```
calc> 17 / 5
17 / 5 = 3 (resto: 2)

calc> 3.14 * 2
Erro: use apenas inteiros.

calc> 100 % 7
100 % 7 = 2

calc> h
[1] 17 / 5 = 3
[2] 100 % 7 = 2

calc> q
Até logo.
```

---

## Recursos Adicionais

Estes recursos são **opcionais** mas vão solidificar seu entendimento:

**Para ler/assistir agora:**
- [Beej's Guide to C — cap. 1–5](https://beej.us/guide/bgc/html/split/) — referência online gratuita, linguagem acessível
- [CS50x Week 1](https://cs50.harvard.edu/x/) — Harvard, em inglês, cobre exatamente este módulo com exercícios

**Para consulta:**
- `man 3 printf` · `man 3 scanf` · `man 3 string` — documentação completa no seu terminal
- [cdecl.org](https://cdecl.org) — traduz declarações C complexas para inglês

**Para ir além:**
- K&R capítulos 1–4 — o livro original de C, conciso e denso
- [CS:APP cap. 2](https://csapp.cs.cmu.edu/) — representação de dados em nível de bits (gratuito online)

---

## Mapa de Progressão

```
ex01-hello       → primeiro programa, compilar, exit code
ex02-tipos       → sizeof, limites, casting, printf com formatos
ex03-operadores  → aritméticos, bitwise, precedência
ex04-controle    → if/for/while/switch, FizzBuzz, sequências
ex05-funcoes     → modularização, recursão (fib, fatorial)
ex06-arrays      → ordenação básica, pesquisa linear
ex07-strings     → manipulação sem string.h, depois com
ex08-io          → leitura de input, validação
projeto          → calculadora interativa com histórico
```
