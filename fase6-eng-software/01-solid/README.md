# 01 — Princípios SOLID

> SOLID são diretrizes de design, não regras absolutas. O objetivo é criar código mais fácil de manter, estender e testar.

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

## Referências

- **Clean Code** — Uncle Bob, capítulos 9-11
- **Agile Software Development** — Robert C. Martin (onde SOLID foi formalizado)
- **Refactoring.Guru** — refactoring.guru/solid (exemplos visuais)
