# Fase 2 — Sistemas

> *"The purpose of abstracting is not to be vague, but to create a new semantic level in which one can be absolutely precise."* — Dijkstra

Com o C dominado, agora você entende o que o sistema operacional faz. Essa fase abre a caixa preta: processos, signals, threads, sockets — os mecanismos que fazem qualquer sistema real funcionar.

---

## Por que essa fase existe?

Quando você usa `|` no shell, um `fork()` + `pipe()` + dois `execve()` acontecem. Quando um servidor HTTP aceita 1000 conexões simultâneas, threads ou event loops gerenciam isso. Quando o minishell roda um comando, ele cria um processo filho que herda os file descriptors.

Entender isso te diferencia de quem só usa frameworks.

---

## Módulos

| # | Módulo | Conceitos | Status |
|---|--------|-----------|--------|
| 01 | [Processos & Sinais](01-processos-sinais/README.md) | fork, exec, wait, signals, kill | ⬜ |
| 02 | [File Descriptors & I/O](02-file-descriptors/README.md) | open/read/write, dup2, pipe, redirecionamento | ⬜ |
| 03 | [Threads & Concorrência](03-threads-concorrencia/README.md) | pthreads, mutex, semáforos, race conditions | ⬜ |
| 04 | [Sockets & Redes](04-sockets/README.md) | TCP/IP, socket(), bind(), listen(), cliente/servidor | ⬜ |

---

## Projetos

| Projeto | Depende de | O que vai construir |
|---------|------------|---------------------|
| [philosophers](projetos/philosophers/) | 01, 03 | Jantar dos filósofos com threads + mutexes |
| [minishell](projetos/minishell/) | 01, 02 | Shell POSIX com parsing, pipes, builtins, signals |

---

## Referências da Fase

- **The Linux Programming Interface** — Michael Kerrisk · a bíblia de sistemas POSIX
- **Advanced Programming in the UNIX Environment** — Stevens & Rago (APUE)
- **Operating Systems: Three Easy Pieces** — ostep.org (gratuito online)
- `man 2 fork` · `man 2 pipe` · `man 7 signal` · `man 7 pthreads`
