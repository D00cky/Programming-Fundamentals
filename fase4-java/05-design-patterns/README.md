# 05 — Design Patterns (GoF)

> *"Design patterns are not a recipe. They are a vocabulary for talking about design."*

Os 23 patterns do GoF (Gang of Four) são soluções recorrentes para problemas comuns de design. Memorizá-los não é o objetivo — entender o **problema** que cada um resolve é.

---

## Categorias

```
Creational  — Como criar objetos
Structural  — Como compor classes e objetos
Behavioral  — Como objetos se comunicam
```

---

## Creational Patterns

### Singleton
Garante que uma classe tenha **apenas uma instância**:
```java
public class Config {
    private static Config instance;
    private Config() {}
    public static synchronized Config getInstance() {
        if (instance == null) instance = new Config();
        return instance;
    }
    // Moderno (thread-safe, lazy, sem synchronized):
    private static class Holder { static final Config INSTANCE = new Config(); }
    public static Config getInstance() { return Holder.INSTANCE; }
}
```
**Problema que resolve:** configuração global, connection pool, logger.
**Cuidado:** dificulta testes (prefira injeção de dependência).

### Factory Method
Define interface para criar objetos, delegando a subclasses qual instanciar:
```java
abstract class Botao { abstract void render(); }
class BotaoWindows extends Botao { void render() { System.out.println("[Win Button]"); } }
class BotaoMac extends Botao     { void render() { System.out.println("[Mac Button]"); } }

abstract class UI {
    abstract Botao criarBotao();  // Factory Method
    void renderUI() { criarBotao().render(); }
}
class UIWindows extends UI { Botao criarBotao() { return new BotaoWindows(); } }
class UIMac     extends UI { Botao criarBotao() { return new BotaoMac(); }     }
```

### Builder
Constrói objetos complexos passo a passo:
```java
Pizza pizza = new Pizza.Builder("grande")
    .queijo(true)
    .pepperoni(true)
    .cogumelos(false)
    .build();
```

---

## Structural Patterns

### Adapter
Converte interface incompatível para outra esperada:
```java
interface Tomada3Pinos { void conectar3(); }
class TomadaEuropeia   { void conectar2() { /* 2 pinos */ } }

class Adaptador implements Tomada3Pinos {
    private TomadaEuropeia europeia;
    Adaptador(TomadaEuropeia e) { europeia = e; }
    public void conectar3() { europeia.conectar2(); }  // adapta
}
```

### Decorator
Adiciona comportamento a objetos sem alterar a classe:
```java
interface Cafe { String descricao(); double preco(); }
class CafeSimples implements Cafe { ... }

abstract class DecoradorCafe implements Cafe {
    protected Cafe cafe;
    DecoradorCafe(Cafe c) { cafe = c; }
}
class ComLeite extends DecoradorCafe {
    public String descricao() { return cafe.descricao() + ", leite"; }
    public double preco()     { return cafe.preco() + 0.5; }
}
// Uso: new ComLeite(new ComAcucar(new CafeSimples()))
```

### Composite
Trata objetos individuais e composições uniformemente (árvores):
```java
interface Componente { void render(int indent); }
class Arquivo implements Componente { ... }
class Pasta   implements Componente {
    List<Componente> filhos = new ArrayList<>();
    void add(Componente c) { filhos.add(c); }
    public void render(int indent) {
        // imprime nome; chama render em cada filho
    }
}
```

---

## Behavioral Patterns

### Observer
Define dependência um-para-muitos: quando um objeto muda, todos os dependentes são notificados:
```java
interface Observer { void atualizar(String evento); }

class EventoPublisher {
    private List<Observer> observers = new ArrayList<>();
    void subscribe(Observer o)   { observers.add(o); }
    void unsubscribe(Observer o) { observers.remove(o); }
    void notificar(String ev)    { observers.forEach(o -> o.atualizar(ev)); }
}
```
**Exemplos reais:** event listeners, MVC (Model → View), Rx/reactive streams.

### Strategy
Define família de algoritmos, encapsula cada um e os torna intercambiáveis:
```java
interface SortStrategy { void sort(int[] arr); }
class BubbleSort  implements SortStrategy { ... }
class QuickSort   implements SortStrategy { ... }
class MergeSort   implements SortStrategy { ... }

class Ordenador {
    private SortStrategy strategy;
    void setStrategy(SortStrategy s) { strategy = s; }
    void ordenar(int[] arr) { strategy.sort(arr); }
}
// Trocar algoritmo em runtime sem mudar o código do Ordenador
```

### Command
Encapsula uma operação como objeto (permite undo/redo, fila de operações):
```java
interface Comando { void executar(); void desfazer(); }

class IncrementarComando implements Comando {
    private Contador contador;
    public void executar()  { contador.incrementar(); }
    public void desfazer()  { contador.decrementar(); }
}

class EditorTexto {
    Deque<Comando> historico = new ArrayDeque<>();
    void executar(Comando c) { c.executar(); historico.push(c); }
    void undo() { if (!historico.isEmpty()) historico.pop().desfazer(); }
}
```

### Template Method
Define esqueleto de algoritmo na classe base, deixando subclasses preencherem os detalhes:
```java
abstract class RelatorioGerador {
    // Template method — não pode ser sobrescrito
    final void gerar() {
        cabecalho();    // hook
        dados();        // abstract — subclasse define
        rodape();       // hook
    }
    void cabecalho() { System.out.println("=== Relatório ==="); }
    abstract void dados();
    void rodape()   { System.out.println("================="); }
}
```

---

## Patterns mais usados na prática

1. **Singleton** — configuração, loggers
2. **Builder** — objetos com muitos parâmetros (ex: `HttpRequest.builder()`)
3. **Factory** — criar objetos sem expor implementação
4. **Observer** — eventos, listeners, reactive
5. **Strategy** — trocar algoritmos/comportamentos
6. **Decorator** — Java I/O (BufferedReader, InputStreamReader, ...)
7. **Adapter** — integração de sistemas legados

---

## Exercícios

**ex01:** implementar um sistema de notificações com Observer: email, SMS, push notification

**ex02:** calculadora com Strategy (operações como objetos) e Command (com undo/redo)

**ex03:** pipeline de processamento de dados usando Decorator: parse → validar → transformar → serializar

---

## Referências

- **Head First Design Patterns** — Freeman & Freeman · mais didático
- **Design Patterns: Elements of Reusable Object-Oriented Software** — GoF · o original
- **Refactoring.Guru** — refactoring.guru/design-patterns (gratuito, com diagramas)
