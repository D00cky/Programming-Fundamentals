# 03 — Testes

> *"Code without tests is broken code by definition."* — Jacob Kaplan-Moss

## Antes de começar

Certifique-se de que você já:

- [ ] Escreve Java básico com classes e exceptions (`fase4-java/01-fundamentos`)
- [ ] Sabe configurar um projeto com Maven ou Gradle e adicionar dependências
- [ ] Concluiu `fase6-eng-software/01-solid` e `fase6-eng-software/02-clean-code` — código bem estruturado é mais fácil de testar

---

## O que você vai aprender

Ao final deste módulo você será capaz de:

- Escrever testes unitários com JUnit 5 seguindo o padrão Arrange-Act-Assert
- Aplicar TDD (Red-Green-Refactor) para desenvolver uma feature do zero
- Usar Mockito para isolar dependências externas em testes unitários
- Interpretar um relatório de cobertura de código com JaCoCo
- Distinguir quando usar unit test, integration test e E2E test

---

## 1. Pirâmide de Testes

```
         /\
        /  \        E2E (poucos, lentos, frágeis)
       /────\
      /      \      Integration (médio)
     /────────\
    /          \    Unit (muitos, rápidos, estáveis)
   /────────────\
```

- **Unit tests:** testam uma unidade isolada (função, método, classe). Rápidos, determinísticos.
- **Integration tests:** testam integração entre componentes (ex: Service + Repository com banco real).
- **E2E tests:** testam o sistema do ponto de vista do usuário (ex: Selenium, Cypress).

---

## 2. TDD — Test-Driven Development

**Red → Green → Refactor:**

```
1. Red:     Escrever um teste que FALHA (feature ainda não existe)
2. Green:   Escrever o MÍNIMO de código para o teste passar
3. Refactor: Melhorar o código sem quebrar o teste
4. Repetir
```

```java
// 1. Red: teste falha (FizzBuzz não existe ainda)
@Test void deveRetornarFizz_quandoDivisívelPor3() {
    assertEquals("Fizz", FizzBuzz.calcular(3));
}

// 2. Green: implementação mínima
class FizzBuzz {
    static String calcular(int n) {
        if (n % 3 == 0) return "Fizz";
        return String.valueOf(n);
    }
}

// 3. Refactor: adicionar mais casos, manter testes passando
```

---

## 3. JUnit 5 (Java)

```java
import org.junit.jupiter.api.*;
import static org.junit.jupiter.api.Assertions.*;

class ContaTest {

    private Conta conta;

    @BeforeEach
    void setUp() {
        conta = new Conta("João", 100.0);
    }

    @Test
    void devePermitirDepositoValorPositivo() {
        conta.depositar(50.0);
        assertEquals(150.0, conta.getSaldo(), 0.001);
    }

    @Test
    void deveLancarExcecao_quandoDepositoNegativo() {
        assertThrows(IllegalArgumentException.class,
            () -> conta.depositar(-10.0));
    }

    @ParameterizedTest
    @ValueSource(doubles = {0, -1, -100})
    void deveLancarExcecao_quandoSaqueInvalido(double valor) {
        assertThrows(IllegalArgumentException.class,
            () -> conta.sacar(valor));
    }

    @Test
    void deveLancarExcecao_quandoSaldoInsuficiente() {
        assertThrows(IllegalStateException.class,
            () -> conta.sacar(200.0));
    }
}
```

---

## 4. Mocks e Stubs

**Mocks** simulam dependências externas para testar em isolamento:

```java
import org.mockito.*;
import static org.mockito.Mockito.*;

class OrderServiceTest {

    @Mock OrderRepository repo;
    @Mock EmailService emailService;
    @InjectMocks OrderService service;

    @BeforeEach
    void setUp() { MockitoAnnotations.openMocks(this); }

    @Test
    void deveSalvarPedido_eEnviarEmail() {
        Order pedido = new Order("item", 99.90);

        service.processarPedido(pedido);

        verify(repo).salvar(pedido);             // verificar que save foi chamado
        verify(emailService).enviar(any(), any());
    }

    @Test
    void deveLancarExcecao_quandoRepoBanca() {
        when(repo.salvar(any())).thenThrow(new RuntimeException("DB down"));

        assertThrows(RuntimeException.class,
            () -> service.processarPedido(new Order("x", 1.0)));
    }
}
```

---

## 5. GoogleTest (C/C++)

```cpp
#include <gtest/gtest.h>

// Função a testar:
int fatorial(int n) {
    if (n < 0) return -1;
    if (n <= 1) return 1;
    return n * fatorial(n - 1);
}

TEST(FatorialTest, HandlesZero)   { EXPECT_EQ(1, fatorial(0)); }
TEST(FatorialTest, HandlesPositive) {
    EXPECT_EQ(1,   fatorial(1));
    EXPECT_EQ(24,  fatorial(4));
    EXPECT_EQ(120, fatorial(5));
}
TEST(FatorialTest, HandlesNegative) { EXPECT_EQ(-1, fatorial(-1)); }

// Fixtures:
class StackTest : public ::testing::Test {
protected:
    void SetUp() override { stack.push(1); stack.push(2); }
    MinhaStack stack;
};
TEST_F(StackTest, PopReturnsTop) { EXPECT_EQ(2, stack.pop()); }
```

---

## 6. Cobertura de Código

```bash
# Java com JaCoCo (via Maven/Gradle):
mvn test jacoco:report
# Relatório em target/site/jacoco/index.html

# C com gcov:
gcc -fprofile-arcs -ftest-coverage -g main.c -o prog
./prog
gcov main.c
# Relatório em main.c.gcov
```

**Métricas:**
- **Line coverage:** % de linhas executadas
- **Branch coverage:** % de branches (if/else) exercitados
- **Mutation testing:** modifica código, verifica se testes detectam

> 100% de cobertura não garante ausência de bugs. Um teste que executa código sem verificar resultado não detecta nada.

---

## 7. Propriedades de Bons Testes (F.I.R.S.T.)

- **Fast:** rápidos (< 1s por teste unitário)
- **Independent:** não dependem da ordem de execução
- **Repeatable:** mesmo resultado sempre (determinístico)
- **Self-validating:** pass ou fail, sem interpretação manual
- **Timely:** escritos junto com o código de produção

---

## Exercícios

**ex01:** aplicar TDD para implementar `ContaBancaria` do zero (cada comportamento = ciclo Red/Green/Refactor)

**ex02:** testar `OrderService` com mocks para Repository e EmailService; cobrir casos de sucesso e erro

**ex03:** configurar JaCoCo no projeto; atingir 80% de cobertura de branches

---

## Knowledge Check

Responda sem consultar o material. Se travar, releia a seção correspondente.

1. Qual a diferença entre unit test, integration test e E2E test? Dê um exemplo de cada.
2. Por que a pirâmide de testes tem mais unit tests na base?
3. Descreva o ciclo Red-Green-Refactor do TDD em suas palavras.
4. Qual a diferença entre Mock e Stub? Quando você verificaria interações (verify) em vez de só resultados (assert)?
5. Se um teste unitário depende da ordem de execução dos outros testes, qual propriedade FIRST ele viola?
6. Por que 100% de cobertura não garante ausência de bugs?
7. Quando você preferiria um teste de integração a um teste unitário com mock?
8. O que é mutation testing e como ele complementa a cobertura de linhas?

---

## Projeto — Stack com TDD

Implemente uma `MinhaStack<T>` do zero usando TDD (cada comportamento = ciclo Red-Green-Refactor):

**Comportamentos a implementar via TDD (nesta ordem):**
1. Stack recém criada tem tamanho 0
2. `push` aumenta o tamanho em 1
3. `pop` retorna o último elemento inserido
4. `pop` diminui o tamanho em 1
5. `peek` retorna o topo sem remover
6. `pop` em stack vazia lança `EmptyStackException`
7. Stack suporta múltiplos pushes e pops na ordem correta (LIFO)
8. `isEmpty` retorna `true` apenas quando vazia

**Requisitos técnicos:**
- Java com JUnit 5 — cada item acima deve ter pelo menos um teste
- Seguir TDD estrito: escrever o teste antes de qualquer linha de implementação
- Configurar JaCoCo e atingir 100% de branch coverage nos testes da stack
- Documentar nos commits a sequência Red-Green-Refactor

**Exemplo de teste:**
```java
@Test
void deveRetornarElementosEmOrdemLIFO() {
    MinhaStack<Integer> stack = new MinhaStack<>();
    stack.push(1);
    stack.push(2);
    stack.push(3);
    assertEquals(3, stack.pop());
    assertEquals(2, stack.pop());
    assertEquals(1, stack.pop());
}
```

---

## Referências

- **Test-Driven Development: By Example** — Kent Beck · o livro que definiu TDD
- **The Art of Unit Testing** — Roy Osherove
- **JUnit 5 User Guide** — junit.org/junit5/docs/current/user-guide
- **Mockito Documentation** — javadoc.io/doc/org.mockito/mockito-core

---

## Recursos Adicionais

Estes recursos são **opcionais** mas vão solidificar seu entendimento:

**Para ler/assistir agora:**
- **TDD by Example** — Kent Beck caps. 1-17 · implementação de Money com TDD do zero; leitura curta e densa
- **JUnit 5 User Guide** (junit.org) — documentação oficial com exemplos de todas as annotations

**Para consulta:**
- **Mockito Documentation** — referência completa de when/then, verify, ArgumentCaptor e InjectMocks
- **JaCoCo** — configuração com Maven/Gradle e interpretação dos relatórios de cobertura

**Para ir além:**
- **Growing Object-Oriented Software, Guided by Tests** — Freeman & Pryce · TDD com mocks desde o design
- **The Art of Unit Testing** — Roy Osherove · cobertura profunda de mocks, stubs e design testável
