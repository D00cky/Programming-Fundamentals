# 02 — Sistemas Numéricos

## Por que binário?

Computadores usam binário porque transistores têm dois estados: **ligado (1)** ou **desligado (0)**. É físico, não uma escolha arbitrária.

---

## Binário (Base 2)

Cada dígito é uma **potência de 2**:

```
Decimal:  128  64  32  16   8   4   2   1
Binário:    1   0   1   1   0   1   0   1  = 181
```

**Decimal → Binário**: divide por 2 repetidamente, leia os restos de baixo pra cima.
```
181 ÷ 2 = 90 resto 1
 90 ÷ 2 = 45 resto 0
 45 ÷ 2 = 22 resto 1
 22 ÷ 2 = 11 resto 0
 11 ÷ 2 =  5 resto 1
  5 ÷ 2 =  2 resto 1
  2 ÷ 2 =  1 resto 0
  1 ÷ 2 =  0 resto 1
                   ↑ leia daqui
Resultado: 10110101
```

**Binário → Decimal**: some as potências de 2 onde o bit é 1.
```
10110101 = 128 + 32 + 16 + 4 + 1 = 181
```

---

## Hexadecimal (Base 16)

Hexadecimal é uma forma compacta de escrever binário. Cada dígito hex = 4 bits.

| Decimal | Binário | Hex |
|---------|---------|-----|
| 0       | 0000    | 0   |
| 1       | 0001    | 1   |
| 9       | 1001    | 9   |
| 10      | 1010    | A   |
| 11      | 1011    | B   |
| 12      | 1100    | C   |
| 13      | 1101    | D   |
| 14      | 1110    | E   |
| 15      | 1111    | F   |

Em C, hex é prefixado com `0x`:
```c
int x = 0xFF;   // 255 em decimal
int y = 0x1A;   // 26 em decimal
```

**Binário → Hex**: agrupe os bits em grupos de 4, converta cada grupo.
```
10110101
1011 | 0101
  B  |  5
= 0xB5
```

Endereços de memória em C aparecem em hex:
```
0x7fff5fbff6b0   ← endereço típico na stack
0x55a3c2e14260   ← endereço típico na heap
```

---

## Bits, Bytes e Tipos em C

| Unidade | Tamanho |
|---------|---------|
| 1 bit   | 0 ou 1  |
| 1 byte  | 8 bits  |
| 1 KB    | 1024 bytes |
| 1 MB    | 1024 KB |
| 1 GB    | 1024 MB |

**Por que `int` tem 4 bytes (32 bits)?**

Com 32 bits unsigned: 0 a 4.294.967.295 (2³²-1)
Com 32 bits signed: -2.147.483.648 a 2.147.483.647

### Tamanhos dos tipos em C (em sistemas 64-bit)

```c
char    → 1 byte   (8 bits)
short   → 2 bytes  (16 bits)
int     → 4 bytes  (32 bits)
long    → 8 bytes  (64 bits)
float   → 4 bytes  (32 bits, IEEE 754)
double  → 8 bytes  (64 bits, IEEE 754)
pointer → 8 bytes  (64 bits em sistemas 64-bit)
```

---

## Complemento de Dois (Two's Complement)

Como representar números negativos em binário?

**Regra**: inverta todos os bits e adicione 1.

```
+5 em 8 bits:  00000101
Inverte bits:  11111010
Adiciona 1:    11111011  ← este é o -5
```

**Verificação**:
```
  00000101  (+5)
+ 11111011  (-5)
──────────
  00000000  = 0 ✓ (overflow ignorado)
```

O bit mais significativo (MSB) indica o sinal: `0` = positivo, `1` = negativo.

---

## Por que isso importa para C?

```c
unsigned char x = 255;  // 0xFF = 11111111
x++;                    // overflow! vira 0 (00000000)

signed char y = 127;    // 01111111
y++;                    // overflow! vira -128 (10000000)

int a = 0x41;           // 65 em decimal
char c = 0x41;          // 'A' em ASCII — mesmo número, interpretação diferente!
```

---

## Exercícios

1. Converta para binário: 42, 255, 128, 100
2. Converta para decimal: `11001010`, `00001111`, `10000001`
3. Converta para hex: 173, 255, 16, 256
4. Qual é o maior valor de um `unsigned char` (8 bits)? E de um `signed char`?
5. O que acontece com `unsigned int x = 0; x--;` em C? Por quê?
6. Se `char c = 'A'`, qual é o valor de `c + 1`? Que letra é essa? (dica: tabela ASCII)

---

## Ferramentas Práticas

```bash
# No terminal Linux, converter bases:
printf "%d\n" 0xFF        # hex → decimal (255)
printf "%x\n" 255         # decimal → hex (ff)
printf "%b\n" 42          # decimal → binário (101010)  [bash apenas]

# Python também é útil:
python3 -c "print(bin(42))"   # 0b101010
python3 -c "print(hex(255))"  # 0xff
python3 -c "print(int('ff', 16))"  # 255
```

---

## Referências

- **CS50x Week 0** — representação de dados (cs50.harvard.edu)
- **Computerphile** — "Binary Numbers and Base Systems" (YouTube)
- **Ben Eater** (YouTube) — canal sobre como computadores funcionam no nível de bits
- **Tabela ASCII**: man ascii (no terminal Linux)
