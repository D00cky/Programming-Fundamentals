# 05 — Árvores

> *"In a tree, every node is the root of a subtree."*

Árvores são estruturas hierárquicas. A maioria dos sistemas de arquivos, bancos de dados, compiladores e sistemas de roteamento usa alguma variante de árvore.

---

## 1. Terminologia

```
         [10]          ← raiz (root)
        /    \
      [5]    [15]      ← filhos de 10
     /   \      \
   [2]   [7]   [20]   ← folhas (leaf) — sem filhos
```

- **Nó (node):** elemento da árvore
- **Raiz (root):** nó sem pai
- **Folha (leaf):** nó sem filhos
- **Altura:** maior distância da raiz a uma folha
- **Profundidade de um nó:** distância até a raiz
- **Grau:** número de filhos de um nó

---

## 2. Árvore Binária de Busca (BST)

**Propriedade:** para todo nó N:
- Todos os nós à esquerda < N
- Todos os nós à direita > N

```c
typedef struct s_bst {
    int             val;
    struct s_bst   *left;
    struct s_bst   *right;
} t_bst;

// Inserção — O(h), h = altura
t_bst *bst_insert(t_bst *root, int val) {
    if (!root) {
        t_bst *n = malloc(sizeof(t_bst));
        n->val = val; n->left = n->right = NULL;
        return n;
    }
    if (val < root->val) root->left  = bst_insert(root->left,  val);
    else if (val > root->val) root->right = bst_insert(root->right, val);
    return root;
}

// Busca — O(h)
t_bst *bst_search(t_bst *root, int val) {
    if (!root || root->val == val) return root;
    if (val < root->val) return bst_search(root->left, val);
    return bst_search(root->right, val);
}
```

### Traversals (percursos)

```c
// In-order (esquerda → raiz → direita): imprime em ordem CRESCENTE
void inorder(t_bst *root) {
    if (!root) return;
    inorder(root->left);
    printf("%d ", root->val);
    inorder(root->right);
}

// Pre-order (raiz → esquerda → direita): serialização, cópia
void preorder(t_bst *root) {
    if (!root) return;
    printf("%d ", root->val);
    preorder(root->left);
    preorder(root->right);
}

// Post-order (esquerda → direita → raiz): liberação de memória
void postorder(t_bst *root) {
    if (!root) return;
    postorder(root->left);
    postorder(root->right);
    printf("%d ", root->val);   // ou: free(root)
}
```

### Complexidade BST

| Operação | Médio | Pior caso (degenerada) |
|----------|-------|----------------------|
| Busca | O(log n) | O(n) |
| Inserção | O(log n) | O(n) |
| Remoção | O(log n) | O(n) |

A BST degenera em lista ligada quando inserimos elementos já ordenados.

---

## 3. BST Balanceada — AVL

**AVL** garante que a diferença de altura entre subárvores (fator de balanceamento) seja no máximo 1. Isso mantém h = O(log n) sempre.

```c
// Fator de balanceamento: altura(esq) - altura(dir)
// AVL mantém: fator ∈ {-1, 0, 1}

// Rotação à direita (para balancear quando esq > dir)
t_bst *rotate_right(t_bst *y) {
    t_bst *x = y->left;
    t_bst *T = x->right;
    x->right = y;
    y->left  = T;
    // atualizar alturas...
    return x;
}
```

Operações garantidas O(log n) no pior caso.

---

## 4. Heap Binário

Árvore binária completa onde cada nó é ≥ (max-heap) ou ≤ (min-heap) que seus filhos.

**Implementada como array** (sem ponteiros!):
```
Índice:   0   1   2   3   4   5   6
          [9] [7] [8] [3] [4] [5] [6]

pai(i)    = (i - 1) / 2
filho_esq = 2*i + 1
filho_dir = 2*i + 2
```

```c
// Sobe o elemento no índice i (após inserção)
void sift_up(int *heap, int i) {
    while (i > 0) {
        int pai = (i - 1) / 2;
        if (heap[i] > heap[pai]) {
            swap(&heap[i], &heap[pai]);
            i = pai;
        } else break;
    }
}

// Desce o elemento no índice i (após remoção do max)
void sift_down(int *heap, int n, int i) {
    while (2*i + 1 < n) {
        int maior = i, l = 2*i+1, r = 2*i+2;
        if (l < n && heap[l] > heap[maior]) maior = l;
        if (r < n && heap[r] > heap[maior]) maior = r;
        if (maior == i) break;
        swap(&heap[i], &heap[maior]);
        i = maior;
    }
}
```

**Usos:** priority queue, heap sort, algoritmos de menor caminho.

---

## 5. BFS na Árvore

Breadth-First Search percorre nível por nível usando uma fila:

```c
void bfs(t_bst *root) {
    if (!root) return;
    Queue q;
    enqueue(&q, root);
    while (!is_empty(&q)) {
        t_bst *node = dequeue(&q);
        printf("%d ", node->val);
        if (node->left)  enqueue(&q, node->left);
        if (node->right) enqueue(&q, node->right);
    }
}
```

---

## Complexidade Resumida

| Estrutura | Busca | Inserção | Remoção |
|-----------|-------|----------|---------|
| BST (médio) | O(log n) | O(log n) | O(log n) |
| BST (pior) | O(n) | O(n) | O(n) |
| AVL | O(log n) | O(log n) | O(log n) |
| Heap (max/min) | O(n) | O(log n) | O(log n) |
| Heap (get-max) | O(1) | — | — |

---

## Exercícios

**→ [`ex01-bst/`](ex01-bst/main.c)** — insert, search, delete, todos os traversals
**→ [`ex02-bst-propriedades/`](ex02-bst-propriedades/main.c)** — altura, contar nós, verificar se é BST válida
**→ [`ex03-heap/`](ex03-heap/main.c)** — min-heap como array; insert, extract-min

---

## Referências

- **CLRS** capítulos 12 (BST), 13 (Red-Black), 6 (Heapsort)
- **Visualgo** — visualgo.net/en/bst
- **CS50 Data Structures** — youtube.com
