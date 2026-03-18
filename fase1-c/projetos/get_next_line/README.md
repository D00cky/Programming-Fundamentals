# get_next_line

> Fase 1 — C

Leitura linha-a-linha de file descriptor com buffer estático.

## Critérios de conclusão

- [ ] Lê uma linha por chamada, retornando `NULL` no fim
- [ ] Funciona com qualquer valor de `BUFFER_SIZE` (1, 42, 9999…)
- [ ] Bonus: suporte a múltiplos fds simultâneos
- [ ] Sem memory leaks e sem reads além do necessário
- [ ] Compila com `gcc -Wall -Wextra -Werror -D BUFFER_SIZE=42`

## Recursos

- [Especificação 42 School](https://projects.intra.42.fr/projects/get_next_line)
- [gnlTester (Tripouille)](https://github.com/Tripouille/gnlTester)
- [man 2 read](https://man7.org/linux/man-pages/man2/read.2.html)
