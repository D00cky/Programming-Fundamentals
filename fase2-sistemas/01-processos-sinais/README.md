# 01 — Processos & Sinais

---

## 1. O que é um Processo?

Um processo é uma instância de um programa em execução. Cada processo tem:
- PID (Process ID) único
- Espaço de endereçamento próprio (memória virtual isolada)
- Descritores de arquivo (stdin=0, stdout=1, stderr=2, + outros)
- Estado (running, sleeping, zombie, stopped)

```bash
ps aux          # lista todos os processos
pstree          # árvore de processos
cat /proc/$$    # informações do processo atual (Linux)
```

---

## 2. fork()

`fork()` cria um processo filho idêntico ao pai. Retorna:
- **Pai:** PID do filho (> 0)
- **Filho:** 0
- **Erro:** -1

```c
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main(void) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }
    if (pid == 0) {
        // Código do FILHO
        printf("Filho: meu PID=%d, pai=%d\n", getpid(), getppid());
        return 0;
    }
    // Código do PAI
    printf("Pai: meu PID=%d, filho=%d\n", getpid(), pid);
    wait(NULL);   // espera filho terminar (evita zombie)
    return 0;
}
```

> **Zombie:** processo filho que terminou mas o pai não fez `wait()`. O PCB permanece na tabela de processos.

---

## 3. exec()

`exec()` substitui a imagem do processo atual por outro programa. O PID permanece o mesmo, mas o código, dados e stack são substituídos.

```c
#include <unistd.h>

// execve: a syscall base
execve("/bin/ls", (char*[]){"ls", "-la", NULL}, environ);
// Se execve retornar, deu erro (o programa não foi encontrado, etc.)
perror("execve");

// Variantes convenientes:
execvp("ls", (char*[]){"ls", "-la", NULL});  // busca no PATH
execlp("echo", "echo", "hello", NULL);        // argumentos como lista
```

**Padrão fork-exec:**
```c
pid_t pid = fork();
if (pid == 0) {
    execvp(cmd, args);    // filho vira outro programa
    perror("execvp");     // só chega aqui se falhou
    exit(1);
}
waitpid(pid, &status, 0);  // pai espera
```

---

## 4. wait() e waitpid()

```c
#include <sys/wait.h>

int status;
pid_t filho = wait(&status);               // espera qualquer filho

pid_t filho = waitpid(pid, &status, 0);   // espera filho específico
pid_t filho = waitpid(-1, &status, WNOHANG); // não bloqueia

// Inspecionar status:
if (WIFEXITED(status))
    printf("saiu com código %d\n", WEXITSTATUS(status));
if (WIFSIGNALED(status))
    printf("morreu por sinal %d\n", WTERMSIG(status));
```

---

## 5. Signals

Sinais são notificações assíncronas enviadas para processos.

```c
#include <signal.h>

// Sinais importantes:
// SIGINT  (2)  — Ctrl+C
// SIGTERM (15) — kill padrão
// SIGKILL (9)  — não pode ser ignorado nem capturado
// SIGCHLD (17) — filho terminou
// SIGUSR1 (10), SIGUSR2 (12) — definidos pelo usuário
// SIGPIPE (13) — escrita em pipe sem leitores

// Enviar sinal:
kill(pid, SIGTERM);   // envia SIGTERM para pid
kill(0, SIGTERM);     // envia para todos no grupo de processos

// Instalar handler:
void handler(int sig) {
    write(1, "Recebi SIGINT!\n", 15);   // write é async-signal-safe; printf NÃO é!
}

signal(SIGINT, handler);      // forma simples (não recomendada em produção)

// Forma robusta com sigaction:
struct sigaction sa = {0};
sa.sa_handler = handler;
sigemptyset(&sa.sa_mask);
sa.sa_flags = SA_RESTART;
sigaction(SIGINT, &sa, NULL);

// Ignorar sinal:
signal(SIGPIPE, SIG_IGN);

// Restaurar comportamento padrão:
signal(SIGINT, SIG_DFL);
```

> **Regra:** dentro de signal handlers, use apenas funções async-signal-safe: `write()`, `_exit()`, `kill()`. Nunca `printf()`, `malloc()`, `free()`.

---

## 6. getenv() e Variáveis de Ambiente

```c
#include <stdlib.h>

char *path = getenv("PATH");
char *home = getenv("HOME");
if (!path) printf("PATH não definido\n");

// O terceiro argumento de execve é o ambiente:
extern char **environ;   // ambiente global
execve(prog, args, environ);
```

---

## Exercícios

```bash
# Criar ex01-fork/, ex02-exec/, ex03-signals/ cada um com main.c
```

**ex01-fork:** fork(), pai espera filho, filho e pai imprimem algo diferente

**ex02-exec:** fork-exec para rodar `ls`, `cat`, `echo` com argumentos dinâmicos

**ex03-signals:** instalar handler para SIGINT (Ctrl+C); contar quantas vezes recebeu; sair com SIGQUIT

---

## Referências

- `man 2 fork` · `man 2 execve` · `man 2 waitpid` · `man 2 kill` · `man 2 sigaction`
- **TLPI** capítulos 24-26 (processos), 20-22 (signals)
- **OSTEP** capítulo 5 (Process API)
