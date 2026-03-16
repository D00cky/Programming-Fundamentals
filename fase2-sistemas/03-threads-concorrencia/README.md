# 03 — Threads & Concorrência

> *"Threads are not a solution to a problem, but a source of new problems."*

---

## Antes de começar

Certifique-se de que você já:

- [ ] Implementou `fork()`, `exec()` e `waitpid()` e entende o ciclo de vida de processos
- [ ] Usou `pipe()` e `dup2()` para comunicação entre processos
- [ ] Entende a diferença entre espaço de endereçamento isolado (processos) e compartilhado (threads)
- [ ] Detectou file descriptor leaks com `/proc/self/fd`

---

## O que você vai aprender

Ao final deste módulo você será capaz de:

- Criar e sincronizar threads com `pthread_create`, `pthread_join`, `pthread_detach`
- Proteger dados compartilhados com mutex (`pthread_mutex_t`)
- Implementar produtor-consumidor com semáforos e variáveis de condição
- Detectar race conditions com ThreadSanitizer (`-fsanitize=thread`)
- Explicar as 4 condições de Coffman para deadlock
- Distinguir código thread-safe de reentrante

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

## Knowledge Check

Responda sem consultar o material. Se travar, releia a seção correspondente.

1. O que threads de um mesmo processo compartilham? O que é privado por thread?
2. O que é uma data race? Por que `contador++` não é atômico?
3. Qual a diferença entre mutex e semáforo?
4. Quais são as 4 condições de Coffman para que deadlock ocorra?
5. Por que `volatile` não é suficiente para substituir mutex em `contador++`?
6. Para que serve uma variável de condição (`pthread_cond_t`)? Quando usar no lugar de semáforo?
7. Qual a diferença entre código thread-safe e código reentrante?
8. O que `pthread_join` faz? E `pthread_detach`? Quando usar cada um?

---

## Projeto — Thread Pool

Implemente uma thread pool com N workers e uma fila de tarefas.

**Funcionalidades:**
- Criar N threads worker na inicialização
- Fila de tarefas protegida por mutex + variável de condição
- Workers dormem quando fila está vazia, acordam quando há tarefa
- `pool_submit(func, arg)` adiciona tarefa à fila (não bloqueia o chamador)
- Ao receber `SIGINT`, não aceitar novas tarefas e aguardar as em andamento terminarem

**Requisitos técnicos:**
- Sem busy-waiting: usar `pthread_cond_wait` para dormir
- Compilar com `gcc -Wall -Wextra -Werror -lpthread`
- Verificar com ThreadSanitizer: `gcc -fsanitize=thread`
- Zero memory leaks

**Exemplo de execução:**
```
$ ./threadpool 4
[pool] 4 workers iniciados
[pool] submetendo 20 tarefas...
[worker 0] executando tarefa 0
[worker 1] executando tarefa 1
[worker 2] executando tarefa 2
[worker 3] executando tarefa 3
...
^C
[pool] shutdown: aguardando 2 tarefas em andamento...
[pool] encerrado.
```

---

## Exercícios

**ex01-race:** demonstrar race condition; corrigir com mutex; verificar com ThreadSanitizer (`-fsanitize=thread`)

**ex02-filosofos:** implementar o jantar dos filósofos com 5 filósofos e 5 garfos; evitar deadlock e starvation

**ex03-produtor-consumidor:** buffer circular com N produtores e M consumidores

---

## Recursos Adicionais

Estes recursos são **opcionais** mas vão solidificar seu entendimento:

**Para ler/assistir agora:**
- **OSTEP** capítulos 26-32 (Concurrency) — gratuito em ostep.org, excelentes animações
- **The Little Book of Semaphores** — Allen Downey, gratuito em greenteapress.com

**Para consulta:**
- `man 7 pthreads` · `man 3 pthread_create` · `man 3 sem_init`
- **TLPI** capítulos 29-33 (threads) — referência completa com exemplos Linux

**Para ir além:**
- [ThreadSanitizer documentation](https://clang.llvm.org/docs/ThreadSanitizer.html) — detector de race conditions em runtime
- Paul McKenney — "Is Parallel Programming Hard?" (gratuito online) — análise profunda de concorrência
