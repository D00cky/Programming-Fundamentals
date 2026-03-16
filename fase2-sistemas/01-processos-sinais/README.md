# 01 — Processos & Sinais

> *"Unix is not just an operating system, it's a way of thinking."*

---

## Antes de começar

Certifique-se de que você já:

- [ ] Escreve e compila código C com `gcc` e usa ponteiros (`fase1-c/`)
- [ ] Sabe navegar o terminal e ler páginas `man`
- [ ] Criou e usou Makefiles com targets `all`, `clean`, `fclean`
- [ ] Entende entrada/saída padrão e redirecionamento básico no shell

---

## O que você vai aprender

Ao final deste módulo você será capaz de:

- Descrever o ciclo de vida de um processo (criação, execução, término)
- Explicar os valores de retorno de `fork()` e o que acontece em pai e filho
- Implementar o padrão fork-exec-waitpid para rodar programas externos
- Instalar signal handlers robustos com `sigaction`
- Comunicar processos através de pipes anônimos
- Explicar o que é um processo zombie e como evitá-lo

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

## Knowledge Check

Responda sem consultar o material. Se travar, releia a seção correspondente.

1. O que `fork()` retorna no processo pai? E no filho? E em caso de erro?
2. Qual a diferença entre `fork()` e `exec()`? O que acontece com o PID em cada um?
3. O que é um processo zombie? Como ele surge e como evitá-lo?
4. Por que `SIGCHLD` é importante para um servidor que cria muitos filhos?
5. Por que `printf()` não pode ser usado dentro de um signal handler?
6. Qual a diferença entre `wait()` e `waitpid()`? Quando você usaria cada um?
7. O que acontece se `execvp()` falhar (programa não encontrado)?
8. O que é o "padrão fork-exec"? Por que ele é usado em vez de execvp direto?
9. Como `kill(0, SIGTERM)` difere de `kill(pid, SIGTERM)`?
10. O que `WIFEXITED(status)` e `WEXITSTATUS(status)` retornam?

---

## Projeto — Supervisor de Processos

Implemente um supervisor que gerencia N worker processes e os reinicia se morrerem.

**Funcionalidades:**
- Criar N workers com `fork()` que executam uma tarefa em loop
- Supervisor monitora workers com `waitpid(WNOHANG)` em loop
- Se um worker morrer (por qualquer razão), reiniciá-lo automaticamente
- Ao receber `SIGINT`, enviar `SIGTERM` para todos os workers e aguardar encerramento gracioso

**Requisitos técnicos:**
- Usar `sigaction` para instalar handlers (não `signal`)
- Workers encerram limpo ao receber `SIGTERM`
- Compilar com `gcc -Wall -Wextra -Werror`
- Sem zombie processes

**Exemplo de execução:**
```
$ ./supervisor 3
[supervisor] Iniciando 3 workers...
[worker 1234] rodando...
[worker 1235] rodando...
[worker 1236] rodando...
^C
[supervisor] SIGINT recebido. Encerrando workers...
[worker 1234] encerrado
[worker 1235] encerrado
[worker 1236] encerrado
[supervisor] Todos os workers encerrados.
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

## Recursos Adicionais

Estes recursos são **opcionais** mas vão solidificar seu entendimento:

**Para ler/assistir agora:**
- **OSTEP** capítulo 5 (Process API) — gratuito em ostep.org, cobre fork/exec com exemplos
- **TLPI** capítulos 24-26 (processos) — a referência definitiva em Linux

**Para consulta:**
- `man 2 fork` · `man 2 execve` · `man 2 waitpid` · `man 2 kill` · `man 2 sigaction`
- **TLPI** capítulos 20-22 (signals) — lista completa de sinais e comportamentos

**Para ir além:**
- **APUE** (Advanced Programming in the Unix Environment) — Stephens & Rago
- [LWN.net](https://lwn.net) — artigos técnicos sobre syscalls e comportamentos do kernel Linux
