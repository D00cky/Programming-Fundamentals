# 01 — Princípios SOLID

> SOLID são diretrizes de design, não regras absolutas. O objetivo é criar código mais fácil de manter, estender e testar.

## Antes de começar

Certifique-se de que você já:

- [ ] Escreve código orientado a objetos com classes, interfaces e herança (`fase3-cpp/02-oop` ou `fase4-java/01-fundamentos`)
- [ ] Entende o que são interfaces e polimorfismo em Java ou C++
- [ ] Criou pelo menos um projeto com mais de uma classe interagindo entre si

---

## O que você vai aprender

Ao final deste módulo você será capaz de:

- Aplicar cada um dos cinco princípios SOLID com exemplos reais em Java
- Identificar violações de SOLID em código legado e nomear o princípio violado
- Refatorar código usando polimorfismo, interfaces e injeção de dependência
- Explicar quando SOLID é valioso e quando pode ser exagerado

---

## S — Single Responsibility Principle

**"Uma classe deve ter um único motivo para mudar."**

```java
// ❌ Violação: a classe faz I/O E lógica de negócio E formatação
class Relatorio {
    void gerarRelatorio() { /* busca dados do banco */ }
    void salvarEmArquivo() { /* I/O de arquivo */ }
    void enviarEmail()     { /* envio de email */ }
}

// ✅ Cada responsabilidade em sua própria classe:
class GeradorRelatorio   { Relatorio gerar(Dados d) { ... } }
class SerializadorRelatorio { String serializar(Relatorio r) { ... } }
class NotificadorEmail   { void enviar(String dest, String corpo) { ... } }
```

**Sinal de violação:** a classe tem múltiplas razões para mudar (mudança nos dados, mudança no formato de saída, mudança no protocolo de envio).

---

## O — Open/Closed Principle

**"Aberto para extensão, fechado para modificação."**

```java
// ❌ Violar: adicionar novo tipo de desconto exige modificar a classe
class CalculadoraDesconto {
    double calcular(Produto p) {
        if (p.getTipo() == TipoProduto.ELETRONICO) return p.getPreco() * 0.9;
        if (p.getTipo() == TipoProduto.ROUPA)      return p.getPreco() * 0.8;
        // ... cada novo tipo = modificar aqui
    }
}

// ✅ Abrir para extensão via polimorfismo:
interface EstrategiaDesconto { double calcular(double preco); }

class DescontoEletronico implements EstrategiaDesconto {
    public double calcular(double preco) { return preco * 0.9; }
}
class DescontoRoupa implements EstrategiaDesconto {
    public double calcular(double preco) { return preco * 0.8; }
}

// Adicionar nova categoria = nova classe, sem tocar nas existentes
```

---

## L — Liskov Substitution Principle

**"Subtipos devem ser substituíveis por seus tipos base sem quebrar o programa."**

```java
// ❌ Violação clássica: Quadrado herda de Retângulo
class Retangulo {
    void setLargura(double l)  { largura = l; }
    void setAltura(double a)   { altura = a; }
    double area() { return largura * altura; }
}

class Quadrado extends Retangulo {
    @Override void setLargura(double l) { largura = altura = l; } // quebra comportamento!
    @Override void setAltura(double a)  { largura = altura = a; }
}

// Código que espera Retangulo:
Retangulo r = new Quadrado();
r.setLargura(5);
r.setAltura(3);
// Espera área = 15, mas quadrado retorna 9!

// ✅ Solução: não herdar. Usar composição ou interfaces separadas.
interface Forma { double area(); }
class Retangulo implements Forma { ... }
class Quadrado  implements Forma { ... }
```

---

## I — Interface Segregation Principle

**"Clientes não devem ser forçados a depender de interfaces que não usam."**

```java
// ❌ Interface gorda:
interface Trabalhador {
    void trabalhar();
    void comer();
    void dormir();
}

// Robô não come nem dorme — forçado a implementar métodos irrelevantes
class Robo implements Trabalhador {
    public void trabalhar() { /* ok */ }
    public void comer()     { throw new UnsupportedOperationException(); } // ❌
    public void dormir()    { throw new UnsupportedOperationException(); } // ❌
}

// ✅ Interfaces específicas:
interface Trabalhavel { void trabalhar(); }
interface Alimentavel  { void comer(); }
interface Descansavel  { void dormir(); }

class Humano implements Trabalhavel, Alimentavel, Descansavel { ... }
class Robo   implements Trabalhavel { ... }
```

---

## D — Dependency Inversion Principle

**"Módulos de alto nível não devem depender de módulos de baixo nível. Ambos devem depender de abstrações."**

```java
// ❌ Alto nível (Service) depende de baixo nível (implementação concreta):
class OrderService {
    private MySQLDatabase db = new MySQLDatabase();  // acoplamento forte!
    void salvar(Order o) { db.insert(o); }
}

// ✅ Depender de abstração (interface):
interface OrderRepository {
    void salvar(Order o);
    Order buscar(String id);
}

class OrderService {
    private final OrderRepository repo;
    OrderService(OrderRepository repo) { this.repo = repo; }  // injeção de dependência
    void processarPedido(Order o) { repo.salvar(o); }
}

// Implementações concretas podem trocar sem afetar OrderService:
class MySQLOrderRepository   implements OrderRepository { ... }
class InMemoryOrderRepository implements OrderRepository { ... }  // para testes!
```

---

## SOLID na Prática

Os princípios se reforçam mutuamente:
- SRP: cada classe tem uma responsabilidade
- OCP: adicionar features sem modificar código existente
- LSP: herança correta e segura
- ISP: interfaces pequenas e específicas
- DIP: depender de interfaces, não de implementações

**Quando SOLID importa mais:** projetos que crescem, equipes que mudam, código que precisa de testes automatizados.

**Quando SOLID pode ser exagerado:** scripts pequenos, protótipos, código descartável. Não aplique dogmaticamente.

---

## Exercícios

**ex01:** dado uma classe `UsuarioService` que faz validação, acesso ao banco e envio de email, refatorar aplicando SRP

**ex02:** implementar sistema de relatórios (PDF, CSV, JSON) aplicando OCP com Strategy

**ex03:** criar repositório de usuários com DIP; testar com InMemoryRepository; depois trocar por JpaRepository sem mudar o Service

---

## Knowledge Check

Responda sem consultar o material. Se travar, releia a seção correspondente.

1. SRP diz que uma classe deve ter "um único motivo para mudar". O que conta como um "motivo para mudar"?
2. Como OCP e Strategy Pattern se relacionam? Desenhe mentalmente o diagrama.
3. Por que Quadrado herdar de Retângulo viola LSP? Como resolver sem quebrar a hierarquia?
4. Qual o sintoma mais claro de violação do ISP em código Java?
5. O que é injeção de dependência? Por que ela é necessária para aplicar DIP?
6. Se você tem um `switch` com `instanceof` para decidir comportamento por tipo, qual princípio provavelmente está sendo violado?
7. Como DIP facilita a escrita de testes unitários? Dê um exemplo concreto.
8. Em que situações aplicar SOLID rigorosamente pode ser contraproducente?

---

## Projeto — Sistema de E-Commerce com SOLID

Implemente um sistema de e-commerce simplificado que demonstre todos os 5 princípios:

**Requisitos de domínio:**
- Processar pedidos com diferentes métodos de pagamento (cartão, pix, boleto)
- Calcular descontos por categoria de produto (eletrônicos, roupas, alimentos)
- Notificar cliente após confirmação (email ou SMS)
- Persistir pedido (em memória por ora, mas fácil de trocar por banco)

**Mapeamento para SOLID:**
- **SRP:** `OrderService` orquestra; `PaymentProcessor`, `DiscountCalculator`, `Notifier`, `OrderRepository` separados
- **OCP:** adicionar novo método de pagamento = nova classe, sem modificar `OrderService`
- **LSP:** `CreditCardPayment` e `PixPayment` substituíveis via interface `PaymentMethod`
- **ISP:** `Notifiable` separado de `Auditable`; não forçar todos a implementar tudo
- **DIP:** `OrderService` recebe interfaces no construtor, nunca instancia dependências concretas

**Requisitos técnicos:**
- Java com pelo menos 8 classes/interfaces
- Demonstrar DIP com pelo menos 2 implementações intercambiáveis (ex: `EmailNotifier` e `SmsNotifier`)
- Incluir um teste JUnit simples que usa `InMemoryOrderRepository`

---

## Referências

- **Clean Code** — Uncle Bob, capítulos 9-11
- **Agile Software Development** — Robert C. Martin (onde SOLID foi formalizado)
- **Refactoring.Guru** — refactoring.guru/solid (exemplos visuais)

---

## Recursos Adicionais

Estes recursos são **opcionais** mas vão solidificar seu entendimento:

**Para ler/assistir agora:**
- **Refactoring.Guru/SOLID** — explicações visuais com exemplos antes/depois; ponto de entrada ideal
- **Uncle Bob: The Principles of OOD** (butunclebob.com) — artigos originais onde SOLID foi definido, gratuitos

**Para consulta:**
- **Agile Software Development** — Robert C. Martin caps. 7-12 · onde SOLID foi formalizado com contexto
- **Refactoring** — Martin Fowler · catálogo de refactorings que frequentemente implementam SOLID

**Para ir além:**
- **Growing Object-Oriented Software, Guided by Tests** — Freeman & Pryce · SOLID aplicado com TDD desde o início
- **Head First Design Patterns** — Freeman · padrões de design que materializam OCP, DIP e ISP
