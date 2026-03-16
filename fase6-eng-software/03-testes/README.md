# 03 — Testes

> *"Code without tests is broken code by definition."* — Jacob Kaplan-Moss

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

## Referências

- **Test-Driven Development: By Example** — Kent Beck · o livro que definiu TDD
- **The Art of Unit Testing** — Roy Osherove
- **JUnit 5 User Guide** — junit.org/junit5/docs/current/user-guide
- **Mockito Documentation** — javadoc.io/doc/org.mockito/mockito-core
