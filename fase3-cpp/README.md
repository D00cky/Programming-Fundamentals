# Fase 3 — C++

> *"C++ is designed to allow you to express ideas, but if you don't have ideas or don't have any clue about how to express them, C++ doesn't offer much help."* — Bjarne Stroustrup

C++ não é "C com classes". É uma linguagem multi-paradigma que suporta programação procedural, orientada a objetos e genérica ao mesmo tempo. Entender C++ profundamente significa entender *quando* usar cada paradigma.

---

## Por que C++ depois de C?

C te ensinou a gerenciar memória manualmente e entender o hardware. C++ te ensina a criar abstrações que escondem essa complexidade sem perder performance — o princípio "zero-cost abstractions".

Quando você escrever `std::vector<int>`, vai saber que por baixo há um array dinâmico com `malloc`/`realloc`/`free`. Quando usar `std::unique_ptr`, vai saber que é só um ponteiro com destrutor automático.

---

## Módulos

| # | Módulo | Conceitos |
|---|--------|-----------|
| 01 | [Transição C → C++](01-transicao/README.md) | namespaces, refs, RAII, streams, classes básicas |
| 02 | [OOP](02-oop/README.md) | herança, polimorfismo, virtual, interfaces, operator overloading |
| 03 | [Templates & STL](03-stl/README.md) | templates, vector, map, set, iteradores, algorithms |
| 04 | [Modern C++](04-modern-cpp/README.md) | C++11/14/17: smart pointers, lambdas, move semantics, auto |

---

## Projetos

| Projeto | Depende de | O que vai construir |
|---------|------------|---------------------|
| [CPP Modules](projetos/cpp_modules/) | 01-04 | Módulos 00-09: OOP progressivo, herança múltipla, templates |
| [CPP Containers](projetos/cpp_containers/) | 03 | Reimplementar vector, map, stack, set com iterators |

---

## Referências da Fase

- **The C++ Programming Language** — Bjarne Stroustrup (4ª ed, cobre C++11)
- **Effective Modern C++** — Scott Meyers · *indispensável* para C++11/14
- **A Tour of C++** — Stroustrup · introdução concisa
- **cppreference.com** — documentação completa online
- **CppCon** (YouTube) — palestras dos melhores engenheiros C++
