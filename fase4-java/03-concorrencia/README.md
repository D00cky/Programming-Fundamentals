# 03 — Concorrência em Java

> *"Concurrency is not parallelism."* — Rob Pike

---

## Antes de começar

Certifique-se de que você já:

- [ ] Usou `ArrayList`, `HashMap` e Stream API (`fase4-java/02`)
- [ ] Entende o que é uma race condition e mutex (`fase2-sistemas/03`)
- [ ] Implementou produtor-consumidor com semáforos em C
- [ ] Conhece o conceito de thread-safety

---

## O que você vai aprender

Ao final deste módulo você será capaz de:

- Criar e sincronizar threads com `Thread`, `Runnable` e `ExecutorService`
- Usar `synchronized`, `ReentrantLock` e classes atômicas (`AtomicInteger`)
- Compor operações assíncronas com `CompletableFuture`
- Implementar produtor-consumidor com `BlockingQueue`
- Explicar a diferença entre `volatile` e `synchronized`
- Usar o framework `java.util.concurrent` para evitar primitivas de baixo nível

---

## 1. Thread

```java
// Forma 1: implementar Runnable (preferível)
Runnable tarefa = () -> {
    System.out.println("Thread: " + Thread.currentThread().getName());
};
Thread t = new Thread(tarefa, "minha-thread");
t.start();
t.join();   // espera terminar

// Forma 2: estender Thread (evitar)
class MinhaThread extends Thread {
    public void run() { /* ... */ }
}
```

---

## 2. Executor Framework

```java
import java.util.concurrent.*;

// Pool de 4 threads:
ExecutorService exec = Executors.newFixedThreadPool(4);

// Submeter tarefas:
exec.execute(() -> System.out.println("tarefa rápida"));

Future<Integer> futuro = exec.submit(() -> {
    Thread.sleep(1000);
    return 42;
});
int resultado = futuro.get();  // bloqueia até completar

// Finalizar:
exec.shutdown();                    // aguarda tarefas em andamento
exec.awaitTermination(5, TimeUnit.SECONDS);
exec.shutdownNow();                 // interrompe imediatamente

// Tipos de pool:
Executors.newSingleThreadExecutor()    // 1 thread, fila de tarefas
Executors.newCachedThreadPool()        // cresce conforme necessário
Executors.newScheduledThreadPool(2)    // agendamento periódico
```

---

## 3. synchronized e Locks

```java
// synchronized: lock implícito no objeto (ou classe)
class Contador {
    private int valor = 0;

    public synchronized void incrementa() { valor++; }
    public synchronized int getValor() { return valor; }
}

// ReentrantLock: mais flexível
import java.util.concurrent.locks.*;
ReentrantLock lock = new ReentrantLock();

lock.lock();
try {
    // seção crítica
} finally {
    lock.unlock();  // SEMPRE no finally para garantir liberação
}

// tryLock: não bloqueia
if (lock.tryLock(500, TimeUnit.MILLISECONDS)) {
    try { /* ... */ } finally { lock.unlock(); }
}
```

---

## 4. Classes Atômicas

```java
import java.util.concurrent.atomic.*;

AtomicInteger contador = new AtomicInteger(0);
contador.incrementAndGet();        // thread-safe, sem lock!
contador.addAndGet(5);
contador.compareAndSet(5, 10);     // CAS: muda de 5 para 10 atomicamente

AtomicLong, AtomicBoolean, AtomicReference<T>   // outros tipos atômicos
```

---

## 5. CompletableFuture (Java 8+)

Composição assíncrona sem callback hell:

```java
CompletableFuture<String> futuro = CompletableFuture
    .supplyAsync(() -> buscaDados())          // executa em pool
    .thenApply(dados -> processa(dados))      // transforma resultado
    .thenApply(String::toUpperCase)
    .exceptionally(ex -> "Erro: " + ex.getMessage());

String resultado = futuro.join();             // bloqueia e obtém resultado

// Combinar múltiplos futuros:
CompletableFuture<String> f1 = CompletableFuture.supplyAsync(() -> "hello");
CompletableFuture<String> f2 = CompletableFuture.supplyAsync(() -> "world");

CompletableFuture.allOf(f1, f2).join();      // espera todos
String combinado = f1.thenCombine(f2, (a, b) -> a + " " + b).join();
```

---

## 6. BlockingQueue — Produtor/Consumidor

```java
BlockingQueue<Integer> fila = new ArrayBlockingQueue<>(10);

// Produtor:
new Thread(() -> {
    for (int i = 0; i < 100; i++) {
        fila.put(i);    // bloqueia se fila cheia
    }
}).start();

// Consumidor:
new Thread(() -> {
    while (true) {
        int val = fila.take();   // bloqueia se fila vazia
        processa(val);
    }
}).start();
```

---

## 7. volatile e Visibilidade

```java
// volatile garante que o valor é sempre lido da memória principal (não de cache da CPU):
volatile boolean running = true;

void parar() { running = false; }

void loop() {
    while (running) { /* ... */ }  // sem volatile, pode nunca ver running=false
}

// volatile NÃO garante atomicidade para operações compostas (i++ não é atomic)
// Para isso: AtomicInteger ou synchronized
```

---

## Knowledge Check

Responda sem consultar o material. Se travar, releia a seção correspondente.

1. Qual a diferença entre `Thread.start()` e `Thread.run()`?
2. Por que `synchronized` num método de instância bloqueia `this` e não a classe?
3. Qual a diferença entre `AtomicInteger.incrementAndGet()` e `synchronized` para um contador?
4. Por que `volatile` não é suficiente para `contador++`?
5. O que `CompletableFuture.allOf` faz? Quando você o usaria?
6. Qual a diferença entre `ExecutorService.shutdown()` e `shutdownNow()`?
7. Por que `BlockingQueue.put()` bloqueia? O que acontece com a thread chamadora?
8. O que é starvation em concorrência? Como `ReentrantLock(fair=true)` ajuda?

---

## Projeto — Web Scraper Concorrente

Implemente um scraper que busca dados de N URLs em paralelo e agrega os resultados.

**Funcionalidades:**
- Receber lista de URLs de um arquivo
- Buscar cada URL em paralelo com `CompletableFuture`
- Extrair título da página (tag `<title>`)
- Limitar a 5 requisições simultâneas com `Executors.newFixedThreadPool(5)`
- Exibir resultados na ordem de conclusão; exibir erros sem abortar as demais

**Requisitos técnicos:**
- Usar `CompletableFuture.supplyAsync` com pool customizado
- Usar `ConcurrentHashMap` para acumular resultados de forma thread-safe
- Timeout de 5 segundos por URL
- Compilar e rodar com `java`

**Exemplo de execução:**
```
$ java Scraper urls.txt
[1/10] https://example.com → "Example Domain" (142ms)
[2/10] https://httpbin.org → "httpbin.org" (89ms)
[ERR]  https://invalid.url → Connection refused
...
=== Concluído: 9/10 sucessos em 1.2s ===
```

---

## Exercícios

**ex01:** thread pool manual: implementar `SimpleExecutor` com array de threads e BlockingQueue

**ex02:** web scraper concorrente: buscar N URLs em paralelo com CompletableFuture, coletar resultados

**ex03:** simulação de banco: 100 threads fazendo transferências simultâneas; garantir consistência com locks; detectar deadlock

---

## Recursos Adicionais

Estes recursos são **opcionais** mas vão solidificar seu entendimento:

**Para ler/assistir agora:**
- **Java Concurrency in Practice** — Brian Goetz et al. — a referência definitiva em concorrência Java
- [Oracle Concurrency Tutorial](https://docs.oracle.com/javase/tutorial/essential/concurrency/) — tutorial oficial

**Para consulta:**
- **Effective Java** — Joshua Bloch, items 78-84 (concorrência)
- [java.util.concurrent Javadoc](https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/concurrent/package-summary.html)

**Para ir além:**
- **The Art of Multiprocessor Programming** — Herlihy & Shavit — teoria e algoritmos lock-free
- Project Loom (Virtual Threads, Java 21) — concorrência com threads virtuais de baixo custo
