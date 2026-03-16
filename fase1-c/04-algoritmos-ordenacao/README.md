# 04 — Algoritmos de Ordenação & Busca

> *"An algorithm must be seen to be believed."* — Donald Knuth

Ordenação é o problema mais estudado em CS. Cada algoritmo revela um trade-off diferente entre tempo, espaço e simplicidade. Entender todos eles é entender como pensar em performance.

---

## Antes de começar

Certifique-se de que você já:

- [ ] Manipula arrays e escreve funções que recebem arrays como parâmetros
- [ ] Entende ponteiros básicos e passagem de endereços (`fase1-c/02-ponteiros-memoria/`)
- [ ] Conhece a notação Big-O em nível conceitual (O(n), O(n²), O(log n))

---

## O que você vai aprender

Ao final deste módulo você será capaz de:

- Implementar do zero: Bubble Sort, Selection Sort, Insertion Sort, Merge Sort, Quick Sort e Heap Sort
- Analisar a complexidade de tempo (melhor, médio e pior caso) de cada algoritmo
- Escolher o algoritmo certo para cada situação real
- Explicar o que é um algoritmo de ordenação estável e por que importa
- Usar `qsort()` da stdlib com função comparadora customizada
- Medir o tempo de execução com `clock()` e comparar algoritmos empiricamente

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

## Knowledge Check

Responda sem consultar o material. Se travar, releia a seção correspondente.

1. Qual a diferença prática entre um algoritmo O(n²) e um O(n log n) para n = 1.000.000?
2. O que significa dizer que um algoritmo de ordenação é "estável"? Por que isso importa?
3. Por que Insertion Sort tem complexidade O(n) no melhor caso?
4. Em que condição Quick Sort degenera para O(n²)? Como evitar?
5. Qual a desvantagem de Merge Sort em relação a Quick Sort e Heap Sort?
6. Por que busca binária exige array ordenado? O que acontece se o array não estiver?
7. Como você escreveria uma função comparadora para `qsort` que ordena strings em ordem decrescente?
8. O que é análise de complexidade de espaço? Qual algoritmo deste módulo usa O(1) espaço extra?
9. Qual algoritmo usaria para ordenar uma lista de 10 elementos dentro de um programa maior? E 10 milhões?

---

## Projeto — Benchmark de Ordenação

Implemente um programa que mede e compara a performance real dos algoritmos.

**Funcionalidades:**
- Gerar arrays aleatórios de tamanho 1.000, 10.000 e 100.000
- Ordenar cada array com todos os 6 algoritmos implementados
- Medir tempo com `clock()` e exibir tabela comparativa
- Testar também com arrays já ordenados e arrays em ordem inversa

**Requisitos técnicos:**
- Copiar o array original antes de cada sort (comparar na mesma entrada)
- Compilar com `gcc -Wall -Wextra -Werror -O0` (sem otimização do compilador)
- Verificar que o array está corretamente ordenado após cada algoritmo

**Exemplo de execução:**
```
Benchmark — n = 10.000 (aleatório)
┌─────────────────┬──────────────┬──────────────┬──────────────┐
│ Algoritmo       │ Aleatório    │ Ordenado     │ Invertido    │
├─────────────────┼──────────────┼──────────────┼──────────────┤
│ Bubble Sort     │  245.3 ms    │    1.2 ms    │  248.7 ms    │
│ Selection Sort  │  198.1 ms    │  197.9 ms    │  199.0 ms    │
│ Insertion Sort  │  112.4 ms    │    0.8 ms    │  224.1 ms    │
│ Merge Sort      │    4.2 ms    │    3.1 ms    │    3.3 ms    │
│ Quick Sort      │    2.9 ms    │   89.4 ms    │   91.2 ms    │
│ Heap Sort       │    5.1 ms    │    4.8 ms    │    4.9 ms    │
└─────────────────┴──────────────┴──────────────┴──────────────┘
```

---

## Exercícios

**→ [`ex01-basicos/`](ex01-basicos/main.c)** — bubble, selection, insertion; medir iterações
**→ [`ex02-merge-quick/`](ex02-merge-quick/main.c)** — merge sort e quick sort completos
**→ [`ex03-busca-binaria/`](ex03-busca-binaria/main.c)** — busca binária iterativa e recursiva
**→ [`ex04-benchmark/`](ex04-benchmark/main.c)** — comparar algoritmos com clock(); gerar gráfico de dados

---

## Recursos Adicionais

Estes recursos são **opcionais** mas vão solidificar seu entendimento:

**Para ler/assistir agora:**
- [Sorting Algorithms Animations](https://www.toptal.com/developers/sorting-algorithms) — visualização interativa de todos os algoritmos
- [Visualgo Sorting](https://visualgo.net/en/sorting) — animações passo a passo com pseudocódigo

**Para consulta:**
- **CLRS** capítulos 2, 6, 7, 8 — análise formal com provas de corretude e complexidade
- **Algorithms** — Sedgewick & Wayne (capítulos sobre sorting)

**Para ir além:**
- **CLRS** capítulo 8 (Sorting in Linear Time) — Counting Sort, Radix Sort, Bucket Sort
- [Tim Sort](https://en.wikipedia.org/wiki/Timsort) — o algoritmo usado em Python e Java em produção
