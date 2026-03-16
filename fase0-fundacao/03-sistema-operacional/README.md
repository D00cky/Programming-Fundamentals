# 03 — Sistema Operacional

## O que é um SO?

O Sistema Operacional é um programa que gerencia o hardware e fornece serviços para outros programas. Ele fica entre o hardware e os seus programas:

```
┌─────────────────────────────────────┐
│         Seus programas (C, etc.)    │
├─────────────────────────────────────┤
│         Sistema Operacional         │
│  (kernel + drivers + syscalls)      │
├─────────────────────────────────────┤
│         Hardware                    │
│  (CPU, RAM, disco, rede, etc.)      │
└─────────────────────────────────────┘
```

Quando seu programa C faz `printf("hello")`, ele não fala direto com a tela. Ele pede ao SO que escreva na tela — isso é uma **syscall** (chamada de sistema).

---

## Processos

Um **processo** é um programa em execução. O SO gerencia múltiplos processos ao mesmo tempo (multitasking).

Cada processo tem:
- **PID** (Process ID): número único
- **Espaço de memória próprio** (isolado dos outros)
- **Descritores de arquivo** (arquivos abertos)
- **Estado**: running, sleeping, stopped, zombie

```bash
# Ver processos no terminal
ps aux
top
htop  # mais visual
```

### O que um processo enxerga na memória?

```
Endereço alto
┌──────────────┐
│    Stack     │  ← variáveis locais, chamadas de função (cresce pra baixo)
│      ↓       │
│              │
│      ↑       │
│     Heap     │  ← malloc() (cresce pra cima)
├──────────────┤
│     BSS      │  ← variáveis globais não inicializadas
├──────────────┤
│    Data      │  ← variáveis globais inicializadas
├──────────────┤
│    Text      │  ← código do programa (instruções)
└──────────────┘
Endereço baixo
```

Isso é o **layout de memória de um processo**. Em C você vai interagir com todas essas regiões.

---

## Memória Virtual

Cada processo acha que tem a RAM toda para si — mas é uma ilusão criada pelo SO.

O SO usa **paginação**: divide a memória em páginas (normalmente 4KB) e mapeia endereços virtuais para endereços físicos reais via uma **tabela de páginas**.

**Por que isso importa em C?**
```c
int *p1 = malloc(10);
int *p2 = malloc(10);
// p1 e p2 têm endereços virtuais diferentes
// mas fisicamente podem estar em lugares diferentes da RAM
// dois processos podem ter o mesmo endereço virtual
// mas apontam para locais físicos completamente diferentes
```

---

## Filesystem

O filesystem organiza dados em disco como uma árvore de diretórios e arquivos.

```
/                    ← raiz (root)
├── bin/             ← binários essenciais (ls, cp, etc.)
├── etc/             ← configurações do sistema
├── home/
│   └── d00cky/      ← seu diretório home (~)
│       └── Projects/
├── usr/
│   ├── bin/         ← programas do usuário
│   └── include/     ← headers C (stdio.h, etc.)
├── var/             ← logs, dados variáveis
└── tmp/             ← arquivos temporários
```

### File Descriptors

Em Linux/Unix, **tudo é arquivo** — arquivos, dispositivos, pipes, sockets.

Todo processo nasce com 3 descritores de arquivo abertos:

| FD | Nome   | Padrão |
|----|--------|--------|
| 0  | stdin  | teclado |
| 1  | stdout | terminal |
| 2  | stderr | terminal (erros) |

Quando você faz `printf()` em C, ele escreve no fd 1 (stdout).

---

## Syscalls

Syscalls são a interface entre seu programa e o kernel. Exemplos:

| Syscall | O que faz | Chamada C |
|---------|-----------|-----------|
| `read`  | lê de um fd | `read(fd, buf, n)` |
| `write` | escreve em um fd | `write(fd, buf, n)` |
| `open`  | abre um arquivo | `open(path, flags)` |
| `close` | fecha um fd | `close(fd)` |
| `fork`  | cria um processo filho | `fork()` |
| `exec`  | substitui o processo por outro programa | `execve(...)` |
| `exit`  | encerra o processo | `exit(code)` |
| `mmap`  | mapeia memória | `malloc()` usa isso internamente |

```c
// printf("hello") em C acaba virando algo como:
write(1, "hello", 5);  // escreve "hello" no fd 1 (stdout)
```

---

## Permissões de Arquivo

Linux usa um sistema de permissões por usuário/grupo/outros:

```bash
ls -la
# -rw-r--r-- 1 d00cky d00cky 1234 Jan 1 arquivo.c
#  ↑↑↑↑↑↑↑↑↑
#  │││└──┴──→ outros (r--)
#  │└──┴──→ grupo (r--)
#  └──┴──→ dono (rw-)
# r = read, w = write, x = execute
```

```bash
chmod 755 meu_programa  # dono: rwx, grupo: r-x, outros: r-x
chmod +x script.sh      # adiciona permissão de execução
```

---

## Exercícios Práticos

```bash
# 1. Explore os processos do seu sistema
ps aux | head -20
ps aux | grep bash

# 2. Veja o PID do seu shell atual
echo $$

# 3. Explore o filesystem
ls /
ls /usr/include  # onde ficam os headers C (stdio.h, etc.)
cat /usr/include/stdio.h | head -50

# 4. File descriptors na prática
# Redirecionar stdout para um arquivo (fd 1 → arquivo)
echo "hello" > /tmp/teste.txt
cat /tmp/teste.txt

# Redirecionar stderr (fd 2)
ls /diretorio_que_nao_existe 2> /tmp/erro.txt
cat /tmp/erro.txt

# 5. Veja o mapa de memória de um processo
cat /proc/$$/maps | head -20
```

---

## Referências

- **The Linux Programming Interface** — Michael Kerrisk (a bíblia de Linux/Unix)
- **Operating Systems: Three Easy Pieces** — Arpaci-Dusseau (gratuito: ostep.org) ← **muito recomendado**
- **CS50x Week 4** — memória (cs50.harvard.edu)
- **Akitando** — episódios sobre Linux e sistemas
- `man proc` — documentação do filesystem /proc no Linux
