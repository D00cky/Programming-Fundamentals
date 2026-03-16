# 04 — Arquitetura de Software

> *"Architecture is about the important stuff. Whatever that is."* — Ralph Johnson

## Antes de começar

Certifique-se de que você já:

- [ ] Concluiu `fase6-eng-software/01-solid`, `02-clean-code` e `03-testes`
- [ ] Entende injeção de dependência e interfaces em Java
- [ ] Já leu sobre ou implementou uma aplicação MVC (Model-View-Controller)

---

## O que você vai aprender

Ao final deste módulo você será capaz de:

- Diferenciar Layered, Clean Architecture e Hexagonal Architecture e quando usar cada uma
- Identificar os conceitos essenciais de DDD: Bounded Context, Aggregate, Domain Event
- Explicar as trocas entre monolito e microsserviços com exemplos concretos
- Desenhar a arquitetura de um sistema simples em diagrama com justificativa das decisões

---

## 1. Layered Architecture (N-Tier)

A arquitetura mais comum. Camadas com dependências top-down:

```
┌──────────────────────────┐
│  Presentation Layer      │  Controller, REST endpoints, UI
├──────────────────────────┤
│  Business Logic Layer    │  Services, Use Cases, Domain rules
├──────────────────────────┤
│  Data Access Layer       │  Repository, DAO, ORM
├──────────────────────────┤
│  Infrastructure Layer    │  Database, Cache, Message Queue
└──────────────────────────┘
```

**Problema:** Business Layer depende de Data Access Layer — difícil testar sem banco.

---

## 2. Clean Architecture

Inversão de dependências: o domínio não conhece nada externo.

```
                  ┌──────────────────────┐
                  │     Frameworks       │  Spring, Express, Django
              ┌───┴──────────────────────┴───┐
              │       Interface Adapters      │  Controllers, Presenters, Gateways
          ┌───┴───────────────────────────────┴───┐
          │          Application Use Cases         │  Regras de aplicação
      ┌───┴───────────────────────────────────────┴───┐
      │                 Domain Entities                │  Regras de negócio puras
      └───────────────────────────────────────────────┘
```

**Regra de dependência:** apontam sempre para dentro. O domínio não importa nada externo.

```java
// Domínio: sem anotações Spring, sem dependências externas
class Pedido {
    private List<Item> itens;
    Money calcularTotal() { ... }
    void validar() { ... }
}

// Use Case: orquestra domínio, depende de interfaces
class ProcessarPedidoUseCase {
    ProcessarPedidoUseCase(PedidoRepository repo, Notificador notificador) { ... }
    void executar(Pedido pedido) {
        pedido.validar();
        repo.salvar(pedido);
        notificador.notificar(pedido);
    }
}

// Infraestrutura: implementa as interfaces do domínio
@Repository
class JpaPedidoRepository implements PedidoRepository { ... }
```

---

## 3. Hexagonal Architecture (Ports & Adapters)

O domínio no centro, adaptadores nas bordas:

```
                    [HTTP Client]  [Message Queue]
                         │               │
                    ┌────┴───────────────┴────┐
                    │      Input Adapters      │   (Driving side)
                    │  REST Controller / CLI   │
                    └────────────┬────────────┘
                                 │ Port (Interface)
                    ┌────────────┴────────────┐
                    │       Domain Core        │
                    │     (Business Logic)     │
                    └────────────┬────────────┘
                                 │ Port (Interface)
                    ┌────────────┴────────────┐
                    │     Output Adapters      │   (Driven side)
                    │  Database / Email / SMS  │
                    └────────────┬────────────┘
                                 │
                        [MySQL]  [SMTP]  [AWS]
```

O domínio define **ports** (interfaces). Adapters implementam os ports. Fácil trocar adaptadores (ex: MySQL → PostgreSQL) sem tocar o domínio.

---

## 4. Domain-Driven Design (DDD) — Conceitos Essenciais

**Ubiquitous Language:** o código usa os mesmos termos do negócio. Se o cliente fala "pedido", o código tem classe `Pedido`.

**Bounded Context:** limites onde um modelo é consistente. Em sistemas grandes, diferentes contextos têm diferentes modelos do mesmo conceito (ex: "Cliente" no contexto de Vendas vs. Suporte).

**Aggregates:** clusters de objetos tratados como unidade. Toda modificação passa pela raiz do aggregate.
```java
// Pedido é aggregate root — não modifique Item diretamente
class Pedido {  // aggregate root
    private List<ItemPedido> itens;   // dentro do aggregate
    void adicionarItem(Produto p, int qtd) { itens.add(new ItemPedido(p, qtd)); }
}
```

**Repository:** abstração de persistência por aggregate.
**Domain Service:** lógica que não pertence naturalmente a uma entidade.
**Domain Events:** fatos que ocorreram no domínio (`PedidoCriado`, `PagamentoConfirmado`).

---

## 5. Microservices vs Monolith

| Aspecto | Monolith | Microservices |
|---------|----------|---------------|
| Deploy | Simples | Complexo (K8s, service discovery) |
| Desenvolvimento | Simples (1 codebase) | Complexo (múltiplos repos/times) |
| Escalabilidade | Escalar tudo junto | Escalar serviços individualmente |
| Falhas | Afeta tudo | Isoladas (com circuit breaker) |
| Latência | Chamadas in-process | Chamadas de rede (ms) |
| Quando usar | Maioria dos casos | Escala organizacional muito alta |

> **Regra de ouro:** comece com monolith bem modularizado. Extraia microsserviços apenas quando tiver razão concreta (times diferentes, escalas muito diferentes, linguagens diferentes).

---

## 6. Padrões de Integração

**API REST:** stateless, recursos como URLs, HTTP verbs (GET, POST, PUT, DELETE)

**Event-Driven:** serviços publicam eventos em message broker (Kafka, RabbitMQ). Decoupling máximo, complexidade maior.

**CQRS:** Command Query Responsibility Segregation — separar operações de leitura (Query) de escrita (Command). Permite otimizar cada lado independentemente.

**Event Sourcing:** estado = sequência de eventos, não estado atual. Auditoria completa, reversibilidade total.

---

## Exercícios

**ex01:** refatorar o CRUD do projeto Java de layered architecture para Clean Architecture

**ex02:** desenhar o design de um sistema de e-commerce simples com DDD (identificar aggregates, bounded contexts, domain events)

**ex03:** adicionar event-driven communication entre dois módulos usando observer pattern in-process; discutir como isso evoluiria para microservices

---

## Knowledge Check

Responda sem consultar o material. Se travar, releia a seção correspondente.

1. Qual o problema da Layered Architecture com relação a testabilidade? Como Clean Architecture resolve?
2. O que é a "Regra de Dependência" da Clean Architecture? O que ela proíbe?
3. Explique Ports & Adapters (Hexagonal) em suas palavras. O que é um Port? O que é um Adapter?
4. O que é Ubiquitous Language no DDD? Por que ela importa além do código?
5. O que é um Aggregate Root? Por que você não deve modificar entidades internas diretamente?
6. Qual a diferença entre Domain Service e Application Use Case?
7. Quando você escolheria microsserviços sobre um monolito bem modularizado?
8. O que é CQRS? Qual problema ele resolve e qual complexidade ele introduz?

---

## Projeto — Arquitetura de Sistema de E-Commerce

Desenhe e documente a arquitetura de um sistema de e-commerce simplificado:

**Escopo do sistema:**
- Catálogo de produtos (listar, buscar, detalhar)
- Carrinho de compras (adicionar, remover, calcular total com descontos)
- Checkout (validar estoque, processar pagamento, emitir pedido)
- Notificações (email de confirmação, SMS de envio)

**Entregáveis:**

**1. Diagrama de Arquitetura** (ASCII art no README ou ferramenta como draw.io):
- Mostrar camadas/hexágonos com dependências claramente marcadas
- Identificar quais componentes são Domain, Application, Infra

**2. Mapeamento DDD:**
- Identificar os Aggregates (ex: Pedido, Produto, Carrinho)
- Listar pelo menos 3 Domain Events (ex: `PedidoCriado`, `PagamentoConfirmado`)
- Definir os Bounded Contexts e onde estão os limites

**3. Decisões Arquiteturais (ADR simplificado):**
- Por que Clean Architecture (ou Hexagonal) neste contexto?
- Monolito ou microsserviços? Justifique.
- Como o domínio de Pagamento se comunica com o de Pedido?

**Exemplo de estrutura de pacotes:**
```
com.loja/
  domain/
    pedido/          ← Aggregate Pedido
    produto/         ← Aggregate Produto
    pagamento/       ← Domain Service
  application/
    checkout/        ← Use Case
    catalogo/        ← Use Case
  infrastructure/
    persistence/     ← JPA Repositories
    messaging/       ← Email/SMS adapters
  interfaces/
    rest/            ← Controllers
```

---

## Referências

- **Clean Architecture** — Robert C. Martin
- **Domain-Driven Design** — Eric Evans (livro azul)
- **Implementing DDD** — Vaughn Vernon (livro vermelho, mais prático)
- **Designing Data-Intensive Applications** — Martin Kleppmann (sistemas distribuídos)
- **Building Microservices** — Sam Newman

---

## Recursos Adicionais

Estes recursos são **opcionais** mas vão solidificar seu entendimento:

**Para ler/assistir agora:**
- **Implementing DDD** — Vaughn Vernon · mais prático que o livro azul do Evans; começa com exemplos concretos em Java
- **Clean Architecture** — Uncle Bob · curto e direto, explica a Regra de Dependência com clareza

**Para consulta:**
- **Martin Fowler's bliki** (martinfowler.com) — artigos sobre Microservices, EventSourcing, CQRS, DDD com exemplos curtos
- **Architecture Decision Records (ADR)** — template de Michael Nygard para documentar decisões arquiteturais

**Para ir além:**
- **Designing Data-Intensive Applications** — Kleppmann · sistemas distribuídos, consistência eventual, Kafka; a leitura mais importante para sistemas em escala
- **Domain-Driven Design** — Eric Evans · o livro azul original, denso mas fundamental para entender os conceitos em profundidade
