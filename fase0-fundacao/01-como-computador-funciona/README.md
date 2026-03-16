# 01 — Como o Computador Funciona

## O que você precisa saber

Um computador moderno tem 4 componentes fundamentais:

```
┌─────────────────────────────────────────────────┐
│                     CPU                         │
│  ┌──────────────┐    ┌────────────────────────┐ │
│  │  Registradores│    │  ALU (Unidade Aritmét.)│ │
│  │  (memória     │◄──►│  + FPU + CU            │ │
│  │   ultra-rápida│    │                        │ │
│  └──────────────┘    └────────────────────────┘ │
└───────────────────┬─────────────────────────────┘
                    │ barramento
        ┌───────────┴────────────┐
        │                        │
   ┌────▼─────┐          ┌───────▼──────┐
   │   RAM    │          │  Disco/SSD   │
   │ (volátil)│          │ (persistente)│
   └──────────┘          └──────────────┘
```

### CPU (Processador)

A CPU executa instruções. Ela tem:

- **Registradores**: memória dentro da CPU, extremamente rápida. Poucos bytes (ex: `rax`, `rbx`, `rsp` em x86-64). É onde os cálculos realmente acontecem.
- **ALU** (Arithmetic Logic Unit): faz operações matemáticas (soma, subtração, AND, OR...).
- **CU** (Control Unit): decodifica e coordena a execução de cada instrução.
- **Cache** (L1/L2/L3): memória rápida entre CPU e RAM para reduzir latência.

### Ciclo Fetch-Decode-Execute

Tudo que a CPU faz se repete nesse ciclo, bilhões de vezes por segundo:

```
1. FETCH    → busca a próxima instrução na memória (endereço no PC/IP)
2. DECODE   → interpreta o que a instrução significa
3. EXECUTE  → executa (soma, move dado, pula para outro endereço...)
4. WRITEBACK→ salva o resultado (em registrador ou memória)
```

O **Program Counter (PC)** ou **Instruction Pointer (IP)** é um registrador que guarda o endereço da próxima instrução.

### RAM (Memória Principal)

- Array gigante de bytes endereçáveis: cada byte tem um endereço (0, 1, 2, 3...)
- **Volátil**: apaga quando desliga
- Muito mais lenta que registradores, muito mais rápida que disco

```
Endereço:  0x00  0x01  0x02  0x03  0x04 ...
Valor:     0xFF  0x41  0x00  0x1A  0x7F ...
```

Quando você escreve `int x = 42;` em C, o valor `42` vai parar em algum endereço da RAM.

### Disco / SSD

- **Não-volátil**: persiste sem energia
- Muito mais lento que RAM
- Onde ficam seus arquivos, programas, o sistema operacional

### Hierarquia de Memória (velocidade vs capacidade)

```
Registradores  →  ~1 ciclo     ~kb      (dentro da CPU)
Cache L1/L2/L3 →  2-50 ciclos  kb/mb    (dentro/próximo da CPU)
RAM            →  ~100 ciclos  gb       (na placa-mãe)
SSD            →  ~100k ciclos tb       (armazenamento)
HDD            →  ~1M ciclos   tb       (armazenamento lento)
```

**Quanto mais longe da CPU, mais lento e maior.**

---

## Por que isso importa para C?

Em C você vai manipular memória diretamente:
- `int x` → aloca espaço na RAM (ou registrador, o compilador decide)
- `&x` → o endereço de memória de `x`
- `*ptr` → acessa o valor no endereço guardado em `ptr`
- `malloc(100)` → pede 100 bytes de RAM ao sistema operacional

Entender que a RAM é um array de bytes endereçados é fundamental para entender ponteiros.

---

## Exercícios de Reflexão

1. Se a RAM é um array de bytes, e um `int` tem 4 bytes, quantos endereços ele ocupa?
2. Por que registradores são mais rápidos que a RAM?
3. O que acontece com os valores na RAM quando você desliga o computador?
4. Por que programas precisam ser carregados na RAM para executar, se estão no disco?

---

## Referências

- **Akitando #80** — youtube.com (buscar "akitando 80 hardcore computação")
- **CS50x Week 0** — cs50.harvard.edu (gratuito)
- **"But How Do It Know?"** — J. Clark Scott
- **Computerphile** (YouTube) — canal excelente para visualizar conceitos de hardware
- **nandgame.com** — construa um computador do zero partindo de portas lógicas (interativo)
