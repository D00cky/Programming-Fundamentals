# 02 — Clean Code & Refactoring

> *"Code is read far more often than it is written."*

## Antes de começar

Certifique-se de que você já:

- [ ] Concluiu `fase6-eng-software/01-solid` — refatoração pressupõe entender design
- [ ] Escreve código em alguma linguagem há pelo menos alguns meses e acumulou dívida técnica
- [ ] Tem um projeto com código que você mesmo acha difícil de ler após uma semana

---

## O que você vai aprender

Ao final deste módulo você será capaz de:

- Nomear variáveis, funções e classes de forma que o código se documente sozinho
- Identificar e corrigir code smells clássicos: Long Method, Magic Numbers, Feature Envy
- Aplicar refactorings do catálogo de Fowler (Extract Method, Introduce Parameter Object, etc.)
- Decidir quando comentar e quando o comentário é sinal de código mal escrito

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

## Knowledge Check

Responda sem consultar o material. Se travar, releia a seção correspondente.

1. O que torna um nome "significativo"? Dê um exemplo de nome ruim e sua versão melhorada.
2. "Uma função deve fazer uma coisa." O que conta como "uma coisa"?
3. Quando um comentário é sinal de código mal escrito? Dê um exemplo.
4. O que é Feature Envy? Como você detecta e corrige?
5. Qual a diferença entre Extract Method e Move Method? Quando usar cada um?
6. Por que Magic Numbers são problemáticos além da legibilidade?
7. O que é Command-Query Separation? Por que violar isso causa bugs sutis?
8. Você tem um método de 80 linhas com 3 níveis de indentação. Quais refactorings aplicar e em que ordem?

---

## Projeto — Refatoração de Código Legado

Dado o seguinte código com múltiplas violações, aplique pelo menos 6 refactorings distintos e documente cada mudança:

```java
public class Proc {
    public static double calc(List<int[]> d, int t, boolean f, String m) {
        double r = 0;
        for (int i = 0; i < d.size(); i++) {
            int[] x = d.get(i);
            if (x[2] == 1 && f == true) {
                if (t == 1) { r = r + (x[1] * 0.9); }
                else if (t == 2) { r = r + (x[1] * 0.8); }
                else { r = r + x[1]; }
            } else if (x[2] != 1) {
                r = r + x[1];
            }
        }
        if (m.equals("USD")) { r = r * 5.0; }
        System.out.println("Result: " + r);
        return r;
    }
}
```

**Refactorings a identificar e aplicar:**
- Nomes significativos para variáveis e parâmetros
- Extrair constantes para magic numbers (0.9, 0.8, 5.0, 1, 2)
- Simplificar condicionais (`f == true` → `f`)
- Extract Method para cada responsabilidade
- Separar cálculo de exibição (Command-Query Separation)
- Replace Conditional with Polymorphism para os tipos de desconto

**Entrega:** versão original, versão refatorada, e uma tabela listando cada refactoring aplicado com o nome do padrão do catálogo de Fowler.

---

## Referências

- **Clean Code** — Robert C. Martin · capítulos 1-7, 9, 14
- **Refactoring: Improving the Design of Existing Code** — Martin Fowler · 2ª ed
- **Refactoring.Guru** — refactoring.guru/refactoring (catálogo online gratuito)

---

## Recursos Adicionais

Estes recursos são **opcionais** mas vão solidificar seu entendimento:

**Para ler/assistir agora:**
- **Refactoring.Guru/Refactoring** — catálogo visual gratuito de todos os refactorings de Fowler com exemplos antes/depois em Java
- **Clean Code** caps. 1-7 — os capítulos mais densos e aplicáveis sobre nomes, funções e comentários

**Para consulta:**
- **Refactoring** — Martin Fowler 2ª ed · catálogo completo de refactorings com mecânica passo a passo
- **Checkstyle / SpotBugs documentation** — ferramentas que automatizam detecção de code smells em Java

**Para ir além:**
- **Working Effectively with Legacy Code** — Michael Feathers · técnicas para refatorar código sem testes
- **The Pragmatic Programmer** — Hunt & Thomas · perspectiva mais ampla sobre qualidade de código e carreira
