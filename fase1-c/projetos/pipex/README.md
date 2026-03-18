# pipex

> Fase 1 — C

Recriar o comportamento de pipe no shell: `fork` + `dup2` + `execve`.

## Critérios de conclusão

- [ ] Replica `cmd1 | cmd2` com redirecionamento de arquivo de entrada e saída
- [ ] Usa `fork`, `pipe`, `dup2` e `execve` corretamente
- [ ] Todos os file descriptors são fechados após o uso
- [ ] Trata erros (comando não encontrado, arquivo inexistente, permissões)
- [ ] Bonus: here_doc (`<<`) e múltiplos pipes

## Recursos

- [Especificação 42 School](https://projects.intra.42.fr/projects/pipex)
- [man 2 pipe](https://man7.org/linux/man-pages/man2/pipe.2.html)
- [man 2 dup2](https://man7.org/linux/man-pages/man2/dup2.2.html)
