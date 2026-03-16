# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

Repositório de aprendizado de fundamentos de programação. Progressão: **C → C++ → Java**. Inspirado na metodologia da 42SP (projeto-based, sem professor), filosofia Akita (entender de verdade, ler bons livros) e currículo IME-USP.

Ver `roadmap.md` para o mapa completo de conteúdos e ordem de estudo.

## Estrutura

```
fase0-fundacao/          # Como a máquina funciona, terminal, git, compilação
fase1-c/                 # Fundamentos C, ponteiros, estruturas de dados, algoritmos
  ├── 01-fundamentos/
  ├── 02-ponteiros-memoria/
  ├── 03-estruturas-dados/
  ├── 04-algoritmos-ordenacao/
  ├── 05-arvores/
  ├── 06-algoritmos-avancados/
  └── projetos/          # libft, ft_printf, get_next_line, push_swap, minitalk, pipex
fase2-sistemas/          # Processos, signals, threads, sockets
  └── projetos/          # philosophers, minishell
fase3-cpp/               # OOP, templates, STL, Modern C++
fase4-java/              # OOP empresarial, JVM, design patterns
fase5-teoria-cs/         # Grafos, DP, complexidade computacional
fase6-eng-software/      # SOLID, Clean Code, testes, arquitetura
```

## Convenções

- Todo código C deve compilar com `gcc -Wall -Wextra -Werror`
- Projetos da Fase 1 devem ter Makefile com targets: `all`, `clean`, `fclean`, `re`
- Sem memory leaks: testar com `valgrind --leak-check=full`
- Cada subpasta de exercício deve ter seu próprio `main.c` para testar
