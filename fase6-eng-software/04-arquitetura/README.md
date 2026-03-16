# 04 — Arquitetura de Software

> *"Architecture is about the important stuff. Whatever that is."* — Ralph Johnson

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

## Referências

- **Clean Architecture** — Robert C. Martin
- **Domain-Driven Design** — Eric Evans (livro azul)
- **Implementing DDD** — Vaughn Vernon (livro vermelho, mais prático)
- **Designing Data-Intensive Applications** — Martin Kleppmann (sistemas distribuídos)
- **Building Microservices** — Sam Newman
