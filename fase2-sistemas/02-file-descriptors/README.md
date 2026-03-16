# 02 — File Descriptors & I/O

> *"In Unix, everything is a file."*

---

## Antes de começar

Certifique-se de que você já:

- [ ] Implementou `fork()`, `exec()` e `waitpid()` (`fase2-sistemas/01`)
- [ ] Entende o que são processos zombie e sabe evitá-los
- [ ] Usou `pipe()` e sabe que fechar as pontas certas é essencial
- [ ] Diagnosticou file descriptor leaks com `/proc/self/fd`

---

## O que você vai aprender

Ao final deste módulo você será capaz de:

- Usar `open`, `read`, `write`, `close` para I/O sem bufferização
- Redirecionar stdin/stdout de um processo com `dup2` antes de `execve`
- Encadear N comandos em pipeline usando `pipe()` + `fork()` + `dup2()`
- Explicar a tabela de file descriptors por processo e herança em `fork()`
- Usar `lseek` para navegação em arquivos e inspecionar FDs com `/proc/PID/fd`

---

## 1. File Descriptors

Em Unix, **tudo é arquivo**: arquivos regulares, diretórios, pipes, sockets, dispositivos. Um *file descriptor* (fd) é um inteiro que referencia um arquivo aberto na tabela do kernel.

```
FD 0 → stdin  (teclado por padrão)
FD 1 → stdout (terminal por padrão)
FD 2 → stderr (terminal por padrão)
FD 3 → primeiro fd livre após open()
...
```

---

## 2. open / read / write / close

```c
#include <fcntl.h>
#include <unistd.h>

// open: retorna fd ou -1 em erro
int fd = open("arquivo.txt", O_RDONLY);
int fd = open("saida.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
int fd = open("log.txt",   O_WRONLY | O_CREAT | O_APPEND, 0644);

// Flags comuns:
// O_RDONLY  — somente leitura
// O_WRONLY  — somente escrita
// O_RDWR    — leitura e escrita
// O_CREAT   — cria se não existe (precisa de mode)
// O_TRUNC   — trunca para tamanho 0
// O_APPEND  — escreve sempre no final

// read: retorna bytes lidos, 0 = EOF, -1 = erro
char buf[4096];
ssize_t n = read(fd, buf, sizeof(buf));

// write: retorna bytes escritos, -1 = erro
write(fd, "hello\n", 6);

// close: libera o fd
close(fd);
```

---

## 3. dup e dup2

`dup2(oldfd, newfd)` faz `newfd` apontar para o mesmo arquivo que `oldfd`. Se `newfd` estava aberto, é fechado primeiro.

**Uso principal:** redirecionar stdin/stdout antes de `execve`.

```c
// Redirecionar stdout para um arquivo:
int fd = open("saida.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
dup2(fd, STDOUT_FILENO);   // fd 1 agora aponta para "saida.txt"
close(fd);                 // fechar fd original (ainda temos via fd 1)
// Qualquer printf() agora escreve em "saida.txt"
```

---

## 4. pipe()

Cria um par de fds: `pipefd[0]` para leitura, `pipefd[1]` para escrita. Dados escritos em `[1]` são lidos em `[0]`.

```c
int pipefd[2];
pipe(pipefd);  // pipefd[0]=leitura, pipefd[1]=escrita

pid_t pid = fork();
if (pid == 0) {
    // FILHO: escreve no pipe
    close(pipefd[0]);                    // fecha lado de leitura
    write(pipefd[1], "mensagem", 8);
    close(pipefd[1]);
    exit(0);
}
// PAI: lê do pipe
close(pipefd[1]);                        // fecha lado de escrita
char buf[32];
int n = read(pipefd[0], buf, sizeof(buf));
buf[n] = '\0';
printf("Pai recebeu: %s\n", buf);
close(pipefd[0]);
wait(NULL);
```

**Regra:** sempre fechar as pontas do pipe que não são usadas. Sem isso, `read()` nunca retorna EOF.

---

## 5. Implementando `cmd1 | cmd2`

```c
// Equivalente a: ls -la | grep main

int pipefd[2];
pipe(pipefd);

// Filho 1: ls -la → escreve no pipe
pid_t p1 = fork();
if (p1 == 0) {
    dup2(pipefd[1], STDOUT_FILENO);  // stdout → pipe write
    close(pipefd[0]); close(pipefd[1]);
    execlp("ls", "ls", "-la", NULL);
}

// Filho 2: grep main → lê do pipe
pid_t p2 = fork();
if (p2 == 0) {
    dup2(pipefd[0], STDIN_FILENO);   // stdin ← pipe read
    close(pipefd[0]); close(pipefd[1]);
    execlp("grep", "grep", "main", NULL);
}

close(pipefd[0]); close(pipefd[1]);  // pai fecha os dois
waitpid(p1, NULL, 0);
waitpid(p2, NULL, 0);
```

---

## 6. lseek e /proc

```c
// Mover posição de leitura:
lseek(fd, 0, SEEK_SET);   // volta ao início
lseek(fd, 0, SEEK_END);   // vai para o fim
off_t pos = lseek(fd, 0, SEEK_CUR);  // posição atual

// Tamanho do arquivo:
off_t tamanho = lseek(fd, 0, SEEK_END);
lseek(fd, 0, SEEK_SET);

// /proc: informações de processos em tempo real
// /proc/self/fd/    → fds do processo atual
// /proc/self/maps   → mapa de memória virtual
// /proc/<pid>/status → estado do processo
```

---

## Knowledge Check

Responda sem consultar o material. Se travar, releia a seção correspondente.

1. Quais são os FDs 0, 1 e 2 por convenção? O que acontece se você fechar o FD 0?
2. Qual a diferença entre `open()` da syscall e `fopen()` da libc?
3. O que `dup2(4, 1)` faz exatamente? O que acontece com o FD 1 original?
4. Por que é obrigatório fechar as pontas não usadas de um pipe?
5. Qual a diferença entre `O_TRUNC` e `O_APPEND`?
6. Você pode usar `lseek()` em um pipe? Por que?
7. Como você inspeciona os FDs abertos de um processo em execução no Linux?
8. Quando um processo filho é criado com `fork()`, o que acontece com os FDs do pai?

---

## Projeto — Mini Shell Pipeline

Implemente um mini shell capaz de executar pipelines de N comandos.

**Funcionalidades:**
- Ler linha do stdin e parsear `cmd1 | cmd2 | cmd3 | ...`
- Criar N-1 pipes e N processos filhos
- Conectar stdout de cada processo ao stdin do próximo via `dup2`
- Aguardar todos os processos terminarem
- Tratar erros: comando não encontrado, `fork()` falhou, `pipe()` falhou

**Requisitos técnicos:**
- Fechar todos os FDs não usados em cada processo filho
- Compilar com `gcc -Wall -Wextra -Werror`
- Sem file descriptor leaks (verificar com `/proc/self/fd`)
- Sem zombie processes

**Exemplo de execução:**
```
$ ./minishell
minish> ls -la | grep main | wc -l
3
minish> cat /etc/passwd | grep root | cut -d: -f1
root
minish> exit
```

---

## Exercícios

**ex01-rw:** abrir arquivo, ler conteúdo, imprimir no stdout usando só `read`/`write` (sem printf/fgets)

**ex02-redirection:** implementar `>` (output redirect) e `<` (input redirect) usando `dup2`

**ex03-pipe-fork:** implementar `cmd1 | cmd2` genérico com `fork` + `pipe` + `dup2` + `exec`

---

## Recursos Adicionais

Estes recursos são **opcionais** mas vão solidificar seu entendimento:

**Para ler/assistir agora:**
- **TLPI** capítulos 4-5 (File I/O) — a referência em Linux para `open`/`read`/`write`/`dup2`
- [Julia Evans' zines](https://wizardzines.com) — explicações visuais de pipes e FDs

**Para consulta:**
- `man 2 open` · `man 2 read` · `man 2 write` · `man 2 dup2` · `man 2 pipe`
- **APUE** capítulos 3, 8, 15 — Advanced Programming in the Unix Environment

**Para ir além:**
- **TLPI** capítulo 63 (I/O Multiplexing: select, poll, epoll) — para servidores de alta performance
- [The Art of Unix Programming](http://www.catb.org/esr/writings/taoup/) — filosofia e design de Unix pipes
