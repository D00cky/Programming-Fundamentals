# Roadmap: Fundamentos de Programação

Baseado na metodologia da **42SP** (projeto-based, peer-to-peer), na filosofia do **Akita** (entender de verdade, não copiar-colar, ler bons livros) e no currículo de Algoritmos da **IME-USP** (progressão lógica em C).

**Progressão de linguagens:** C → C++ → Java

---

## FASE 0 — Fundação da Máquina (Pré-C)

> *"Antes de programar, entenda a máquina."* — Akitando #80

- Como o computador funciona: CPU, memória, registradores
- Sistema binário, hexadecimal, representação de números
- Sistema operacional: processos, memória virtual, filesystem
- Terminal/Shell: comandos essenciais Linux
- Git: controle de versão como hábito desde o início
- Compilação: o que acontece de `.c` até executável (pré-processador → compilador → linker)

---

## FASE 1 — C: A Linguagem da Máquina

> *C te obriga a entender o que está acontecendo. Não tem abstração que te esconda.*

### 1.1 Fundamentos C
- Tipos: `int`, `char`, `float`, `double`, `size_t`
- Operadores e precedência
- Controle de fluxo: `if/else`, `while`, `for`, `switch`
- Funções: declaração, passagem por valor
- Strings em C: char arrays, null-terminator, manipulação (`strlen`, `strcpy`, `strcmp`)
- Input/Output: `printf`, `scanf`, `fgets`, `putchar`
- Code layout e documentação

### 1.2 Ponteiros e Memória (coração de C)
- Endereços e ponteiros
- Aritmética de ponteiros
- Ponteiros e arrays
- Passagem por referência
- Alocação dinâmica: `malloc`, `calloc`, `realloc`, `free`
- Memory leaks e debugging com Valgrind

### 1.3 Estruturas de Dados em C
*Implementar do zero, sem bibliotecas externas — filosofia 42SP*
- Structs e unions
- Arrays estáticos e dinâmicos
- Linked lists (simples, dupla, circular)
- Stacks (pilha): com array e com lista
- Queues (fila): com array e com lista
- Hash tables com tratamento de colisões

### 1.4 Algoritmos de Busca e Ordenação
*Baseado nos capítulos 9–14 do IME-USP*
- Binary search
- Bubble, Selection, Insertion sort
- Mergesort
- Quicksort
- Heapsort
- Radixsort
- Análise de complexidade: **Big-O notation**

### 1.5 Árvores
- Binary trees
- Binary search trees (BST)
- Traversal: inorder, preorder, postorder
- AVL trees (balanceamento automático)
- Heaps e priority queues

### 1.6 Algoritmos Avançados
- String matching: KMP, Boyer-Moore
- Hashing avançado
- Algoritmos de enumeração e backtracking
- Recursão avançada

### 1.7 Tooling & Ambiente
- Makefile: targets, variáveis, regras automáticas
- GDB: debugging step-by-step
- Valgrind: memcheck
- Normas de estilo de código

### 1.8 Projetos 42SP-style em C
| Projeto | Descrição |
|---------|-----------|
| **libft** | Reimplementar a libc do zero (strlen, memcpy, atoi, etc.) |
| **ft_printf** | Reimplementar printf com variadic functions |
| **get_next_line** | Ler linha por linha de um file descriptor |
| **push_swap** | Algoritmo de ordenação com pilhas, contagem de operações |
| **minitalk** | Comunicação entre processos via UNIX signals |
| **pipex** | Reimplementar pipes do shell (`|`) com fork/exec |

---

## FASE 2 — Sistemas: O C na Prática

> *Entender o SO é entender o chão onde seus programas vivem.*

- Processos: `fork`, `exec`, `wait`, signals
- File descriptors: `open`, `read`, `write`, `close`, `dup2`
- Pipes e redirecionamento
- Threads: pthreads, mutex, semáforos
- Sockets: TCP/IP básico, cliente/servidor

**Projetos:**
- **Philosophers** (Dining Philosophers): concorrência, deadlock, starvation
- **Minishell**: shell completo com builtins, pipes, redirections, variáveis de ambiente

---

## FASE 3 — C++: Abstração sem Perder o Controle

### 3.1 Transição C → C++
- Classes e objetos
- Construtores/destrutores, RAII
- Referências vs ponteiros
- const-correctness
- Namespaces

### 3.2 OOP em C++
- Herança (simples, múltipla)
- Polimorfismo e virtual functions
- Interfaces (pure virtual)
- Sobrecarga de operadores

### 3.3 Templates & STL
- Function templates e class templates
- `std::vector`, `std::list`, `std::map`, `std::set`, `std::unordered_map`
- Iterators
- Algoritmos da STL: `sort`, `find`, `transform`

### 3.4 Modern C++ (C++11/14/17)
- Smart pointers: `unique_ptr`, `shared_ptr`, `weak_ptr`
- Lambda functions
- Move semantics, rvalue references
- `auto`, `decltype`, range-based for
- `std::optional`, `std::variant`

### 3.5 Projetos 42SP-style em C++
- **Módulos 00–09**: série da 42 cobrindo OOP, templates, STL, exceptions, etc.
- **CPP Containers**: reimplementar `std::vector`, `std::stack`, `std::map` do zero

---

## FASE 4 — Java: OOP Empresarial e JVM

### 4.1 Fundamentos Java
- Tipos, classes, métodos
- OOP: herança, interfaces, abstract classes
- Generics
- Collections Framework: `ArrayList`, `LinkedList`, `HashMap`, `TreeMap`, `HashSet`
- Exceptions: checked vs unchecked
- I/O: `File`, `BufferedReader`, `Scanner`

### 4.2 Java Intermediário
- Concorrência: `Thread`, `Runnable`, `synchronized`, `Lock`
- Executor framework e thread pools
- Streams API (Java 8+)
- Lambdas e functional interfaces
- `Optional`

### 4.3 JVM e Performance
- Como a JVM funciona: classloader, JIT, GC
- Tipos de Garbage Collectors (Serial, Parallel, G1, ZGC)
- Profiling básico com VisualVM / JMH

### 4.4 Design Patterns (em Java)
- **Creational**: Singleton, Factory, Builder
- **Structural**: Adapter, Decorator, Proxy
- **Behavioral**: Observer, Strategy, Command, Iterator
- Referência: *Design Patterns* — GoF

### 4.5 Projetos Java
- Implementar estruturas de dados do zero em Java
- Projeto CRUD com padrões de projeto aplicados
- Projeto concorrente: produtor/consumidor, thread pool

---

## FASE 5 — Teoria CS: O que une tudo

- Complexidade computacional: P, NP, NP-completo
- Paradigmas de algoritmos:
  - Divide & Conquer
  - Dynamic Programming
  - Greedy Algorithms
  - Backtracking / Branch & Bound
- **Grafos**: representação (adj matrix/list), BFS, DFS, Dijkstra, Bellman-Ford, Floyd-Warshall, Kruskal, Prim
- Teoria de compiladores: tokens, parsing, AST
- Criptografia básica: hashing, simétrica/assimétrica

---

## FASE 6 — Engenharia de Software

- SOLID principles
- Clean Code
- Testes: unit, integration, TDD
- Refactoring
- Code review e pair programming
- CI/CD básico
- Arquitetura: layered, hexagonal, visão geral de microservices

---

## Livros de Referência

| Livro | Autor | Fase |
|-------|-------|------|
| The C Programming Language (K&R) | Kernighan & Ritchie | Fase 1 |
| C Programming: A Modern Approach | K. N. King | Fase 1 |
| Introduction to Algorithms (CLRS) | Cormen et al. | Fase 1–5 |
| Algorithms | Sedgewick & Wayne | Fase 1–5 |
| The C++ Programming Language | Bjarne Stroustrup | Fase 3 |
| Effective C++ | Scott Meyers | Fase 3 |
| Effective Java | Joshua Bloch | Fase 4 |
| Design Patterns | GoF | Fase 4–6 |
| Clean Code | Robert C. Martin | Fase 6 |
| The Pragmatic Programmer | Hunt & Thomas | Fase 6 |
| SICP | Abelson & Sussman | Fase 5 |

---

## Materiais Online

- **IME-USP Algoritmos**: https://www.ime.usp.br/~pf/algorithms/
- **42SP Curriculum**: https://42sp.org.br
- **Akitando** (YouTube): séries "Começando aos 40", episódios #80, #38, #37, #64
- **Akita Book**: "Introdução à Computação: Da Lógica aos Jogos com Ruby" — Casa do Código

---

## Estrutura do Repositório

```
Programming-Fundamentals/
├── roadmap.md
├── fase0-fundacao/
├── fase1-c/
│   ├── 01-fundamentos/
│   ├── 02-ponteiros-memoria/
│   ├── 03-estruturas-dados/
│   ├── 04-algoritmos-ordenacao/
│   ├── 05-arvores/
│   ├── 06-algoritmos-avancados/
│   └── projetos/
│       ├── libft/
│       ├── ft_printf/
│       ├── get_next_line/
│       ├── push_swap/
│       ├── minitalk/
│       └── pipex/
├── fase2-sistemas/
│   └── projetos/
│       ├── philosophers/
│       └── minishell/
├── fase3-cpp/
│   └── projetos/
├── fase4-java/
│   └── projetos/
├── fase5-teoria-cs/
└── fase6-eng-software/
```
