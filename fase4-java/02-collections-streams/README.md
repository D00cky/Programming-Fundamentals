# 02 — Collections & Streams

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

## Exercícios

**ex01:** contar frequência de palavras num texto usando Map; listar top-10 mais frequentes

**ex02:** dado uma lista de pessoas (nome, idade, cidade), usar Stream para: filtrar maiores de 18, agrupar por cidade, calcular média de idade por grupo

**ex03:** implementar um mini-banco de dados em memória usando Maps aninhados, com queries via Stream

---

## Referências

- **Effective Java** — capítulos sobre collections (items 43-49)
- **Modern Java in Action** — Urma, Fusco, Mycroft
- **Oracle Collections Tutorial** — docs.oracle.com/javase/tutorial/collections
