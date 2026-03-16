# 01 — Complexidade Computacional

---

## Antes de começar

Certifique-se de que você já:

- [ ] Sabe escrever loops e recursão em C ou outra linguagem (`fase1-c/01-fundamentos`, `fase1-c/02-ponteiros-memoria`)
- [ ] Entende o que é uma função matemática básica (exponencial, logaritmo)
- [ ] Implementou ao menos um algoritmo de ordenação (`fase1-c/04-algoritmos-ordenacao`)
- [ ] Sabe calcular o número de iterações de um loop simples

---

## O que você vai aprender

Ao final deste módulo você será capaz de:

- Classificar algoritmos usando notação Big-O, Big-Omega e Big-Theta
- Aplicar o Teorema Mestre para resolver recorrências divide-and-conquer
- Calcular espaço auxiliar de algoritmos além do tempo de execução
- Identificar se um problema pertence às classes P, NP ou NP-Completo
- Explicar análise amortizada e aplicar o método do potencial
- Comparar dois algoritmos e escolher o mais eficiente para um contexto dado

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

---

## Knowledge Check

Responda sem consultar o material. Se travar, releia a seção correspondente.

1. Qual a diferença entre O(n), Ω(n) e Θ(n)? Quando cada notação é mais útil?
2. Um algoritmo tem dois loops aninhados, o externo roda n vezes e o interno roda log(n) vezes. Qual a complexidade total?
3. Qual é a complexidade de busca binária? Escreva a recorrência e resolva com o Teorema Mestre.
4. Por que Merge Sort usa O(n) de espaço auxiliar mas Quick Sort usa O(log n)?
5. O que significa dizer que um problema está em NP? Todo problema NP é difícil de resolver?
6. Se P = NP, o que muda no mundo da criptografia?
7. Explique análise amortizada com o exemplo do vetor dinâmico. Por que O(n) por push é errado mesmo que às vezes aconteça?
8. Dado T(n) = 3T(n/3) + O(n), qual é a solução pelo Teorema Mestre?
9. Qual a diferença entre NP-Completo e NP-Hard? Um problema NP-Hard precisa estar em NP?
10. Por que é ineficiente usar complexidade de pior caso para todos os cenários? Dê um exemplo onde o caso médio importa mais.

---

## Projeto — Benchmark de Algoritmos

Implemente um programa que compare empiricamente a complexidade de diferentes algoritmos:

**Funcionalidades:**
- Implemente em C: Bubble Sort O(n²), Merge Sort O(n log n) e Binary Search O(log n)
- Meça o tempo de execução real para tamanhos de entrada: 100, 1000, 10000, 100000 elementos
- Calcule a razão entre tempos consecutivos e compare com a razão teórica esperada
- Exiba uma tabela comparativa no terminal

**Requisitos técnicos:**
- Usar `clock()` ou `gettimeofday()` para medir tempo
- Compilar com `gcc -Wall -Wextra -Werror -O0` (sem otimização para não distorcer)
- Testar com arrays em ordem aleatória, crescente e decrescente

**Exemplo de saída:**
```
Algoritmo     | n=1000  | n=10000 | n=100000 | Razão(10x)
Bubble Sort   | 2.1ms   | 215ms   | 21500ms  | ~100x  (esperado n²: 100x)
Merge Sort    | 0.12ms  | 1.4ms   | 16ms     | ~11.5x (esperado n log n: ~11.7x)
Binary Search | 0.001ms | 0.001ms | 0.002ms  | ~1.3x  (esperado log n: ~1.3x)
```

---

## Recursos Adicionais

**Para ler/assistir agora:**
- **CLRS** capítulos 3, 4 — notações assintóticas e recorrências, com provas rigorosas
- **Algorithms** — Dasgupta, Papadimitriou, Vazirani · gratuito online · mais acessível que CLRS

**Para consulta:**
- **Complexity Zoo** — complexityzoo.net · catálogo completo de classes de complexidade
- **Big-O Cheat Sheet** — bigocheatsheet.com · referência rápida de algoritmos clássicos

**Para ir além:**
- **CLRS** capítulo 34 (NP-completeness) — provas formais de NP-completude
- **P vs NP Problem** — Clay Mathematics Institute · descrição formal do problema de 1 milhão de dólares
