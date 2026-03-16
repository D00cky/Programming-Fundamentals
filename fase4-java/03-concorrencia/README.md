# 03 — Concorrência em Java

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

## Exercícios

**ex01:** thread pool manual: implementar `SimpleExecutor` com array de threads e BlockingQueue

**ex02:** web scraper concorrente: buscar N URLs em paralelo com CompletableFuture, coletar resultados

**ex03:** simulação de banco: 100 threads fazendo transferências simultâneas; garantir consistência com locks; detectar deadlock

---

## Referências

- **Java Concurrency in Practice** — Goetz et al. · a referência definitiva
- **Effective Java** — capítulos sobre concorrência (items 78-84)
- **Oracle Concurrency Tutorial** — docs.oracle.com/javase/tutorial/essential/concurrency
