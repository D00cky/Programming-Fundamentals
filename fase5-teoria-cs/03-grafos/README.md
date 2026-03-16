# 03 — Teoria dos Grafos

## Antes de começar

Certifique-se de que você já:

- [ ] Domina structs e ponteiros em C ou C++ (`fase1-c/02-ponteiros-memoria`)
- [ ] Implementou listas ligadas e sabe usar alocação dinâmica (`fase1-c/03-estruturas-dados`)
- [ ] Entende recursão e sabe implementar algoritmos com call stack
- [ ] Concluiu `fase5-teoria-cs/01-complexidade` — notação Big-O e análise de loops

---

## O que você vai aprender

Ao final deste módulo você será capaz de:

- Implementar BFS e DFS e usá-los para detectar ciclos e componentes conexos
- Implementar Dijkstra para menor caminho em grafos com pesos não-negativos
- Implementar Kruskal com Union-Find para Árvore Geradora Mínima
- Realizar ordenação topológica em DAGs usando DFS ou algoritmo de Kahn
- Escolher a representação correta (lista vs. matriz de adjacência) para cada contexto

---

## 1. Fundamentos

**Grafo** G = (V, E): vértices V e arestas E.
- **Dirigido (dígrafo):** arestas têm direção (u→v ≠ v→u)
- **Não-dirigido:** arestas bidirecionais
- **Ponderado:** arestas têm pesos/custos
- **DAG:** Directed Acyclic Graph — sem ciclos dirigidos

**Propriedades:**
```
|E| ≤ |V|²          (máximo de arestas)
|E| = O(|V|²)       → grafo denso → usar matriz de adjacência
|E| = O(|V|)        → grafo esparso → usar lista de adjacência
```

---

## 2. Menor Caminho

### Dijkstra — grafos ponderados positivos, O((V+E) log V)

```
dist[s] = 0; dist[v] = ∞ para v ≠ s
Usar min-heap (priority_queue)
Enquanto heap não vazio:
    u = vértice com menor dist
    Para cada vizinho v de u:
        if dist[u] + peso(u,v) < dist[v]:
            dist[v] = dist[u] + peso(u,v)
            adicionar v ao heap
```

**Requer pesos não-negativos.** Pesos negativos → Bellman-Ford.

### Bellman-Ford — pesos negativos, O(V·E)

```
dist[s] = 0; dist[v] = ∞
Repetir V-1 vezes:
    Para cada aresta (u,v,w):
        if dist[u] + w < dist[v]:
            dist[v] = dist[u] + w

Verificar ciclo negativo: se alguma dist muda na V-ésima iteração → ciclo negativo detectado
```

### Floyd-Warshall — todos os pares, O(V³)

```
dist[i][j] = peso direto ou ∞
Para k = 1 to V:
    Para i = 1 to V:
        Para j = 1 to V:
            dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j])
```

---

## 3. Árvore Geradora Mínima (MST)

**MST:** subgrafo que conecta todos os vértices com custo mínimo (sem ciclos).

### Algoritmo de Kruskal — O(E log E)

```
Ordenar arestas por peso (crescente)
Para cada aresta (u, v, w) em ordem:
    if u e v estão em componentes diferentes (Union-Find):
        adicionar aresta à MST
        unir componentes
```

### Algoritmo de Prim — O((V+E) log V)

```
Começar de vértice arbitrário
Usar min-heap com {peso, vértice}
Enquanto MST não tem todos os vértices:
    u = vértice não visitado de menor peso
    adicionar u à MST
    para cada vizinho v de u: atualizar heap
```

### Union-Find (Disjoint Set)

Estrutura para Kruskal — union e find em O(α(n)) ≈ O(1):

```c
int parent[MAX], rank[MAX];

int find(int x) {
    if (parent[x] != x) parent[x] = find(parent[x]);  // path compression
    return parent[x];
}

void unite(int x, int y) {
    int px = find(x), py = find(y);
    if (rank[px] < rank[py]) swap(&px, &py);
    parent[py] = px;
    if (rank[px] == rank[py]) rank[px]++;
}
```

---

## 4. Ordenação Topológica

Para DAGs: ordem linear tal que para toda aresta u→v, u vem antes de v.

**Via DFS:**
```
Para cada vértice não visitado: DFS
Ao terminar DFS de v: empilhar v
Resultado: inverter a pilha = ordem topológica
```

**Kahn's Algorithm (via in-degree):**
```
Calcular in-degree de cada vértice
Enfileirar vértices com in-degree = 0
Enquanto fila não vazia:
    u = dequeue
    adicionar u à ordenação
    para cada vizinho v: reduzir in-degree[v]; se 0, enfileirar v
```

**Usos:** ordem de compilação (dependências), detecção de ciclos, scheduling.

---

## 5. Componentes Fortemente Conexos (SCC)

**SCC:** maximal subgrafo onde todo vértice é alcançável de qualquer outro.

**Algoritmo de Kosaraju — O(V+E):**
1. DFS no grafo original → pilha por ordem de término
2. Transpor o grafo (inverter arestas)
3. DFS no grafo transposto na ordem da pilha → cada DFS = um SCC

**Algoritmo de Tarjan — O(V+E):** único DFS, usa low-link values.

---

## 6. Fluxo Máximo

**Problema:** dado grafo dirigido com capacidades nas arestas, achar fluxo máximo de S a T.

**Ford-Fulkerson / Edmonds-Karp:**
```
Enquanto existe caminho aumentante de S a T (BFS em Edmonds-Karp):
    fluxo += mínimo da capacidade residual no caminho
    atualizar capacidades residuais
Complexidade: O(V · E²) com Edmonds-Karp
```

**Aplicações:** matching bipartido, corte mínimo, redes de distribuição.

---

## Algoritmos Resumidos

| Problema | Algoritmo | Complexidade |
|----------|-----------|-------------|
| BFS/DFS | — | O(V+E) |
| Menor caminho (pesos ≥0) | Dijkstra | O((V+E)logV) |
| Menor caminho (pesos neg) | Bellman-Ford | O(V·E) |
| Todos os pares | Floyd-Warshall | O(V³) |
| MST | Kruskal / Prim | O(E log E) |
| Ordenação topológica | DFS / Kahn | O(V+E) |
| SCC | Kosaraju / Tarjan | O(V+E) |
| Fluxo máximo | Edmonds-Karp | O(V·E²) |

---

## Knowledge Check

Responda sem consultar o material. Se travar, releia a seção correspondente.

1. Quando usar lista de adjacência vs. matriz de adjacência? O que muda em memória e tempo de acesso?
2. BFS e DFS têm a mesma complexidade O(V+E). Qual usar para encontrar menor caminho não ponderado?
3. Por que Dijkstra não funciona com pesos negativos? O que Bellman-Ford faz diferente?
4. Descreva Kruskal em palavras. Por que Union-Find é necessário?
5. O que é uma ordenação topológica? Em que tipo de grafo ela existe?
6. Como detectar se um grafo dirigido tem ciclo usando DFS?
7. Qual a diferença entre MST e menor caminho? Um MST garante menor caminho entre dois vértices?
8. O que são Componentes Fortemente Conexos (SCC)? Dê um exemplo de uso prático.

---

## Projeto — Sistema de Mapa de Cidades

Implemente um programa que simula um sistema de rotas entre cidades:

**Funcionalidades:**
- Carregar grafo de um arquivo de texto (vértices = cidades, arestas = estradas com distância km)
- Calcular menor caminho entre duas cidades (Dijkstra)
- Calcular a rede de estradas mínima que conecta todas as cidades (MST com Kruskal)
- Detectar se todas as cidades são alcançáveis a partir de uma origem (BFS)
- Exibir o resultado formatado no terminal

**Formato do arquivo de entrada:**
```
5 7          # vertices arestas
São Paulo Campinas 90
São Paulo Santos 80
Campinas Ribeirão 180
...
```

**Exemplo de saída:**
```
=== Menor caminho: São Paulo → Ribeirão ===
São Paulo → Campinas → Ribeirão (270 km)

=== Rede mínima (MST) ===
Total: 5 arestas, 490 km
São Paulo - Santos (80 km)
São Paulo - Campinas (90 km)
...
```

**Requisitos técnicos:**
- Em C ou C++, compilar sem warnings com `-Wall -Wextra`
- Usar lista de adjacência para representar o grafo
- Union-Find com path compression para Kruskal

---

## Referências

- **CLRS** capítulos 22-26 (grafos, menor caminho, MST, fluxo)
- **Competitive Programmer's Handbook** — capítulos 11-15
- **Visualgo** — visualgo.net/en/sssp (animações)
- **Graph Theory** — Douglas West (mais teórico)

---

## Recursos Adicionais

Estes recursos são **opcionais** mas vão solidificar seu entendimento:

**Para ler/assistir agora:**
- **Visualgo** (visualgo.net) — animações interativas de BFS, DFS, Dijkstra, Kruskal; ótimo para construir intuição visual
- **MIT 6.006** aulas de grafos (YouTube/OCW) — cobertura rigorosa com provas de corretude

**Para consulta:**
- **Competitive Programmer's Handbook** caps. 11-15 — implementações prontas em C++ com análise de complexidade
- **CLRS capítulos 22-26** — algoritmos de grafos com provas formais

**Para ir além:**
- **Introduction to Graph Theory** — Douglas West · fundamentos matemáticos rigorosos
- **Network Flows** — Ahuja, Magnanti, Orlin · cobertura avançada de fluxo máximo e aplicações
