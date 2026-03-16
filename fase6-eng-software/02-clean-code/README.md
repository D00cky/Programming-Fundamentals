# 02 — Clean Code & Refactoring

> *"Code is read far more often than it is written."*

---

## 1. Nomes Significativos

```java
// ❌ Nomes ruins:
int d;           // dias? distância? dados?
List<int[]> lst;
boolean flag;
void proc(List<int[]> lst);

// ✅ Nomes que revelam intenção:
int diasAtéVencimento;
List<Transacao> transacoesDoMes;
boolean usuarioEstaAutenticado;
void processarTransacoesPendentes(List<Transacao> transacoes);
```

**Regras:**
- Nomes de classes: substantivos (`Conta`, `UsuarioService`)
- Nomes de métodos: verbos (`calcular`, `salvar`, `processar`)
- Nomes de boolean: predicados (`isAtivo`, `hasPermissao`, `podeExecutar`)
- Evitar prefixos desnecessários: `IUserRepository` → `UserRepository`
- Evitar abreviações: `usrCtrl` → `usuarioController`

---

## 2. Funções

```java
// ❌ Função que faz tudo:
void processarPedido(Pedido p, boolean enviarEmail, boolean debitar, String modo) {
    // 200 linhas misturando validação, banco, email, pagamento...
}

// ✅ Funções pequenas e com um único nível de abstração:
void processarPedido(Pedido pedido) {
    validarPedido(pedido);
    debitarEstoque(pedido);
    registrarPagamento(pedido);
    notificarCliente(pedido);
}
// Cada sub-função faz uma coisa no mesmo nível de abstração
```

**Regras:**
- Uma função = uma coisa
- Máximo 20 linhas (idealmente < 10)
- Máximo 3 parâmetros — mais que isso → criar objeto
- Sem efeitos colaterais surpresa
- Command-Query Separation: função que retorna valor não deve modificar estado

---

## 3. Comentários

```java
// ❌ Comentário que só repete o código:
// Incrementa i
i++;

// ❌ Comentário desatualizado (pior que nenhum):
// Retorna o saldo em reais (em dólares desde 2023)
double getSaldo() { return saldoDolares; }

// ✅ Quando comentar: "por que", não "o que"
// Algoritmo de Floyd-Warshall com ajuste para grafos negativos.
// Referência: CLRS 3ª ed., cap 25.
void calcularMenoresCaminhos(int[][] grafo) { ... }

// ✅ Comentários válidos: TODO, FIXME, avisos de não-obviedade
// TODO: trocar por ConcurrentHashMap quando migrar para multi-thread
// FIXME: overflow em n > 10^9 — corrigir com BigInteger
// Não usar equals() aqui — NaN != NaN por definição IEEE 754
```

**Regra geral:** bom código raramente precisa de comentários. Se você precisa comentar, primeiro tente renomear ou refatorar.

---

## 4. Code Smells

**Long Method:** método muito longo → extrair sub-métodos

**Large Class:** classe faz demais → aplicar SRP, extrair classes

**Long Parameter List:** muitos parâmetros → criar objeto de parâmetros

**Duplicate Code:** código duplicado → extrair função/método

**Dead Code:** código comentado, não usado → deletar (git guarda o histórico)

**Magic Numbers:**
```java
// ❌
if (status == 3) { ... }
timeout = 86400;

// ✅
private static final int STATUS_APROVADO = 3;
private static final int SEGUNDOS_POR_DIA = 86_400;
```

**Feature Envy:** método usa mais dados de outra classe que da própria → mover método

**Primitive Obsession:** usar primitivos onde objeto seria mais claro
```java
// ❌
String email = "test@test.com";
// ✅
Email email = new Email("test@test.com");  // valida no construtor, tem métodos
```

---

## 5. Refactoring — Catálogo Essencial

**Extract Method:** transformar trecho de código em método com nome descritivo

**Rename:** renomear para revelar intenção

**Replace Magic Number with Constant:** `if (x > 13) → if (x > MAX_RETRIES)`

**Introduce Parameter Object:** `(String nome, int idade, String email)` → `(Usuario usuario)`

**Replace Conditional with Polymorphism:** switch/if por tipo → Strategy ou polimorfismo

**Move Method:** método que usa mais dados de outra classe → mover para lá

**Decompose Conditional:**
```java
// ❌
if (!conta.isVencida() && saldo >= valor && !usuario.isBloqueado()) { ... }

// ✅
if (transacaoPodeSerRealizada(conta, saldo, usuario)) { ... }
```

---

## 6. Formatação e Estrutura

- Código que pertence junto fica junto (high cohesion)
- Declarar variáveis perto de onde são usadas
- Funções chamadas devem estar abaixo das que as chamam (top-down)
- Linhas: 80-120 caracteres máximo
- Consistência é mais importante que preferência pessoal — use linter/formatter

---

## Exercícios

**ex01:** dado este código (fornecido abaixo), aplicar pelo menos 5 refactorings diferentes e documentar cada mudança com o nome do refactoring

**ex02:** implementar o mesmo algoritmo de duas formas: code smell vs clean — apresentar as diferenças

**ex03:** configurar checkstyle/spotbugs no projeto Java e corrigir todos os warnings

---

## Referências

- **Clean Code** — Robert C. Martin · capítulos 1-7, 9, 14
- **Refactoring: Improving the Design of Existing Code** — Martin Fowler · 2ª ed
- **Refactoring.Guru** — refactoring.guru/refactoring (catálogo online gratuito)
