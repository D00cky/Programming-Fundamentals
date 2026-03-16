# 02 — Ponteiros & Memória

> *"C makes it easy to shoot yourself in the foot; C++ makes it harder, but when you do it blows your whole leg off."* — Bjarne Stroustrup

Esse módulo é onde a maioria das pessoas trava. Mas ponteiros não são difíceis — são só endereços. Um ponteiro é um número inteiro que representa uma posição na memória.

---

## 1. O Modelo de Memória

```
Endereço   Conteúdo
0x7fff0010  [  42  ]  ← variável int x = 42
0x7fff0014  [0x10 ]   ← variável int *p = &x (armazena o endereço de x)
0x7fff0018  [  0  ]
...
```

Toda variável tem:
- Um **valor** (o conteúdo)
- Um **endereço** (onde está na memória)

---

## 2. Sintaxe Básica

```c
int  x = 42;    // variável int, valor 42
int *p = &x;    // ponteiro para int; & = "endereço de"

// Leitura:
printf("%d\n",  x);   // 42         — valor de x
printf("%p\n",  p);   // 0x7fff...  — endereço que p guarda
printf("%d\n", *p);   // 42         — *p = "dereference": valor no endereço p

// Escrita:
*p = 99;        // modifica o que está no endereço p → x agora vale 99
printf("%d\n", x);  // 99
```

### Declaração: `*` está no tipo, não no nome

```c
int *p;     // p é um ponteiro para int
int* p;     // mesma coisa — prefira a primeira para evitar confusão:
int* a, b;  // ERRO CONCEITUAL: a é int*, mas b é int (não int*)!
int *a, *b; // correto: ambos são int*
```

---

## 3. Passagem por Referência

O C passa argumentos por valor. Ponteiros permitem modificar variáveis do chamador:

```c
void troca(int *a, int *b)   // recebe ENDEREÇOS
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int x = 10, y = 20;
troca(&x, &y);               // passa ENDEREÇOS
printf("%d %d\n", x, y);     // 20 10
```

---

## 4. Ponteiros e Arrays

Arrays e ponteiros são intimamente ligados. O nome de um array **decai** para ponteiro para o primeiro elemento:

```c
int arr[5] = {10, 20, 30, 40, 50};
int *p = arr;         // equivale a: int *p = &arr[0]

arr[2]  == *(arr + 2)  // true: notação de índice = aritmética de ponteiro
p[2]    == *(p + 2)    // mesma coisa

// Percorrer com ponteiro:
while (p < arr + 5) {
    printf("%d ", *p);
    p++;
}
```

### Diferença importante

```c
int arr[5];
int *p = arr;

sizeof(arr)  // 20 bytes (5 × 4)
sizeof(p)    // 8 bytes (tamanho do ponteiro na máquina 64-bit)
```

---

## 5. Strings como `char *`

```c
char str[] = "hello";   // array na stack, mutável
char *s    = "hello";   // ponteiro para literal — NÃO MODIFICAR (UB)

// Correto para string mutável:
char buf[6];
strcpy(buf, "hello");   // copia para buffer nosso
buf[0] = 'H';           // OK

// Iterar com ponteiro:
char *p = str;
while (*p)              // *p != '\0'
    putchar(*p++);      // imprime char e avança ponteiro
```

---

## 6. Stack vs Heap

### Stack (automática)
- Alocada/liberada automaticamente ao entrar/sair de funções
- Limitada (~8MB típico)
- Rápida

```c
void func(void) {
    int arr[1000];    // 4000 bytes na stack — OK
    // arr destruído quando func retorna
}
```

### Heap (dinâmica)
- Controlada por você: `malloc`/`free`
- Só limitada pela RAM disponível
- Persiste até você liberar

```c
#include <stdlib.h>

int *arr = malloc(1000 * sizeof(int));  // aloca 4000 bytes no heap
if (!arr)                                // SEMPRE verificar NULL!
    return (1);                          // malloc pode falhar

arr[0] = 42;                            // usa normalmente
free(arr);                              // LIBERAR quando acabar
arr = NULL;                             // boa prática: evita use-after-free
```

### Funções de memória

```c
malloc(n)           // aloca n bytes sem inicializar (conteúdo lixo)
calloc(count, size) // aloca count×size bytes, inicializa com ZERO
realloc(ptr, n)     // redimensiona bloco existente
free(ptr)           // libera bloco alocado com malloc/calloc/realloc
```

---

## 7. Memory Bugs Clássicos

### Memory Leak (vazamento)
```c
void leak(void) {
    int *p = malloc(100);
    // ... usa p ...
    return;   // esqueceu free(p) → memória perdida até o programa terminar
}
```

### Double Free
```c
free(p);
free(p);   // undefined behavior — pode corromper o heap
```

### Use After Free
```c
free(p);
*p = 42;   // undefined behavior
```

### Buffer Overflow
```c
int arr[5];
arr[10] = 99;  // escreve fora do array → corrompimento de memória
```

### Dangling Pointer (ponteiro pendente)
```c
int *p = malloc(4);
free(p);
// p ainda aponta para o endereço, mas a memória foi liberada
// p é agora um "dangling pointer" — não usar!
```

---

## 8. Valgrind

Detecta memory bugs em runtime:

```bash
# Compilar com debug info:
gcc -Wall -Wextra -Werror -g main.c -o programa

# Rodar com Valgrind:
valgrind --leak-check=full --show-leak-kinds=all ./programa

# Output típico de leak:
# LEAK SUMMARY:
#    definitely lost: 40 bytes in 1 blocks
```

---

## 9. Ponteiros para Ponteiros

```c
int  x   = 42;
int  *p  = &x;    // ponteiro para int
int **pp = &p;    // ponteiro para ponteiro para int

**pp = 99;         // modifica x através de dois níveis de indireção
```

Uso comum: funções que precisam modificar um ponteiro:
```c
void aloca(int **out, int n) {
    *out = malloc(n * sizeof(int));
}

int *arr;
aloca(&arr, 10);   // passa endereço do ponteiro
```

---

## Exercícios

**→ [`ex01-ponteiros-basico/`](ex01-ponteiros-basico/main.c)** — `&`, `*`, passagem por referência
**→ [`ex02-aritmetica/`](ex02-aritmetica/main.c)** — aritmética de ponteiro, arrays com ponteiro
**→ [`ex03-malloc-free/`](ex03-malloc-free/main.c)** — alocação dinâmica, Valgrind
**→ [`ex04-strings-dinamicas/`](ex04-strings-dinamicas/main.c)** — strings no heap, `strdup`
**→ [`ex05-ponteiro-funcao/`](ex05-ponteiro-funcao/main.c)** — ponteiros para funções, callbacks

---

## Referências

- **K&R** capítulo 5 (Pointers and Arrays) — leitura obrigatória
- **Beej's Guide** capítulo 5 (Pointers)
- `man 3 malloc` · `man 3 free` · `man 3 memcpy`
- **Valgrind Quick Start** — valgrind.org/docs/manual/quick-start.html
- CS50x semana 4 (Memory)
