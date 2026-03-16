# Fase 0 — Fundação da Máquina

> *"Antes de escrever código, entenda onde ele roda."*

Antes de tocar em C, você precisa entender a máquina. Não em nível de engenharia elétrica, mas o suficiente para que nada pareça "mágica" depois.

## Tópicos

| # | Tópico | O que você vai entender |
|---|--------|------------------------|
| 01 | [Como o Computador Funciona](01-como-computador-funciona/README.md) | CPU, memória, registradores, ciclo fetch-decode-execute |
| 02 | [Sistemas Numéricos](02-sistemas-numericos/README.md) | Binário, hexadecimal, complemento de 2, representação de dados |
| 03 | [Sistema Operacional](03-sistema-operacional/README.md) | Processos, memória virtual, filesystem, syscalls |
| 04 | [Terminal Linux](04-terminal-linux/README.md) | Comandos essenciais, navegação, pipes, redirecionamento |
| 05 | [Git](05-git/README.md) | Controle de versão como hábito desde o primeiro dia |
| 06 | [Compilação](06-compilacao/README.md) | O que acontece de `.c` até executável |

## Por que essa fase existe?

Quando você escrever `int x = 42;` em C, vai querer saber:
- Onde esse `42` está guardado na memória?
- Como o computador "sabe" que é um número e não um texto?
- Por que um `int` tem 4 bytes?
- O que o compilador fez com aquela linha?

Essa fase responde essas perguntas antes que elas apareçam.

## Referências da Fase 0

- **Akitando #80** — O Guia Hardcore de Introdução à Computação (YouTube)
- **CS50x** (Harvard, gratuito) — Semanas 0 e 1 cobrem boa parte disso
- **"But How Do It Know?"** — J. Clark Scott (livro curto e acessível sobre como CPUs funcionam)
- **The Linux Command Line** — William Shotts (gratuito online: linuxcommand.org)
- **Pro Git** — Scott Chacon (gratuito: git-scm.com/book)
