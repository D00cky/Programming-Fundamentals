# 02 — Collections & Streams

> *"Data manipulation is the core of enterprise software."*

---

## Antes de começar

Certifique-se de que você já:

- [ ] Criou classes Java com generics e interfaces (`fase4-java/01`)
- [ ] Entende a diferença entre `ArrayList` e arrays Java tradicionais
- [ ] Sabe usar lambdas básicas em Java (`() -> expressão`)
- [ ] Conhece os conceitos de imutabilidade e `Optional`

---

## O que você vai aprender

Ao final deste módulo você será capaz de:

- Escolher o container Java correto (List, Set, Map, Queue) baseado em complexidade
- Manipular coleções com a Stream API: `filter`, `map`, `collect`, `groupingBy`
- Evitar `NullPointerException` usando `Optional` corretamente
- Ordenar objetos com `Comparator.comparing` e `thenComparing`
- Usar `Map.computeIfAbsent`, `merge` e `getOrDefault` para padrões comuns
- Criar coleções imutáveis com `List.of`, `Map.of` (Java 9+)

---

## 1. Collections Framework

```
Collection
├── List (ordenado, permite duplicatas)
│   ├── ArrayList  — array dinâmico, O(1) acesso
│   └── LinkedList — lista duplamente ligada, O(1) add/remove nas pontas
├── Set (sem duplicatas)
│   ├── HashSet     — hash table, O(1), sem ordem
│   ├── LinkedHashSet — hash + lista, mantém ordem de inserção
│   └── TreeSet     — árvore BST, O(log n), ordenado
└── Queue / Deque
    ├── ArrayDeque  — fila dupla com array
    └── PriorityQueue — min-heap

Map (chave → valor, não herda de Collection)
├── HashMap        — hash table, O(1), sem ordem
├── LinkedHashMap  — hash + lista, mantém ordem de inserção
└── TreeMap        — árvore BST, O(log n), ordenado por chave
```

---

## 2. List — ArrayList vs LinkedList

```java
List<String> lista = new ArrayList<>();   // preferir por padrão
lista.add("hello");
lista.add(0, "world");        // inserção no início: O(n) para ArrayList
lista.get(0);                 // acesso por índice: O(1) para ArrayList, O(n) LinkedList
lista.remove("hello");        // por valor: O(n)
lista.remove(0);              // por índice: O(n) para ArrayList (shift)
lista.size();
lista.contains("world");      // O(n)
lista.sort(Comparator.naturalOrder());
Collections.sort(lista);

// Criar lista imutável:
List<String> imutavel = List.of("a", "b", "c");   // Java 9+
```

---

## 3. Map

```java
Map<String, Integer> freq = new HashMap<>();

// Adicionar/atualizar:
freq.put("hello", 1);
freq.put("hello", freq.getOrDefault("hello", 0) + 1);  // incrementar
freq.merge("hello", 1, Integer::sum);                   // equivalente conciso

// Acessar:
freq.get("hello")            // null se não existe
freq.getOrDefault("x", 0)   // 0 se não existe
freq.containsKey("hello")

// Iterar:
for (Map.Entry<String, Integer> entry : freq.entrySet())
    System.out.println(entry.getKey() + ": " + entry.getValue());

// Java 5+: forEach com lambda
freq.forEach((k, v) -> System.out.println(k + ": " + v));

// computeIfAbsent — criar valor lazy:
Map<String, List<Integer>> agrupados = new HashMap<>();
agrupados.computeIfAbsent("pares", k -> new ArrayList<>()).add(2);
```

---

## 4. Stream API (Java 8+)

Streams são pipelines de operações sobre sequências de dados. **Lazy**: só executam quando uma operação terminal é chamada.

```java
List<Integer> nums = List.of(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);

// Operações intermediárias (lazy, retornam Stream):
nums.stream()
    .filter(n -> n % 2 == 0)      // manter pares
    .map(n -> n * n)              // elevar ao quadrado
    .sorted()                     // ordenar
    .limit(3)                     // pegar 3 primeiros
    .forEach(System.out::println); // terminal

// Coletar resultado:
List<Integer> pares = nums.stream()
    .filter(n -> n % 2 == 0)
    .collect(Collectors.toList());

// Operações terminais de redução:
int soma   = nums.stream().mapToInt(Integer::intValue).sum();
OptionalInt max = nums.stream().mapToInt(Integer::intValue).max();
long count = nums.stream().filter(n -> n > 5).count();

String joined = List.of("a","b","c").stream()
    .collect(Collectors.joining(", ", "[", "]"));   // "[a, b, c]"

// Grouping:
Map<Boolean, List<Integer>> parImpar = nums.stream()
    .collect(Collectors.partitioningBy(n -> n % 2 == 0));

Map<Integer, List<String>> porTamanho = palavras.stream()
    .collect(Collectors.groupingBy(String::length));
```

---

## 5. Optional — Evitando NullPointerException

```java
// Em vez de retornar null:
public Optional<Usuario> buscarPorEmail(String email) {
    // ...
    return Optional.ofNullable(resultado);  // ou Optional.empty()
}

Optional<Usuario> user = buscarPorEmail("test@test.com");

// Usar:
user.isPresent()              // true se tem valor
user.get()                   // valor (lança se vazio)
user.orElse(padrão)         // valor ou padrão
user.orElseGet(() -> novo()) // valor ou computado lazy
user.orElseThrow()           // valor ou lança NoSuchElementException

// Encadeamento:
user.map(Usuario::getNome)
    .filter(nome -> nome.startsWith("A"))
    .ifPresent(System.out::println);
```

---

## 6. Comparators

```java
List<Produto> produtos = new ArrayList<>();

// Ordenar por preço:
produtos.sort(Comparator.comparingDouble(Produto::getPreco));

// Ordenar por nome, depois por preço:
produtos.sort(Comparator.comparing(Produto::getNome)
    .thenComparingDouble(Produto::getPreco));

// Ordem reversa:
produtos.sort(Comparator.comparingDouble(Produto::getPreco).reversed());

// Com stream:
produtos.stream()
    .sorted(Comparator.comparing(Produto::getCategoria)
        .thenComparing(Produto::getNome))
    .collect(Collectors.toList());
```

---

## Knowledge Check

Responda sem consultar o material. Se travar, releia a seção correspondente.

1. Qual a diferença entre `ArrayList` e `LinkedList`? Quando usar cada um?
2. Por que `HashMap` é O(1) em média mas O(n) no pior caso?
3. O que significa dizer que Streams são "lazy"? Dê um exemplo onde isso importa.
4. Qual a diferença entre operação intermediária e terminal em Stream?
5. O que `Optional.orElseGet` tem de diferente de `Optional.orElse`? Quando importa?
6. Por que `freq.merge("key", 1, Integer::sum)` é preferível a `getOrDefault + put`?
7. Qual a diferença entre `HashSet` e `TreeSet`? Quando usar cada um?
8. O que `Collectors.groupingBy` faz? Dê um exemplo de uso real.

---

## Projeto — Sistema de Análise de Vendas

Implemente um sistema que processa dados de vendas usando Collections e Stream API.

**Dados:**
```java
record Venda(String produto, String categoria, double valor, String vendedor, LocalDate data) {}
```

**Funcionalidades a implementar com Streams:**
- Top 5 produtos mais vendidos por valor total
- Total de vendas por categoria
- Vendedor com maior volume de vendas no mês
- Média de valor por categoria
- Produtos vendidos com valor abaixo da média geral

**Requisitos técnicos:**
- Carregar dados de arquivo CSV (simular com lista estática)
- Usar `Collectors.groupingBy`, `Collectors.summingDouble`, `Collectors.toMap`
- Compilar com `javac` e rodar com `java`
- Sem loops imperativos — apenas Stream API

**Exemplo de execução:**
```
=== Top 5 produtos por receita ===
1. Notebook Pro    R$ 45.230,00
2. Smartphone X    R$ 38.900,00
...

=== Vendas por categoria ===
Eletrônicos:  R$ 89.430,00
Vestuário:    R$ 12.300,00
...

=== Melhor vendedor (março/2026) ===
Ana Silva: R$ 23.400,00
```

---

## Exercícios

**ex01:** contar frequência de palavras num texto usando Map; listar top-10 mais frequentes

**ex02:** dado uma lista de pessoas (nome, idade, cidade), usar Stream para: filtrar maiores de 18, agrupar por cidade, calcular média de idade por grupo

**ex03:** implementar um mini-banco de dados em memória usando Maps aninhados, com queries via Stream

---

## Recursos Adicionais

Estes recursos são **opcionais** mas vão solidificar seu entendimento:

**Para ler/assistir agora:**
- **Modern Java in Action** — Urma, Fusco, Mycroft — o melhor livro sobre Stream API
- [Oracle Collections Tutorial](https://docs.oracle.com/javase/tutorial/collections/) — documentação oficial

**Para consulta:**
- **Effective Java** — Joshua Bloch, capítulos sobre collections (items 43-49)
- [Java Stream API Javadoc](https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/stream/Stream.html)

**Para ir além:**
- [Collectors deep dive](https://www.baeldung.com/java-collectors) — todos os Collectors da API
- Reactive Streams (Project Reactor / RxJava) — streams assíncronos e não-bloqueantes
