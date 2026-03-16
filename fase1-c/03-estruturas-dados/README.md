# 03 — Estruturas de Dados em C

> *"Algorithms + Data Structures = Programs"* — Niklaus Wirth

Estruturas de dados são a base de todo software eficiente. Em C, você as implementa do zero — sem classes, sem generics. Isso força você a entender como cada estrutura funciona internamente.

---

## Antes de começar

Certifique-se de que você já:

- [ ] Sabe usar `malloc`, `free` e Valgrind (`fase1-c/02-ponteiros-memoria/`)
- [ ] Entende a sintaxe de `struct` e acesso com `->` e `.`
- [ ] Usa ponteiros para ponteiros (`**`) para modificar ponteiros em funções
- [ ] Conhece aritmética de ponteiros e indexação de arrays

---

## O que você vai aprender

Ao final deste módulo você será capaz de:

- Implementar lista ligada simples e duplamente ligada do zero em C
- Construir pilha (LIFO) e fila (FIFO) usando array ou lista ligada
- Implementar hash table com resolução de colisões por chaining
- Analisar a complexidade de cada operação e escolher a estrutura certa para cada problema
- Explicar o trade-off entre acesso O(1) de arrays e inserção O(1) de listas ligadas

---

## 1. struct

`struct` agrupa variáveis de tipos diferentes sob um nome:

```c
typedef struct s_node
{
    int             data;
    struct s_node   *next;   // auto-referência: ponteiro para o próprio tipo
} t_node;

typedef struct s_list
{
    t_node  *head;
    int      size;
} t_list;
```

```c
// Criar nó no heap:
t_node *novo = malloc(sizeof(t_node));
novo->data = 42;    // -> desreferencia e acessa: equivale a (*novo).data
novo->next = NULL;
```

---

## 2. Lista Ligada (Linked List)

Cada nó armazena um dado e um ponteiro para o próximo.

```
head → [10|→] → [20|→] → [30|NULL]
```

### Operações

```c
// Adicionar no início: O(1)
void push_front(t_list *lst, int val) {
    t_node *n = malloc(sizeof(t_node));
    n->data = val;
    n->next = lst->head;
    lst->head = n;
    lst->size++;
}

// Adicionar no fim: O(n) sem tail pointer
void push_back(t_list *lst, int val) {
    t_node *n = malloc(sizeof(t_node));
    n->data = val;
    n->next = NULL;
    if (!lst->head) { lst->head = n; }
    else {
        t_node *cur = lst->head;
        while (cur->next) cur = cur->next;
        cur->next = n;
    }
    lst->size++;
}

// Remover do início: O(1)
int pop_front(t_list *lst) {
    t_node *tmp = lst->head;
    int val = tmp->data;
    lst->head = tmp->next;
    free(tmp);
    lst->size--;
    return val;
}

// Liberar toda a lista
void list_clear(t_list *lst) {
    t_node *cur = lst->head;
    while (cur) {
        t_node *next = cur->next;
        free(cur);
        cur = next;
    }
    lst->head = NULL;
    lst->size = 0;
}
```

### Complexidades

| Operação | Com tail pointer | Sem tail pointer |
|----------|-----------------|-----------------|
| push_front | O(1) | O(1) |
| push_back | O(1) | O(n) |
| pop_front | O(1) | O(1) |
| Busca | O(n) | O(n) |
| Acesso por índice | O(n) | O(n) |

---

## 3. Pilha (Stack) — LIFO

Last In, First Out. Implementada sobre lista ligada ou array.

```c
typedef struct {
    int *data;
    int  top;
    int  cap;
} t_stack;

void push(t_stack *s, int val) { s->data[++s->top] = val; }
int  pop(t_stack *s)           { return s->data[s->top--]; }
int  peek(t_stack *s)          { return s->data[s->top]; }
int  is_empty(t_stack *s)      { return s->top == -1; }
```

**Usos:** chamadas de função, undo/redo, parsing de expressões, DFS iterativo.

---

## 4. Fila (Queue) — FIFO

First In, First Out. Com array circular evita mover elementos:

```c
typedef struct {
    int *data;
    int  head, tail, size, cap;
} t_queue;

void enqueue(t_queue *q, int val) {
    q->data[q->tail] = val;
    q->tail = (q->tail + 1) % q->cap;  // circular
    q->size++;
}

int dequeue(t_queue *q) {
    int val = q->data[q->head];
    q->head = (q->head + 1) % q->cap;
    q->size--;
    return val;
}
```

**Usos:** BFS, scheduling, buffers de I/O.

---

## 5. Hash Table

Mapeia chaves para valores em O(1) amortizado. Resolve colisões com **chaining** (lista ligada por bucket):

```c
#define BUCKETS 64

typedef struct s_entry {
    char            *key;
    int              val;
    struct s_entry  *next;   // chain para colisões
} t_entry;

typedef struct {
    t_entry *buckets[BUCKETS];
} t_htable;

// Função hash simples (djb2):
unsigned int hash(const char *key) {
    unsigned int h = 5381;
    while (*key) h = h * 33 ^ *key++;
    return h % BUCKETS;
}

void ht_set(t_htable *ht, const char *key, int val) {
    unsigned int idx = hash(key);
    t_entry *e = malloc(sizeof(t_entry));
    e->key = strdup(key);
    e->val = val;
    e->next = ht->buckets[idx];
    ht->buckets[idx] = e;
}

int ht_get(t_htable *ht, const char *key) {
    unsigned int idx = hash(key);
    t_entry *e = ht->buckets[idx];
    while (e) {
        if (strcmp(e->key, key) == 0) return e->val;
        e = e->next;
    }
    return -1;  // não encontrado
}
```

### Complexidade

| Operação | Médio | Pior caso |
|----------|-------|-----------|
| insert | O(1) | O(n) (muitas colisões) |
| lookup | O(1) | O(n) |
| delete | O(1) | O(n) |

---

## 6. Lista Duplamente Ligada

Cada nó tem ponteiro para o anterior também:

```c
typedef struct s_dnode {
    int              data;
    struct s_dnode  *prev;
    struct s_dnode  *next;
} t_dnode;
```

Permite remoção O(1) dado o nó, e iteração nos dois sentidos.

---

## Complexidade Comparada

| Estrutura | Acesso | Busca | Inserção | Remoção |
|-----------|--------|-------|----------|---------|
| Array | O(1) | O(n) | O(n) | O(n) |
| Lista ligada | O(n) | O(n) | O(1)* | O(1)* |
| Stack (topo) | O(1) | — | O(1) | O(1) |
| Queue (frente/fim) | O(1) | — | O(1) | O(1) |
| Hash table | O(1)† | O(1)† | O(1)† | O(1)† |

\* dado o nó/posição · † amortizado, caso médio

---

## Knowledge Check

Responda sem consultar o material. Se travar, releia a seção correspondente.

1. Qual a principal desvantagem de uma lista ligada em relação a um array? E a principal vantagem?
2. O que significa LIFO e FIFO? Dê um exemplo real de cada.
3. O que é "chaining" e por que é necessário em hash tables?
4. Por que a complexidade média de lookup em hash table é O(1) mas o pior caso é O(n)?
5. Como você definiria um `struct` que contém um ponteiro para si mesmo? Para que serve isso?
6. Qual a diferença entre `typedef struct s_node t_node` e apenas `struct s_node`?
7. Quando usar lista duplamente ligada em vez de simplesmente ligada?
8. Em que situação uma fila circular com array é mais eficiente que realocação?

---

## Projeto — Agenda de Contatos

Implemente uma agenda de contatos em linha de comando usando lista ligada.

**Funcionalidades:**
- Adicionar contato (nome, telefone, email)
- Remover contato por nome
- Buscar contato por nome ou parte do nome
- Listar todos os contatos em ordem alfabética
- Salvar e carregar agenda em arquivo de texto (formato `nome|telefone|email`)

**Requisitos técnicos:**
- Contatos armazenados em lista ligada alocada no heap
- Inserção mantém ordem alfabética (lista sempre ordenada)
- Compilar com `gcc -Wall -Wextra -Werror`
- Zero memory leaks ao sair do programa

**Exemplo de execução:**
```
agenda> add Ana 11999999999 ana@email.com
Contato adicionado: Ana

agenda> add Bruno 11988888888 bruno@email.com
Contato adicionado: Bruno

agenda> list
[1] Ana       | 11999999999 | ana@email.com
[2] Bruno     | 11988888888 | bruno@email.com

agenda> search ana
Ana | 11999999999 | ana@email.com

agenda> remove Ana
Contato removido: Ana

agenda> quit
Agenda salva. Até logo.
```

---

## Exercícios

**→ [`ex01-linked-list/`](ex01-linked-list/main.c)** — lista com push, pop, insert, delete, reverse
**→ [`ex02-stack/`](ex02-stack/main.c)** — stack com array; validar parênteses balanceados
**→ [`ex03-queue/`](ex03-queue/main.c)** — queue circular; simular fila de banco
**→ [`ex04-hash-table/`](ex04-hash-table/main.c)** — hash table com chaining; contar frequência de palavras

---

## Recursos Adicionais

Estes recursos são **opcionais** mas vão solidificar seu entendimento:

**Para ler/assistir agora:**
- **K&R** capítulo 6 (Structures) — leitura recomendada antes de avançar
- [Visualgo](https://visualgo.net) — animações interativas de todas as estruturas deste módulo

**Para consulta:**
- **Algorithms** — Sedgewick & Wayne, capítulos 1-3 (implementações em C-like)
- CS50x semanas 5+ — estruturas de dados com animações visuais

**Para ir além:**
- **CLRS** capítulos 10-11 (Elementary Data Structures, Hash Tables) — análise formal
- [Hash Tables: Theory and Practice](https://en.wikipedia.org/wiki/Hash_table) — análise de funções hash e estratégias de colisão
