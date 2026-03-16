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
  ├── 01-transicao/
  ├── 02-oop/
  ├── 03-stl/
  └── 04-modern-cpp/
fase4-java/              # OOP empresarial, JVM, design patterns
  ├── 01-fundamentos/
  ├── 02-collections-streams/
  ├── 03-concorrencia/
  ├── 04-jvm/
  └── 05-design-patterns/
fase5-teoria-cs/         # Complexidade, paradigmas, grafos, compiladores, criptografia
  ├── 01-complexidade/
  ├── 02-paradigmas/
  ├── 03-grafos/
  ├── 04-compiladores/
  └── 05-criptografia/
fase6-eng-software/      # SOLID, Clean Code, testes, arquitetura, CI/CD
  ├── 01-solid/
  ├── 02-clean-code/
  ├── 03-testes/
  ├── 04-arquitetura/
  └── 05-cicd/
```

## Estrutura dos READMEs (Odin Project-style)

Todos os READMEs de módulo seguem este padrão com 5 seções obrigatórias:

1. **`## Antes de começar`** — pré-requisitos com checkboxes `- [ ]` (topo, antes do conteúdo)
2. **`## O que você vai aprender`** — learning outcomes em lista (logo após prerequisites)
3. **`## Knowledge Check`** — 8–10 perguntas sem resposta (antes de Referências)
4. **`## Projeto`** — projeto integrador com requisitos e exemplo de execução (antes de Referências)
5. **`## Recursos Adicionais`** — links divididos em: *Para ler/assistir agora* · *Para consulta* · *Para ir além* (após Referências)

Ao adicionar novos módulos ou editar READMEs existentes, manter este padrão.

## Convenções

- Todo código C deve compilar com `gcc -Wall -Wextra -Werror`
- Projetos da Fase 1 devem ter Makefile com targets: `all`, `clean`, `fclean`, `re`
- Sem memory leaks: testar com `valgrind --leak-check=full`
- Cada subpasta de exercício deve ter seu próprio `main.c` para testar
- Código Java: usar JUnit 5 + Mockito para testes; JaCoCo para cobertura
