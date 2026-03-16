# 02 — File Descriptors & I/O

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

## Exercícios

**ex01-rw:** abrir arquivo, ler conteúdo, imprimir no stdout usando só `read`/`write` (sem printf/fgets)

**ex02-redirection:** implementar `>` (output redirect) e `<` (input redirect) usando `dup2`

**ex03-pipe-fork:** implementar `cmd1 | cmd2` genérico com `fork` + `pipe` + `dup2` + `exec`

---

## Referências

- `man 2 open` · `man 2 read` · `man 2 write` · `man 2 dup2` · `man 2 pipe`
- **TLPI** capítulos 4-5 (I/O), 44 (pipes), 63 (I/O multiplexing)
- **APUE** capítulos 3, 8, 15
