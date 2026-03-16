# 06 — Algoritmos Avançados

> *"Recursion is the root of computation since it trades description for time."* — Alan Perlis

---

## 1. Recursão Avançada

### Backtracking

Explora todas as possibilidades, "voltando atrás" quando um caminho não leva à solução.

**Template:**
```c
void backtrack(Estado *estado, int nivel) {
    if (solucao_completa(estado)) {
        registra_solucao(estado);
        return;
    }
    for cada opcao disponivel:
        if (opcao_valida(opcao, estado)):
            aplica(opcao, estado);
            backtrack(estado, nivel + 1);
            desfaz(opcao, estado);    // ← o "volta atrás"
}
```

**Exemplo: N-Rainhas**
```c
// Coloca N rainhas num tabuleiro N×N sem se atacarem
int col[N], diag1[2*N], diag2[2*N];  // quais colunas/diagonais estão ocupadas

void solve(int row, int n, int *solucoes) {
    if (row == n) { (*solucoes)++; return; }
    for (int c = 0; c < n; c++) {
        if (!col[c] && !diag1[row - c + n] && !diag2[row + c]) {
            col[c] = diag1[row-c+n] = diag2[row+c] = 1;
            solve(row + 1, n, solucoes);
            col[c] = diag1[row-c+n] = diag2[row+c] = 0;
        }
    }
}
```

### Divide & Conquer

Divide o problema em subproblemas menores, resolve recursivamente, combina os resultados.

```c
// Máximo de um array via D&C
int maximo(int *arr, int l, int r) {
    if (l == r) return arr[l];
    int m = (l + r) / 2;
    int esq = maximo(arr, l, m);
    int dir = maximo(arr, m + 1, r);
    return esq > dir ? esq : dir;
}
// O(n) — mas demonstra o padrão; merge sort usa o mesmo esquema
```

---

## 2. Grafos

Um grafo G = (V, E): conjunto de vértices V e arestas E.

### Representações

**Lista de adjacência** (preferida para grafos esparsos):
```c
#define MAX_V 100

typedef struct s_adj {
    int           dest;
    struct s_adj *next;
} t_adj;

t_adj *grafo[MAX_V];  // grafo[v] = lista de vizinhos de v
```

**Matriz de adjacência** (preferida para grafos densos):
```c
int adj[MAX_V][MAX_V];  // adj[u][v] = 1 se existe aresta u→v
```

### BFS (Breadth-First Search) — O(V + E)

Percorre em largura, usando fila. Encontra **menor caminho** em grafos não-ponderados.

```c
void bfs(int inicio, int n) {
    int visitado[MAX_V] = {0};
    int dist[MAX_V];
    Queue q;
    memset(dist, -1, sizeof(dist));

    visitado[inicio] = 1;
    dist[inicio] = 0;
    enqueue(&q, inicio);

    while (!is_empty(&q)) {
        int v = dequeue(&q);
        for (t_adj *e = grafo[v]; e; e = e->next) {
            if (!visitado[e->dest]) {
                visitado[e->dest] = 1;
                dist[e->dest] = dist[v] + 1;
                enqueue(&q, e->dest);
            }
        }
    }
}
```

### DFS (Depth-First Search) — O(V + E)

Percorre em profundidade, usando pilha (ou recursão).

```c
int visitado[MAX_V];

void dfs(int v) {
    visitado[v] = 1;
    printf("%d ", v);
    for (t_adj *e = grafo[v]; e; e = e->next)
        if (!visitado[e->dest])
            dfs(e->dest);
}
```

**Usos:** detectar ciclos, ordenação topológica, componentes conexos.

---

## 3. String Matching

### Algoritmo Ingênuo — O(n·m)

```c
int busca_naive(char *texto, char *padrao) {
    int n = strlen(texto), m = strlen(padrao);
    for (int i = 0; i <= n - m; i++) {
        int j = 0;
        while (j < m && texto[i+j] == padrao[j]) j++;
        if (j == m) return i;  // encontrou na posição i
    }
    return -1;
}
```

### KMP (Knuth-Morris-Pratt) — O(n + m)

Pré-computa um array de "failure function" que evita comparações redundantes.

```c
// Constrói a tabela de falha
void compute_fail(char *pat, int m, int *fail) {
    fail[0] = 0;
    int k = 0;
    for (int i = 1; i < m; i++) {
        while (k > 0 && pat[k] != pat[i]) k = fail[k-1];
        if (pat[k] == pat[i]) k++;
        fail[i] = k;
    }
}

int kmp(char *texto, char *padrao) {
    int n = strlen(texto), m = strlen(padrao);
    int fail[m];
    compute_fail(padrao, m, fail);
    int k = 0;
    for (int i = 0; i < n; i++) {
        while (k > 0 && padrao[k] != texto[i]) k = fail[k-1];
        if (padrao[k] == texto[i]) k++;
        if (k == m) return i - m + 1;  // encontrou
    }
    return -1;
}
```

---

## 4. Programação Dinâmica (Introdução)

DP = memoização de subproblemas sobrepostos.

**Fibonacci com memoização:**
```c
int memo[1000] = {0};

int fib_dp(int n) {
    if (n <= 1) return n;
    if (memo[n]) return memo[n];
    memo[n] = fib_dp(n-1) + fib_dp(n-2);
    return memo[n];
}
// O(n) em vez de O(2^n)
```

**Maior subsequência comum (LCS):**
```c
int lcs(char *a, char *b, int m, int n) {
    int dp[m+1][n+1];
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            if (!i || !j) dp[i][j] = 0;
            else if (a[i-1] == b[j-1]) dp[i][j] = 1 + dp[i-1][j-1];
            else dp[i][j] = (dp[i-1][j] > dp[i][j-1]) ? dp[i-1][j] : dp[i][j-1];
        }
    }
    return dp[m][n];
}
```

---

## Exercícios

**→ [`ex01-backtracking/`](ex01-backtracking/main.c)** — N-Rainhas, labirinto, subconjuntos
**→ [`ex02-grafos-bfs-dfs/`](ex02-grafos-bfs-dfs/main.c)** — BFS, DFS, menor caminho
**→ [`ex03-string-matching/`](ex03-string-matching/main.c)** — naive, KMP
**→ [`ex04-dp-intro/`](ex04-dp-intro/main.c)** — fibonacci DP, coin change, mochila 0/1

---

## Referências

- **CLRS** — capítulos 15 (DP), 22 (Graph algorithms), 32 (String matching)
- **The Algorithm Design Manual** — Steven Skiena
- **Competitive Programmer's Handbook** — cses.fi/book/book.pdf (gratuito)
- **Visualgo** — visualgo.net
