# 01 — Complexidade Computacional

---

## 1. Notações de Crescimento

**Big-O** (O) — limite superior assintótico (pior caso):
```
f(n) = O(g(n)) ⟺ ∃c,n₀: f(n) ≤ c·g(n) para n > n₀
```

**Big-Omega** (Ω) — limite inferior (melhor caso):
**Big-Theta** (Θ) — limite exato (melhor e pior caso):

```
Θ(1) < Θ(log n) < Θ(√n) < Θ(n) < Θ(n log n) < Θ(n²) < Θ(n³) < Θ(2ⁿ) < Θ(n!)
```

---

## 2. Análise de Algoritmos

### Regras básicas

```
// Loop simples: O(n)
for i = 1 to n:
    operação O(1)

// Loops aninhados: O(n²)
for i = 1 to n:
    for j = 1 to n:
        operação O(1)

// Divisão ao meio a cada iteração: O(log n)
while n > 1:
    n = n / 2

// Combinação: O(n log n)
for i = 1 to n:         // O(n) vezes
    while n > 1: n /= 2 // O(log n) cada
```

### Master Theorem

Para recorrências do tipo T(n) = aT(n/b) + f(n):

- Se f(n) = O(n^(log_b(a) - ε)) → T(n) = Θ(n^log_b(a))
- Se f(n) = Θ(n^log_b(a)) → T(n) = Θ(n^log_b(a) · log n)
- Se f(n) = Ω(n^(log_b(a) + ε)) → T(n) = Θ(f(n))

```
Merge Sort: T(n) = 2T(n/2) + O(n)
  a=2, b=2, f(n)=n, log_2(2)=1
  f(n) = Θ(n^1) → caso 2 → T(n) = Θ(n log n) ✓
```

---

## 3. Análise de Espaço

Além do tempo, analisar **espaço auxiliar**:

```
Merge Sort:   O(n) espaço extra (array auxiliar)
Quick Sort:   O(log n) espaço (call stack)
Heap Sort:    O(1) espaço extra (in-place)
Hash Table:   O(n) espaço
```

---

## 4. Classes de Complexidade

### P — Polynomial time
Problemas solucionáveis em tempo polinomial O(nᵏ):
- Ordenação: O(n log n)
- Menor caminho (Dijkstra): O((V+E) log V)
- Matching em grafos bipartidos: O(V·E)

### NP — Nondeterministic Polynomial
Problemas cuja **solução pode ser verificada** em tempo polinomial.

Exemplos: SAT, Clique, Caminho Hamiltoniano, Mochila 0/1

### NP-Completo
Os problemas **mais difíceis** em NP. Se qualquer NP-completo tiver solução polinomial, então P = NP.

- **SAT** (satisfatibilidade booleana) — primeiro provado NP-completo (Cook, 1971)
- **3-SAT, Clique, Vertex Cover, TSP** — todos se reduzem uns aos outros

### NP-Hard
Pelo menos tão difíceis quanto qualquer problema NP, mas não necessariamente em NP.

```
P ⊆ NP ⊆ PSPACE ⊆ EXPTIME

A questão "P = NP?" é o problema aberto mais famoso da CS
```

---

## 5. Reduções

Mostrar que problema A é tão difícil quanto B: reduzir A para B em tempo polinomial.

```
Se B é NP-completo e podemos reduzir A → B em tempo polinomial,
então A também é NP-completo (ou NP-hard).

3-SAT → Clique → Vertex Cover → ... (cadeia de reduções)
```

---

## 6. Análise Amortizada

Custo médio de operação ao longo de uma sequência, mesmo que operações individuais sejam caras:

**Exemplo: push em vetor dinâmico**
- Maioria das vezes: O(1)
- Quando redimensiona (dobra capacidade): O(n)
- Amortizado: O(1) por push

**Método do potencial:** associar "energia potencial" à estrutura. Operações baratas acumulam potencial; operações caras gastam potencial acumulado.

---

## Referências

- **CLRS** capítulos 3, 4 (complexidade, recorrências), 34 (NP-completeness)
- **Algorithms** — Dasgupta, Papadimitriou, Vazirani · gratuito online
- **P vs NP Problem** — Clay Mathematics Institute (claymath.org)
- **Complexity Zoo** — complexityzoo.uwaterloo.ca
