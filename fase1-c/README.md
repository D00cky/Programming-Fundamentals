# Fase 1 — C: A Linguagem da Máquina

> *"C é a linguagem de programação mais próxima do hardware que você pode usar sem escrever assembly. Entender C é entender como o computador realmente funciona."*

---

## Por que C?

Porque C não esconde nada. Não há garbage collector, não há exceções automáticas, não há objetos mágicos. Cada byte de memória que você usa, você pediu. Cada byte que não foi liberado, você esqueceu. Esse nível de controle — e responsabilidade — é o que forma programadores que entendem o que está acontecendo de verdade.

Quando você terminar essa fase, vai entender:
- Por que Java tem GC (porque sem ele, você vaza memória como demonstrará na prática)
- Por que segfault acontece (porque ponteiros são só números)
- Por que algoritmos têm complexidade de tempo *e* de espaço
- O que toda linguagem de alto nível está fazendo por baixo dos panos

---

## Módulos

| # | Módulo | Conceitos | Status |
|---|--------|-----------|--------|
| 01 | [Fundamentos](01-fundamentos/README.md) | tipos, operadores, controle de fluxo, funções, arrays, strings, I/O | 🔵 atual |
| 02 | [Ponteiros & Memória](02-ponteiros-memoria/README.md) | ponteiros, aritmética, malloc/free, stack vs heap, Valgrind | ⬜ |
| 03 | [Estruturas de Dados](03-estruturas-dados/README.md) | linked list, stack, queue, hash table | ⬜ |
| 04 | [Ordenação & Busca](04-algoritmos-ordenacao/README.md) | bubble, merge, quick, heap, radix, binary search | ⬜ |
| 05 | [Árvores](05-arvores/README.md) | BST, AVL, heap, traversals | ⬜ |
| 06 | [Algoritmos Avançados](06-algoritmos-avancados/README.md) | string matching, backtracking, recursão avançada | ⬜ |

---

## Projetos

Os projetos são o coração da fase. Cada um consolida vários módulos ao mesmo tempo.

| Projeto | Depende de | O que vai aprender |
|---------|------------|-------------------|
| [libft](projetos/libft/) | 01, 02 | Reimplementar a libc: strings, memória, lista ligada |
| [ft_printf](projetos/ft_printf/) | libft, 01 | Variadic functions, formatação, parsing |
| [get_next_line](projetos/get_next_line/) | 01, 02 | Buffer estático, leitura de fd linha a linha |
| [push_swap](projetos/push_swap/) | 03, 04 | Ordenação com duas pilhas, otimização de operações |
| [minitalk](projetos/minitalk/) | signals | Comunicação IPC via sinais UNIX, bit manipulation |
| [pipex](projetos/pipex/) | fork/exec | Recriar pipes do shell: fork + dup2 + execve |

---

## Regras da Fase

Seguindo a norma 42SP:

```bash
# Todo código deve compilar sem warnings:
gcc -Wall -Wextra -Werror arquivo.c -o programa

# Sem memory leaks (verificar com Valgrind):
valgrind --leak-check=full --show-leak-kinds=all ./programa

# Projetos precisam de Makefile com targets:
make        # compila
make clean  # remove .o
make fclean # remove .o e executável
make re     # recompila do zero
```

---

## Referências da Fase

- **The C Programming Language** — Kernighan & Ritchie (K&R) · *o* livro
- **C Programming: A Modern Approach** — K.N. King · mais moderno, mais didático
- **CS50x** (Harvard) — Semanas 1–5 cobrem toda a base de C
- **Beej's Guide to C Programming** — beej.us/guide/bgc (gratuito online)
- **cdecl.org** — traduz declarações C complicadas para inglês
- **godbolt.org** — veja o assembly gerado pelo seu código em tempo real
