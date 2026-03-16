# 04 — Compiladores

> *"A compiler is a program that reads a program written in one language and translates it into an equivalent program in another language."* — Aho et al.

## Antes de começar

Certifique-se de que você já:

- [ ] Domina manipulação de strings e arrays de `char` em C (`fase1-c/01-fundamentos`, seção 7)
- [ ] Sabe usar structs e ponteiros para construir estruturas recursivas (`fase1-c/02-ponteiros-memoria`)
- [ ] Entende recursão — o parser descendente recursivo é totalmente recursivo
- [ ] Concluiu `fase5-teoria-cs/02-paradigmas` — especialmente a seção sobre recursão e D&C

---

## O que você vai aprender

Ao final deste módulo você será capaz de:

- Explicar as fases de um compilador e o que acontece em cada uma
- Implementar um lexer manual que converte código-fonte em tokens
- Implementar um parser descendente recursivo (LL(1)) para expressões aritméticas
- Construir e percorrer uma AST (Abstract Syntax Tree) para avaliar expressões
- Entender otimizações clássicas como constant folding e dead code elimination

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

## Knowledge Check

Responda sem consultar o material. Se travar, releia a seção correspondente.

1. O que é um token? Dê 5 exemplos de tokens para a expressão `int x = 2 + 3;`
2. Qual a diferença entre análise léxica e análise sintática?
3. O que é uma gramática livre de contexto (CFG)? Escreva uma gramática simples para `expr → term + term`.
4. Por que no parser descendente recursivo cada não-terminal vira uma função?
5. O que é uma AST? Em que se diferencia de uma parse tree?
6. O que verifica a análise semântica que a sintática não consegue?
7. O que é constant folding? Dê um exemplo de código que se beneficia dessa otimização.
8. O que é SSA (Static Single Assignment) e por que facilita otimizações?

---

## Projeto — Mini Calculadora com Parser

Implemente um interpretador de expressões aritméticas do zero:

**Fase 1 — Lexer:**
- Reconhecer tokens: números inteiros, `+` `-` `*` `/` `(` `)` `:=` identificadores
- Ignorar espaços e retornar `TOK_EOF` no fim

**Fase 2 — Parser e Avaliação:**
- Parser descendente recursivo para gramática: `expr → term ((+|-) term)*` / `term → factor ((*|/) factor)*`
- Avaliar e retornar resultado diretamente (sem construir AST explícita)

**Fase 3 — AST:**
- Construir nós de AST (`NUM`, `BINOP`) durante o parsing
- Percorrer a AST em pós-ordem para avaliar

**Fase 4 (extensão) — Variáveis:**
- Suporte a atribuições: `x := 2 + 3` armazena 5 em `x`
- Usar variáveis em expressões subsequentes: `x * 2` → 10
- Tabela de símbolos com array de pares `(nome, valor)`

**Exemplo de execução:**
```
calc> 2 + 3 * 4
14
calc> x := 10
x = 10
calc> x / 2 + 1
6
calc> (2 + 3) * (4 - 1)
15
calc> q
Saindo.
```

**Requisitos técnicos:**
- Em C, compilar sem warnings com `gcc -Wall -Wextra -Werror`
- Lexer, parser e avaliador em funções separadas
- Mensagem de erro clara para expressão inválida (sem crash)

---

## Referências

- **Crafting Interpreters** — Robert Nystrom · **gratuito online** em craftinginterpreters.com · o melhor recurso prático
- **Dragon Book** (Compilers: Principles, Techniques, and Tools) — Aho et al. · o clássico acadêmico
- **Writing an Interpreter in Go** — Thorsten Ball · prático, pequeno
- **LLVM Tutorial** — llvm.org/docs/tutorial · construir compilador real

---

## Recursos Adicionais

Estes recursos são **opcionais** mas vão solidificar seu entendimento:

**Para ler/assistir agora:**
- **Crafting Interpreters** (craftinginterpreters.com) — gratuito, completo, guia passo a passo para construir um interpretador real; a leitura mais recomendada para este módulo
- **Writing an Interpreter in Go** — Thorsten Ball · curto, muito prático, ótimo complemento

**Para consulta:**
- **Dragon Book** caps. 1-4 — referência acadêmica para fases de compilação com formalismos
- **flex & bison documentation** — geradores automáticos de lexer/parser quando a gramática é grande demais para escrever à mão

**Para ir além:**
- **LLVM Tutorial** (llvm.org) — construir um compilador real com backend de otimização industrial
- **Engineering a Compiler** — Cooper & Torczon · segunda melhor referência após Dragon Book, mais moderna
