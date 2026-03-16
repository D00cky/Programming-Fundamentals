# 02 — Paradigmas Algorítmicos

---

## 1. Divide & Conquer

**Padrão:** divide o problema em subproblemas menores, resolve recursivamente, combina.

```
solve(problema):
    if problema é trivial: resolve diretamente
    sub1, sub2 = divide(problema)
    r1 = solve(sub1)
    r2 = solve(sub2)
    return combina(r1, r2)
```

**Exemplos:**
- Merge Sort: divide ao meio, merge ordena
- Quick Sort: partition, recursão em cada metade
- Binary Search: divide ao meio, recursão no lado correto
- Closest Pair of Points: divide em metades verticais, combina

**Contagem de inversões (O(n log n)):**
```
[2, 4, 1, 3, 5]: inversões = (2,1), (4,1), (4,3) → 3
Modificação do merge sort: contar inversões durante o merge
```

---

## 2. Programação Dinâmica

DP = Divide & Conquer + **memoização de subproblemas sobrepostos**.

**Quando usar:**
1. **Subestrutura ótima:** solução ótima contém soluções ótimas dos subproblemas
2. **Subproblemas sobrepostos:** mesmos subproblemas resolvidos várias vezes

**Abordagens:**
- **Top-down (memoização):** recursão + cache
- **Bottom-up (tabulação):** preencher tabela iterativamente

### Problemas Clássicos

**Mochila 0/1 (Knapsack):**
```
n itens com peso[i] e valor[i]; capacidade W
dp[i][w] = máximo valor usando itens 0..i com peso máximo w
dp[i][w] = max(dp[i-1][w], dp[i-1][w-peso[i]] + valor[i])
Complexidade: O(n·W)
```

**Longest Common Subsequence (LCS):**
```
s1 = "ABCBDAB", s2 = "BDCAB" → LCS = "BCAB" (len=4)
dp[i][j] = LCS de s1[0..i] e s2[0..j]
if s1[i] == s2[j]: dp[i][j] = dp[i-1][j-1] + 1
else:              dp[i][j] = max(dp[i-1][j], dp[i][j-1])
```

**Edit Distance (Levenshtein):**
```
min operações (insert, delete, replace) para transformar s1 em s2
dp[i][j] = edit distance entre s1[0..i] e s2[0..j]
```

**Coin Change:**
```
Mínimas moedas para fazer valor V com moedas dadas
dp[v] = min moedas para valor v
dp[v] = min(dp[v - moeda[i]] + 1) para cada moeda
```

**Longest Increasing Subsequence (LIS):**
```
[10, 9, 2, 5, 3, 7, 101, 18] → LIS = [2, 3, 7, 18] (len=4)
dp[i] = comprimento da LIS terminando em arr[i]
O(n²) ingênuo, O(n log n) com paciência sort
```

---

## 3. Greedy

Faz a escolha localmente ótima em cada etapa, esperando que isso leve ao ótimo global.

**Quando funciona:** quando a escolha gulosa é provadamente ótima (exchange argument).
**Quando falha:** na maioria dos problemas de otimização combinatória.

### Exemplos Clássicos

**Interval Scheduling:** quantos intervalos podem ser agendados sem sobreposição?
```
Greedy: ordenar por hora de FIM, pegar intervalos não conflitantes
Prova: qualquer outra solução pode ser trocada para a greedy sem piorar
```

**Huffman Coding:** compressão ótima de prefixo livre
```
Construir árvore de frequências: sempre combinar os dois nós mais leves
Resultado: código de comprimento variável ótimo (Shannon)
```

**Dijkstra:** grafo ponderado positivo → menor caminho
```
Greedy: sempre expande o vértice não visitado de menor distância conhecida
Funciona porque pesos positivos garantem que essa escolha é definitiva
```

**Falha do Greedy:**
```
Coin Change com moedas {1, 3, 4}, valor = 6
Greedy: 4+1+1 = 3 moedas
DP ótimo: 3+3 = 2 moedas  ← greedy falha!
(O greedy funciona com moedas canônicas: {1, 5, 10, 25})
```

---

## 4. Backtracking

Exploração sistemática do espaço de soluções, com poda (pruning) de ramos inválidos.

```
backtrack(estado):
    if estado completo: registra solução; return
    for cada escolha possível:
        if escolha é válida para estado atual:
            aplica escolha
            backtrack(novo estado)
            desfaz escolha   ← o "back" do backtracking
```

**Exemplos:**
- N-Rainhas
- Sudoku solver
- Permutações e subconjuntos
- Coloração de grafos
- Problema da mochila (força bruta)

**Poda:** quanto mais cedo detectar que um ramo não pode levar a solução, mais rápido.

---

## Comparação dos Paradigmas

| Paradigma | Estratégia | Garantia | Complexidade típica |
|-----------|-----------|----------|---------------------|
| D&C | Divide, resolve, combina | Ótimo se correto | O(n log n) |
| DP | Memoiza subproblemas | Ótimo | O(n²) a O(n·W) |
| Greedy | Escolha local ótima | Ótimo só em casos específicos | O(n log n) |
| Backtracking | Explora + poda | Completo (encontra todas as soluções) | Exponencial |

---

## Referências

- **CLRS** capítulos 15 (DP), 16 (Greedy)
- **Competitive Programmer's Handbook** — capítulos 5-7
- **Dynamic Programming for Coding Interviews** — Meenakshi & Kamal
- **LeetCode** — categoria DP e Greedy para praticar
