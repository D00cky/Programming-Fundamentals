# 06 — Compilação: De `.c` até Executável

> O que o gcc realmente faz quando você digita `gcc hello.c -o hello`?

---

## As 4 Etapas da Compilação

```
hello.c  →  [Pré-processador]  →  hello.i
         →  [Compilador]       →  hello.s  (assembly)
         →  [Assembler]        →  hello.o  (object file)
         →  [Linker]           →  hello    (executável)
```

### 1. Pré-processador (`cpp`)

Processa diretivas `#include`, `#define`, `#ifdef`, etc.

```c
// Antes do pré-processador:
#include <stdio.h>
#define MAX 100

int arr[MAX];
```

```c
// Depois do pré-processador (hello.i):
// ... conteúdo inteiro de stdio.h colado aqui (~800 linhas) ...

int arr[100];  // MAX substituído por 100
```

```bash
# Ver output do pré-processador
gcc -E hello.c -o hello.i
wc -l hello.i  # vai ter muito mais linhas que o original!
```

### 2. Compilador (`cc1`)

Converte C para **assembly** — linguagem de baixo nível específica para a arquitetura (x86-64, ARM, etc.).

```c
// C:
int soma(int a, int b) {
    return a + b;
}
```

```asm
; Assembly x86-64 gerado:
soma:
    push    rbp
    mov     rbp, rsp
    mov     DWORD PTR [rbp-4], edi   ; salva parâmetro a
    mov     DWORD PTR [rbp-8], esi   ; salva parâmetro b
    mov     eax, DWORD PTR [rbp-4]
    add     eax, DWORD PTR [rbp-8]  ; a + b
    pop     rbp
    ret
```

```bash
# Ver assembly gerado
gcc -S hello.c -o hello.s
cat hello.s
```

### 3. Assembler (`as`)

Converte assembly para **código de máquina** — bytes binários que a CPU executa.

```bash
# Ver o object file (binário, não é legível como texto)
gcc -c hello.c -o hello.o
objdump -d hello.o  # desassembla para ver as instruções
xxd hello.o | head  # ver os bytes em hex
```

### 4. Linker (`ld`)

**Une** vários object files e bibliotecas em um executável final.

```bash
# Compilar múltiplos arquivos separados e linkar
gcc -c main.c -o main.o
gcc -c utils.c -o utils.o
gcc main.o utils.o -o programa  # linker une os dois

# O gcc faz tudo de uma vez:
gcc main.c utils.c -o programa
```

O linker também resolve referências externas: quando seu código chama `printf`, o linker encontra a implementação na `libc` (biblioteca C padrão) e conecta.

---

## Bibliotecas

### Estática vs Dinâmica

**Estática** (`.a`): código da biblioteca é copiado para dentro do executável.
```bash
ar rcs libminha.a utils.o    # cria biblioteca estática
gcc main.c -L. -lminha -o programa  # linka estaticamente
```

**Dinâmica/Compartilhada** (`.so`): código fica separado, carregado em tempo de execução.
```bash
gcc -shared -fPIC utils.c -o libminha.so  # cria biblioteca dinâmica
gcc main.c -L. -lminha -o programa        # linka dinamicamente
```

```bash
# Ver quais libs dinâmicas um executável usa
ldd ./programa
ldd /bin/ls
```

### A libc

Toda vez que você usa `printf`, `malloc`, `strlen` — você está usando a **libc** (biblioteca C padrão). O gcc linka com ela automaticamente.

```bash
# -lm é necessário para funções matemáticas (math.h)
gcc programa.c -lm -o programa
```

---

## Flags de Compilação Importantes

```bash
# Flags de warning (obrigatórias neste projeto)
-Wall        # ativa a maioria dos warnings
-Wextra      # warnings extras
-Werror      # transforma warnings em erros (não compila se tiver warning)

# Otimização
-O0          # sem otimização (padrão, melhor para debug)
-O2          # otimização moderada
-O3          # otimização agressiva

# Debug
-g           # inclui informação de debug (para usar GDB)
-g3          # mais informação de debug (inclui macros)
-fsanitize=address  # AddressSanitizer: detecta memory errors em runtime

# Padrão C
-std=c99     # usa padrão C99
-std=c11     # usa padrão C11

# Exemplo completo para debug:
gcc -Wall -Wextra -Werror -g -fsanitize=address hello.c -o hello
```

---

## O Makefile

Em vez de digitar o comando gcc toda vez, use um Makefile:

```makefile
# Makefile básico
CC      = gcc
CFLAGS  = -Wall -Wextra -Werror
NAME    = programa

SRCS    = main.c utils.c
OBJS    = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
```

```bash
make          # compila (roda target "all")
make clean    # remove object files
make fclean   # remove tudo
make re       # recompila do zero
```

---

## Exercícios

1. Crie um `hello.c` simples e veja cada etapa:
   ```bash
   gcc -E hello.c -o hello.i && wc -l hello.i
   gcc -S hello.c -o hello.s && cat hello.s
   gcc -c hello.c -o hello.o && ls -la hello.o
   gcc hello.o -o hello && ./hello
   ```

2. Compile com `-Wall -Wextra` e veja os warnings que aparecem neste código:
   ```c
   #include <stdio.h>
   int main() {
       int x;
       printf("%d\n", x);
   }
   ```

3. Use `ldd` para ver as dependências de alguns programas do sistema:
   ```bash
   ldd /bin/ls
   ldd /bin/bash
   ```

4. Crie um Makefile para um projeto com `main.c` e `utils.c`

---

## Referências

- **GCC docs** — gcc.gnu.org/onlinedocs
- **Makefile Tutorial** — makefiletutorial.com
- **CS50x Week 2** — compilação (cs50.harvard.edu)
- `man gcc` — documentação completa das flags
- **Godbolt Compiler Explorer** — godbolt.org (veja o assembly gerado em tempo real)
