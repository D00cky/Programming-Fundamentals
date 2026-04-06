# Fase 4 — Java

> *"Java is C++ without the guns, knives, and clubs."* — James Gosling

Java é onde a programação orientada a objetos fica séria. A JVM, o garbage collector, o sistema de tipos forte, os design patterns — tudo isso junto é o que faz Java ser a linguagem de escolha para sistemas empresariais de grande escala.

---

## Por que Java depois de C++?

Depois de C, você entendeu memória. Depois de C++, você entendeu OOP com controle total. Java esconde o gerenciamento de memória (GC) e força boas práticas de design. O contraste é intencional: quando o GC pausar sua aplicação por 500ms, você vai entender exatamente o que está acontecendo.

---

## Setup

```bash
# Instalar JDK 21 (LTS atual):
sudo pacman -S jdk21-openjdk   # Arch
sudo apt install openjdk-21-jdk  # Ubuntu

java --version
javac --version

# Compilar e rodar:
javac Main.java
java Main
```

---

## Módulos

| # | Módulo | Conceitos |
|---|--------|-----------|
| 01 | [Fundamentos](01-fundamentos/README.md) | tipos, OOP, interfaces, generics, exceções, enums, lambdas |
| 02 | [Collections & Streams](02-collections-streams/README.md) | ArrayList, HashMap, TreeMap, Stream API, Optional |
| 03 | [Concorrência](03-concorrencia/README.md) | Thread, Executor, CompletableFuture, synchronized, locks |
| 04 | [JVM Internals](04-jvm/README.md) | bytecode, ClassLoader, GC, JIT, profiling, heap tuning |
| 05 | [Design Patterns](05-design-patterns/README.md) | GoF 23 patterns: creational, structural, behavioral |

---

## Projetos

| Projeto | Depende de | O que vai construir |
|---------|------------|---------------------|
| [Java DS](projetos/java_ds/) | 01, 02 | Reimplementar ArrayList, LinkedList, HashMap, PriorityQueue com generics |
| [Java CRUD](projetos/java_crud/) | 01-05 | API REST com Spring Boot, JPA, padrões Repository e Service |
| [Java Concurrent](projetos/java_conc/) | 03 | Produtor-consumidor, thread pool, fork-join framework |

---

## Referências da Fase

- **Effective Java** — Joshua Bloch · *a* referência de boas práticas Java
- **Java Concurrency in Practice** — Goetz · indispensável para concorrência
- **Head First Design Patterns** — Freeman · didático e visual
- **Design Patterns** — GoF (Gang of Four) · o livro original dos 23 patterns
- **docs.oracle.com/javase** — JavaDoc oficial
