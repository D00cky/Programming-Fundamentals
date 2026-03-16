# 04 — Algoritmos de Ordenação & Busca

> *"An algorithm must be seen to be believed."* — Donald Knuth

Ordenação é o problema mais estudado em CS. Cada algoritmo revela um trade-off diferente entre tempo, espaço e simplicidade. Entender todos eles é entender como pensar em performance.

---

## Análise de Complexidade

**Big-O** descreve como o tempo cresce com o input:

| Notação | Nome | n=100 | n=1.000 | n=1.000.000 |
|---------|------|-------|---------|-------------|
| O(1) | Constante | 1 | 1 | 1 |
| O(log n) | Logarítmico | 7 | 10 | 20 |
| O(n) | Linear | 100 | 1.000 | 1.000.000 |
| O(n log n) | Linearítmico | 665 | 10.000 | 20.000.000 |
| O(n²) | Quadrático | 10.000 | 1.000.000 | 10¹² |
| O(2ⁿ) | Exponencial | 10³⁰ | ∞ | ∞ |

---

## Algoritmos O(n²) — Simples, Lentos

### Bubble Sort
Compara pares adjacentes e troca. Cada passagem "borbulha" o maior para o final.

```c
void bubble_sort(int *arr, int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - 1 - i; j++)
            if (arr[j] > arr[j + 1])
                swap(&arr[j], &arr[j + 1]);
}
// Melhor caso (já ordenado): O(n) com flag de otimização
// Pior caso: O(n²)
```

### Selection Sort
A cada passagem, encontra o mínimo e coloca na posição correta.

```c
void selection_sort(int *arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++)
            if (arr[j] < arr[min_idx]) min_idx = j;
        swap(&arr[i], &arr[min_idx]);
    }
}
// Sempre O(n²): nunca melhora mesmo com array ordenado
// Vantagem: minimiza o número de swaps (exato n-1 swaps)
```

### Insertion Sort
Constrói a porção ordenada inserindo um elemento de cada vez.

```c
void insertion_sort(int *arr, int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}
// Melhor caso (quase ordenado): O(n) — muito bom para arrays pequenos
// Estável, in-place, usado na prática para n < 10-20
```

---

## Algoritmos O(n log n) — Eficientes

### Merge Sort
Divide ao meio recursivamente, depois merge as metades ordenadas.

```c
void merge(int *arr, int l, int m, int r) {
    int n1 = m - l + 1, n2 = r - m;
    int L[n1], R[n2];
    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2)
        arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void merge_sort(int *arr, int l, int r) {
    if (l >= r) return;
    int m = l + (r - l) / 2;
    merge_sort(arr, l, m);
    merge_sort(arr, m + 1, r);
    merge(arr, l, m, r);
}
// Sempre O(n log n) — garantido no pior caso
// Estável · Espaço: O(n) extra
```

### Quick Sort
Escolhe um pivô, particiona o array: menores à esquerda, maiores à direita. Recursão nas duas partes.

```c
int partition(int *arr, int lo, int hi) {
    int pivot = arr[hi];
    int i = lo - 1;
    for (int j = lo; j < hi; j++)
        if (arr[j] <= pivot) { i++; swap(&arr[i], &arr[j]); }
    swap(&arr[i + 1], &arr[hi]);
    return i + 1;
}

void quick_sort(int *arr, int lo, int hi) {
    if (lo >= hi) return;
    int p = partition(arr, lo, hi);
    quick_sort(arr, lo, p - 1);
    quick_sort(arr, p + 1, hi);
}
// Médio: O(n log n) · Pior caso (pivô ruim): O(n²)
// Não estável · In-place · Na prática, mais rápido que merge sort
```

### Heap Sort
Usa um heap para ordenar in-place.

```c
void heapify(int *arr, int n, int i) {
    int largest = i, l = 2*i+1, r = 2*i+2;
    if (l < n && arr[l] > arr[largest]) largest = l;
    if (r < n && arr[r] > arr[largest]) largest = r;
    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapify(arr, n, largest);
    }
}

void heap_sort(int *arr, int n) {
    for (int i = n/2 - 1; i >= 0; i--) heapify(arr, n, i);
    for (int i = n - 1; i > 0; i--) {
        swap(&arr[0], &arr[i]);
        heapify(arr, i, 0);
    }
}
// Sempre O(n log n) · In-place · Não estável
// Pior que quick sort na prática (cache unfriendly)
```

---

## Busca

### Busca Linear — O(n)
```c
int busca_linear(int *arr, int n, int alvo) {
    for (int i = 0; i < n; i++)
        if (arr[i] == alvo) return i;
    return -1;
}
```

### Busca Binária — O(log n)
**Requer array ordenado.** A cada iteração elimina metade dos candidatos.

```c
int busca_binaria(int *arr, int n, int alvo) {
    int lo = 0, hi = n - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;   // evita overflow vs (lo+hi)/2
        if (arr[mid] == alvo) return mid;
        if (arr[mid] < alvo)  lo = mid + 1;
        else                  hi = mid - 1;
    }
    return -1;
}
```

---

## Quando Usar Qual?

| Situação | Recomendação |
|----------|-------------|
| n < 20 | Insertion Sort |
| Dados quase ordenados | Insertion Sort |
| Garantia de O(n log n) | Merge Sort |
| Mais rápido na prática | Quick Sort (com pivot aleatório) |
| Sem memória extra | Heap Sort |
| Array ordenado, buscar | Busca Binária |
| Uso geral em C | `qsort()` da stdlib |

---

## qsort da stdlib

```c
#include <stdlib.h>

int cmp_int(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);   // crescente
}

qsort(arr, n, sizeof(int), cmp_int);
```

---

## Exercícios

**→ [`ex01-basicos/`](ex01-basicos/main.c)** — bubble, selection, insertion; medir iterações
**→ [`ex02-merge-quick/`](ex02-merge-quick/main.c)** — merge sort e quick sort completos
**→ [`ex03-busca-binaria/`](ex03-busca-binaria/main.c)** — busca binária iterativa e recursiva
**→ [`ex04-benchmark/`](ex04-benchmark/main.c)** — comparar algoritmos com clock(); gerar gráfico de dados

---

## Referências

- **Introduction to Algorithms (CLRS)** — capítulos 2, 6, 7, 8
- **Algorithms** — Sedgewick & Wayne
- **Sorting Algorithms Animations** — toptal.com/developers/sorting-algorithms
- **Visualgo** — visualgo.net/en/sorting
