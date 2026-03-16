# 03 — Estruturas de Dados em C

> *"Algorithms + Data Structures = Programs"* — Niklaus Wirth

Estruturas de dados são a base de todo software eficiente. Em C, você as implementa do zero — sem classes, sem generics. Isso força você a entender como cada estrutura funciona internamente.

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

## Exercícios

**→ [`ex01-linked-list/`](ex01-linked-list/main.c)** — lista com push, pop, insert, delete, reverse
**→ [`ex02-stack/`](ex02-stack/main.c)** — stack com array; validar parênteses balanceados
**→ [`ex03-queue/`](ex03-queue/main.c)** — queue circular; simular fila de banco
**→ [`ex04-hash-table/`](ex04-hash-table/main.c)** — hash table com chaining; contar frequência de palavras

---

## Referências

- **K&R** capítulo 6 (Structures)
- **Algorithms** — Sedgewick & Wayne (capítulos 1-3)
- CS50x semanas 5 e além
- **Visualgo** — visualgo.net (animações de estruturas de dados)
