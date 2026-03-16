# 03 — Threads & Concorrência

> *"Threads are not a solution to a problem, but a source of new problems."*

---

## 1. Thread vs Processo

| | Processo | Thread |
|-|----------|--------|
| Memória | Própria (isolada) | Compartilhada com outras threads do mesmo processo |
| Criação | Pesada (fork) | Leve |
| Comunicação | IPC (pipe, socket, shm) | Variáveis compartilhadas direto |
| Falha isolada? | Sim | Não — uma thread pode derrubar o processo todo |

---

## 2. pthreads — Criando Threads

```c
#include <pthread.h>

// Compile: gcc ... -lpthread

void *thread_func(void *arg) {
    int *n = (int *)arg;
    printf("Thread recebeu: %d\n", *n);
    return NULL;     // ou: pthread_exit(NULL);
}

int main(void) {
    pthread_t tid;
    int valor = 42;

    pthread_create(&tid, NULL, thread_func, &valor);
    // &valor é passado como void* para thread_func

    pthread_join(tid, NULL);  // espera a thread terminar
    return 0;
}
```

---

## 3. Race Condition

Quando múltiplas threads acessam e modificam dados compartilhados sem sincronização:

```c
// PROBLEMA: incremento não é atômico
// Internamente: LOAD x, ADD 1, STORE x — 3 passos
// Duas threads podem intercalar essas operações

int contador = 0;  // global compartilhado

void *incrementa(void *arg) {
    for (int i = 0; i < 1000000; i++)
        contador++;   // DATA RACE!
    return NULL;
}

// Resultado: diferente a cada execução, raramente 2000000
```

---

## 4. Mutex

Mutual Exclusion — garante que só uma thread executa a seção crítica por vez.

```c
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *incrementa_seguro(void *arg) {
    for (int i = 0; i < 1000000; i++) {
        pthread_mutex_lock(&lock);
        contador++;                     // seção crítica
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

// Destruir ao final:
pthread_mutex_destroy(&lock);
```

> **Regra de ouro:** lock antes de acessar dados compartilhados, unlock imediatamente depois. Quanto menor a seção crítica, melhor a performance.

---

## 5. Deadlock

Ocorre quando duas threads esperam por recursos que a outra segura:

```c
// Thread A:
pthread_mutex_lock(&mutex_A);
pthread_mutex_lock(&mutex_B);   // espera B

// Thread B (simultânea):
pthread_mutex_lock(&mutex_B);
pthread_mutex_lock(&mutex_A);   // espera A → deadlock!
```

**Soluções:**
1. Sempre adquirir locks na mesma ordem
2. Usar `pthread_mutex_trylock()` com timeout
3. Hierarquia de locks

---

## 6. Semáforos

Mais flexíveis que mutex: permitem N threads simultâneas (mutex = semáforo com N=1).

```c
#include <semaphore.h>

sem_t sem;
sem_init(&sem, 0, 5);   // inicializa com valor 5 (5 slots)

// Decrementar (wait): bloqueia se valor == 0
sem_wait(&sem);
// ... seção crítica com no máximo 5 threads simultâneas ...
sem_post(&sem);   // incrementar (signal)

sem_destroy(&sem);
```

---

## 7. Problema Produtor-Consumidor

```c
// Buffer circular com mutex + semáforos
#define CAP 10

int buffer[CAP];
int head = 0, tail = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
sem_t items;    // quantos itens disponíveis para consumir
sem_t spaces;   // quantos espaços disponíveis para produzir

void *produtor(void *arg) {
    for (int i = 0; i < 100; i++) {
        sem_wait(&spaces);
        pthread_mutex_lock(&lock);
        buffer[tail] = i;
        tail = (tail + 1) % CAP;
        pthread_mutex_unlock(&lock);
        sem_post(&items);
    }
    return NULL;
}

void *consumidor(void *arg) {
    for (int i = 0; i < 100; i++) {
        sem_wait(&items);
        pthread_mutex_lock(&lock);
        int val = buffer[head];
        head = (head + 1) % CAP;
        pthread_mutex_unlock(&lock);
        sem_post(&spaces);
        printf("Consumiu: %d\n", val);
    }
    return NULL;
}
```

---

## 8. pthread_mutex_t inicialização dinâmica

```c
pthread_mutex_t *lock = malloc(sizeof(pthread_mutex_t));
pthread_mutex_init(lock, NULL);
// ...
pthread_mutex_destroy(lock);
free(lock);
```

---

## Exercícios

**ex01-race:** demonstrar race condition; corrigir com mutex; verificar com ThreadSanitizer (`-fsanitize=thread`)

**ex02-filosofos:** implementar o jantar dos filósofos com 5 filósofos e 5 garfos; evitar deadlock e starvation

**ex03-produtor-consumidor:** buffer circular com N produtores e M consumidores

---

## Referências

- `man 7 pthreads` · `man 3 pthread_create` · `man 3 sem_init`
- **TLPI** capítulos 29-33 (threads)
- **OSTEP** capítulos 26-32 (concorrência)
- **The Little Book of Semaphores** — Allen Downey (gratuito: greenteapress.com)
