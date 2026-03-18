# Java Concurrent

> Fase 4 — Java

Produtor-consumidor, thread pool e fork-join framework.

## Critérios de conclusão

- [ ] Implementação produtor-consumidor com N produtores e M consumidores via `BlockingQueue`
- [ ] Thread pool configurável (tamanho fixo, cachedado, agendado) usando `ExecutorService`
- [ ] Tarefa paralela com `ForkJoinPool` (ex.: merge sort ou soma de array grande)
- [ ] Benchmark com JMH comparando implementação sequencial vs. paralela
- [ ] Sem race conditions: `ThreadSanitizer` ou análise manual com `FindBugs`/`SpotBugs`

## Recursos

- [Java Concurrency in Practice — Goetz et al.](https://jcip.net/)
- [JMH (Java Microbenchmark Harness)](https://openjdk.org/projects/code-tools/jmh/)
- [java.util.concurrent — Javadoc](https://docs.oracle.com/en/java/docs/api/java.base/java/util/concurrent/package-summary.html)
