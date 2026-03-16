# 01 — Fundamentos de Java

---

## 1. Java vs C++: Diferenças Chave

| Aspecto | C++ | Java |
|---------|-----|------|
| Memória | Manual (new/delete) | Automática (GC) |
| Execução | Binário nativo | Bytecode na JVM |
| Ponteiros | Sim | Não (só referências) |
| Herança múltipla | Sim | Não (interfaces compensam) |
| Primitivos | int, double... | int, double... + Integer, Double (wrappers) |
| String | std::string ou char* | String (imutável, pool) |

---

## 2. Tipos e Variáveis

```java
// Primitivos (stack, valor direto):
byte  b = 127;        // 8-bit
short s = 32767;      // 16-bit
int   i = 2_147_483_647;  // 32-bit (underscore para legibilidade)
long  l = 9_223_372_036_854_775_807L;  // 64-bit, sufixo L
float  f = 3.14f;     // 32-bit float, sufixo f
double d = 3.14;      // 64-bit double
char   c = 'A';       // UTF-16, 16-bit
boolean ok = true;

// Wrappers (heap, objeto):
Integer  obj = 42;     // autoboxing: int → Integer automaticamente
int prim = obj;        // unboxing: Integer → int

// var (Java 10+): inferência de tipo local
var lista = new ArrayList<String>();
var texto = "hello";
```

---

## 3. Strings

String é **imutável** em Java. Cada concatenação cria um novo objeto:

```java
String s = "hello";
s.length()              // 5
s.charAt(0)             // 'h'
s.substring(1, 3)       // "el"
s.indexOf("llo")        // 2
s.contains("llo")       // true
s.toLowerCase()         // "hello"
s.trim()                // remove espaços das bordas
s.split(",")            // String[] dividida por vírgula
s.equals("hello")       // true (NUNCA use == para strings!)
String.format("Olá, %s! Você tem %d anos.", nome, idade)

// Para concatenações em loop: StringBuilder é O(n), String + é O(n²)
StringBuilder sb = new StringBuilder();
for (int j = 0; j < 1000; j++) sb.append(j).append(",");
String resultado = sb.toString();
```

---

## 4. OOP em Java

```java
public class Conta {
    private String titular;   // private: encapsulamento
    private double saldo;

    public Conta(String titular, double saldoInicial) {
        this.titular = titular;
        this.saldo   = saldoInicial;
    }

    public void depositar(double valor) {
        if (valor <= 0) throw new IllegalArgumentException("Valor inválido");
        saldo += valor;
    }

    public void sacar(double valor) {
        if (valor > saldo) throw new IllegalStateException("Saldo insuficiente");
        saldo -= valor;
    }

    public double getSaldo()    { return saldo; }
    public String getTitular()  { return titular; }

    @Override
    public String toString() {
        return String.format("Conta[%s, R$ %.2f]", titular, saldo);
    }
}
```

---

## 5. Herança e Interfaces

```java
// Classe abstrata:
public abstract class Animal {
    protected String nome;
    public Animal(String nome) { this.nome = nome; }
    public abstract String som();   // método abstrato
    public String getNome() { return nome; }
}

// Interface (contrato puro — Java 8+ pode ter default methods):
public interface Domesticavel {
    boolean podeAdestrar();
    default String descricao() {     // default method (Java 8+)
        return "Animal domesticável";
    }
}

// Implementação:
public class Cachorro extends Animal implements Domesticavel {
    public Cachorro(String nome) { super(nome); }

    @Override public String som()           { return "Au au!"; }
    @Override public boolean podeAdestrar() { return true; }
}
```

---

## 6. Generics

```java
// Classe genérica:
public class Par<A, B> {
    private A primeiro;
    private B segundo;
    public Par(A p, B s) { primeiro = p; segundo = s; }
    public A getPrimeiro() { return primeiro; }
    public B getSegundo()  { return segundo; }
}

Par<String, Integer> par = new Par<>("hello", 42);

// Método genérico:
public static <T extends Comparable<T>> T maximo(T a, T b) {
    return a.compareTo(b) >= 0 ? a : b;
}

// Wildcards:
void imprimeLista(List<?> lista) { /* aceita List de qualquer tipo */ }
void processa(List<? extends Number> nums) { /* Number ou subclasses */ }
```

---

## 7. Exceções

```java
// Checked (deve ser tratada ou declarada):
try {
    FileReader f = new FileReader("arquivo.txt");   // throws IOException
} catch (IOException e) {
    System.err.println("Erro: " + e.getMessage());
} finally {
    // sempre executa (fechar recursos)
}

// try-with-resources (Java 7+): fecha automaticamente:
try (BufferedReader br = new BufferedReader(new FileReader("file.txt"))) {
    String line = br.readLine();
}

// Unchecked (RuntimeException — não precisa declarar):
throw new IllegalArgumentException("Valor negativo: " + val);
throw new IllegalStateException("Estado inválido");
throw new NullPointerException();  // melhor: checar com Objects.requireNonNull()

// Exceção personalizada:
public class ContaException extends RuntimeException {
    public ContaException(String msg) { super(msg); }
}
```

---

## 8. Enums e Records

```java
// Enum (mais poderoso que em C/C++):
public enum DiaSemana {
    SEGUNDA, TERCA, QUARTA, QUINTA, SEXTA, SABADO, DOMINGO;
    public boolean isFinDeSemana() {
        return this == SABADO || this == DOMINGO;
    }
}
DiaSemana.SEXTA.isFinDeSemana()  // false

// Record (Java 16+): classe imutável de dados
public record Ponto(double x, double y) {
    // Gera automaticamente: construtor, getters, equals, hashCode, toString
    public double distancia(Ponto outro) {
        return Math.hypot(x - outro.x(), y - outro.y());
    }
}
var p = new Ponto(3.0, 4.0);
p.x(); p.y();  // getters gerados automaticamente
```

---

## Exercícios

**ex01:** implementar a classe `ContaBancaria` com depósito, saque, transferência e histórico de transações

**ex02:** hierarquia de formas (Shape → Circle, Rectangle, Triangle) com área, perímetro, e ordenar por área

**ex03:** pilha genérica `Stack<T>` e fila genérica `Queue<T>` do zero (sem usar java.util)

---

## Referências

- **Effective Java** — capítulos 1-6 (items 1-36)
- **Java: The Complete Reference** — Herbert Schildt
- **Oracle Java Tutorials** — docs.oracle.com/javase/tutorial
