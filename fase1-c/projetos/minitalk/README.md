# minitalk

> Fase 1 — C

Comunicação entre processos via sinais UNIX (`SIGUSR1`/`SIGUSR2`), bit a bit.

## Critérios de conclusão

- [ ] Client envia string para o server codificada em bits via sinais
- [ ] Server imprime a string recebida corretamente
- [ ] Funciona com strings longas e caracteres Unicode (bonus)
- [ ] Server envia acknowledgement ao client após cada mensagem (bonus)
- [ ] Sem undefined behavior com `sigaction`

## Recursos

- [Especificação 42 School](https://projects.intra.42.fr/projects/minitalk)
- [man 2 sigaction](https://man7.org/linux/man-pages/man2/sigaction.2.html)
- [man 2 kill](https://man7.org/linux/man-pages/man2/kill.2.html)
