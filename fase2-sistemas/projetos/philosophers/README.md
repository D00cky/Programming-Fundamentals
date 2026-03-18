# philosophers

> Fase 2 — Sistemas

Jantar dos filósofos: threads, mutexes, evitar deadlock e starvation.

## Critérios de conclusão

- [ ] N filósofos comem, dormem e pensam sem morrer (quando a simulação for infinita)
- [ ] Nenhum filósofo morre de fome quando os tempos permitem sobrevivência
- [ ] Sem data races: `ThreadSanitizer` (`-fsanitize=thread`) não reporta erros
- [ ] Timestamps impressos com precisão ≤ 10 ms do evento real
- [ ] Bonus: processos + semáforos em vez de threads + mutexes

## Recursos

- [Especificação 42 School](https://projects.intra.42.fr/projects/philosophers)
- [man 3 pthread_mutex_lock](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html)
- [The Little Book of Semaphores](https://greenteapress.com/semaphores/LittleBookOfSemaphores.pdf)
