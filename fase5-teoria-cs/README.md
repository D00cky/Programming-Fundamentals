# Fase 5 — Teoria CS

> *"Computer Science is no more about computers than astronomy is about telescopes."* — Dijkstra

Essa fase é sobre os fundamentos matemáticos e teóricos que fundamentam toda a computação. Você não precisa disso para escrever um CRUD — mas precisa para entender por que algoritmos são rápidos ou lentos, como compiladores funcionam, e o que é possível computar.

---

## Por que essa fase existe?

Com C, C++ e Java dominados, você é um programador competente. Essa fase te torna um **engenheiro de software** — alguém que raciocina sobre problemas em vez de só codar soluções.

Entender complexidade te impede de escolher O(n²) quando existe O(n log n). Entender grafos abre um mundo de problemas (rotas, redes, dependências). Entender compiladores te faz ver linguagens com olhos diferentes.

---

## Módulos

| # | Módulo | Conceitos |
|---|--------|-----------|
| 01 | [Complexidade Computacional](01-complexidade/README.md) | P, NP, NP-completo, reduções, Big-O rigoroso, Master Theorem |
| 02 | [Paradigmas Algorítmicos](02-paradigmas/README.md) | Divide & Conquer, Programação Dinâmica, Greedy, Backtracking |
| 03 | [Teoria dos Grafos](03-grafos/README.md) | Dijkstra, Bellman-Ford, Floyd-Warshall, MST, SCC, fluxo |
| 04 | [Compiladores](04-compiladores/README.md) | Léxico, parser LL/LR, AST, geração de código, otimização |
| 05 | [Criptografia](05-criptografia/README.md) | Simétrica (AES), assimétrica (RSA/EC), hashes, TLS |

---

## Referências da Fase

- **Introduction to Algorithms (CLRS)** — Cormen et al. · a bíblia de algoritmos
- **The Algorithm Design Manual** — Steven Skiena · mais prático que o CLRS
- **Competitive Programmer's Handbook** — Antti Laaksonen · gratuito em cses.fi
- **Compilers: Principles, Techniques, and Tools** — Aho et al. (Dragon Book)
- **Cryptography and Network Security** — Stallings
