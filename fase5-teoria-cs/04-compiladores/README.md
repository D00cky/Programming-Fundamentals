# 04 — Compiladores

> *"A compiler is a program that reads a program written in one language and translates it into an equivalent program in another language."* — Aho et al.

---

## 1. Fases de um Compilador

```
Código Fonte
     ↓
[Análise Léxica]      → tokens
     ↓
[Análise Sintática]   → AST (Abstract Syntax Tree)
     ↓
[Análise Semântica]   → AST anotada (tipos, escopos)
     ↓
[Geração de IR]       → código intermediário
     ↓
[Otimização]          → IR otimizado
     ↓
[Geração de Código]   → assembly / bytecode
     ↓
Código Objeto
```

---

## 2. Análise Léxica (Lexer/Scanner)

Converte sequência de caracteres em **tokens**.

```
"int x = 42 + 1;"
→ [INT] [IDENT:"x"] [ASSIGN] [NUM:42] [PLUS] [NUM:1] [SEMICOLON]
```

Implementado com **autômatos finitos determinísticos (DFA)** ou ferramentas como `flex`/`lex`.

```c
// Lexer manual simples:
typedef enum { TOK_INT, TOK_IDENT, TOK_NUM, TOK_PLUS, TOK_ASSIGN, TOK_EOF } TokenType;
typedef struct { TokenType type; char *val; int num; } Token;

Token next_token(char **src) {
    while (**src == ' ') (*src)++;
    if (**src == '\0') return (Token){TOK_EOF};
    if (isdigit(**src)) {
        int n = 0;
        while (isdigit(**src)) n = n*10 + (*(*src)++ - '0');
        return (Token){TOK_NUM, NULL, n};
    }
    if (**src == '+') { (*src)++; return (Token){TOK_PLUS}; }
    // ...
}
```

---

## 3. Análise Sintática (Parser)

Verifica se os tokens formam estrutura gramatical válida e constrói a AST.

### Gramáticas Livres de Contexto (CFG)

```
// Gramática para expressões simples:
expr   → term (('+' | '-') term)*
term   → factor (('*' | '/') factor)*
factor → NUM | '(' expr ')'
```

### Parser Descendente Recursivo (LL(1))

```c
// Cada não-terminal vira uma função:
int parse_factor(char **src) {
    Token t = next_token(src);
    if (t.type == TOK_NUM) return t.num;
    if (t.type == TOK_LPAREN) {
        int val = parse_expr(src);
        expect(src, TOK_RPAREN);
        return val;
    }
    error("esperado número ou (");
}

int parse_term(char **src) {
    int val = parse_factor(src);
    while (peek(src) == TOK_STAR || peek(src) == TOK_SLASH) {
        Token op = next_token(src);
        int right = parse_factor(src);
        val = (op.type == TOK_STAR) ? val * right : val / right;
    }
    return val;
}
```

### AST — Abstract Syntax Tree

```
"2 + 3 * 4"

        [+]
       /   \
     [2]   [*]
           / \
         [3] [4]
```

```c
typedef enum { NUM, BINOP } NodeType;
typedef struct Node {
    NodeType type;
    int num;
    char op;
    struct Node *left, *right;
} Node;
```

---

## 4. Análise Semântica

Verifica regras que a gramática não captura:
- **Tipos:** `int x = "hello"` — erro de tipo
- **Escopo:** usar variável antes de declarar
- **Arity:** chamar função com número errado de argumentos

Implementado com **tabela de símbolos** (hash table mapeando identificadores → tipo/escopo).

---

## 5. Geração de Código Intermediário

**Three-Address Code (TAC):**
```
// a = b * c + d * e
t1 = b * c
t2 = d * e
a  = t1 + t2
```

**Static Single Assignment (SSA):** cada variável atribuída exatamente uma vez — facilita otimizações. LLVM IR usa SSA.

---

## 6. Otimizações Clássicas

```
Constant Folding:     2 + 3 → 5 (em compile time)
Dead Code Elimination: if (false) { ... } → removido
Common Subexpression: a*b + a*b → t=a*b; t+t
Loop Invariant:       mover código invariante para fora do loop
Inlining:             substituir chamada de função pelo corpo
Tail Call Opt:        recursão de cauda → loop
```

---

## 7. Projeto Prático: Mini Calculadora com Parser

Implementar:
1. Lexer para expressões aritméticas (`+`, `-`, `*`, `/`, `(`, `)`, números)
2. Parser recursivo descendente
3. Avaliador que percorre a AST
4. Extensão: suporte a variáveis e atribuições

---

## Referências

- **Crafting Interpreters** — Robert Nystrom · **gratuito online** em craftinginterpreters.com · o melhor recurso prático
- **Dragon Book** (Compilers: Principles, Techniques, and Tools) — Aho et al. · o clássico acadêmico
- **Writing an Interpreter in Go** — Thorsten Ball · prático, pequeno
- **LLVM Tutorial** — llvm.org/docs/tutorial · construir compilador real
