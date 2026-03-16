# 02 — OOP: Herança, Polimorfismo & Interfaces

> *"OOP to me means only messaging, local retention and protection, and hiding of state-process."* — Alan Kay

---

## Antes de começar

Certifique-se de que você já:

- [ ] Escreveu classes C++ com construtores, destrutores e RAII (`fase3-cpp/01`)
- [ ] Entende referências e a diferença entre `const T&` e `T` como parâmetro
- [ ] Sabe compilar com `g++ -std=c++17` e usar `std::string`
- [ ] Conhece o conceito de herança de `struct` em C (base para herança em C++)

---

## O que você vai aprender

Ao final deste módulo você será capaz de:

- Usar herança pública e o papel do destrutor virtual em classes base
- Implementar polimorfismo dinâmico via funções virtuais e vtable
- Criar classes abstratas com funções virtuais puras (`= 0`)
- Sobrecarregar operadores para tipos customizados
- Explicar e aplicar a Regra dos 3/5/0
- Usar herança múltipla para simular interfaces em C++

---

## 1. Herança

```cpp
class Animal {
protected:
    std::string nome;
public:
    Animal(const std::string &nome) : nome(nome) {}
    virtual ~Animal() {}               // destrutor virtual: SEMPRE em classes base

    virtual void faz_som() const {     // virtual permite override
        std::cout << nome << " faz um som\n";
    }
    std::string getNome() const { return nome; }
};

class Cachorro : public Animal {
public:
    Cachorro(const std::string &nome) : Animal(nome) {}

    void faz_som() const override {    // override: avisa o compilador
        std::cout << nome << " late: Au au!\n";
    }
};

class Gato : public Animal {
public:
    Gato(const std::string &nome) : Animal(nome) {}
    void faz_som() const override {
        std::cout << nome << " mia: Miau!\n";
    }
};
```

---

## 2. Polimorfismo

```cpp
// Via ponteiro ou referência para a classe base:
void faz_barulho(Animal &a) {
    a.faz_som();    // chama o método da classe real (em runtime!)
}

Cachorro d("Rex");
Gato     g("Mimi");
faz_barulho(d);   // Rex late: Au au!
faz_barulho(g);   // Mimi mia: Miau!

// Polimorfismo com vetor:
std::vector<Animal*> animais = {new Cachorro("Rex"), new Gato("Mimi")};
for (Animal *a : animais) {
    a->faz_som();    // dispatch dinâmico via vtable
    delete a;
}
```

---

## 3. Funções Virtuais Puras — Classes Abstratas

```cpp
class Forma {
public:
    virtual ~Forma() {}
    virtual double area() const = 0;          // = 0: pura virtual → classe abstrata
    virtual double perimetro() const = 0;
    virtual void imprime() const {            // pode ter implementação
        std::cout << "Área: " << area() << "\n";
    }
};

// Forma não pode ser instanciada diretamente
// Forma f;  → ERRO de compilação

class Circulo : public Forma {
    double r;
public:
    Circulo(double r) : r(r) {}
    double area() const override      { return 3.14159 * r * r; }
    double perimetro() const override { return 2 * 3.14159 * r; }
};
```

---

## 4. Operator Overloading

```cpp
class Vetor2D {
public:
    double x, y;
    Vetor2D(double x, double y) : x(x), y(y) {}

    // Operador +
    Vetor2D operator+(const Vetor2D &o) const {
        return Vetor2D(x + o.x, y + o.y);
    }

    // Operador * (escalar)
    Vetor2D operator*(double s) const {
        return Vetor2D(x * s, y * s);
    }

    // Comparação
    bool operator==(const Vetor2D &o) const {
        return x == o.x && y == o.y;
    }

    // Impressão (friend function)
    friend std::ostream &operator<<(std::ostream &os, const Vetor2D &v) {
        return os << "(" << v.x << ", " << v.y << ")";
    }
};

Vetor2D a(1, 2), b(3, 4);
std::cout << a + b << "\n";   // (4, 6)
std::cout << a * 2 << "\n";   // (2, 4)
```

---

## 5. Herança Múltipla & Interfaces

C++ permite herança múltipla (Java não). Interfaces são classes com apenas métodos virtuais puros:

```cpp
class Serializable {
public:
    virtual std::string serialize() const = 0;
    virtual void deserialize(const std::string &) = 0;
    virtual ~Serializable() {}
};

class Printable {
public:
    virtual void print() const = 0;
    virtual ~Printable() {}
};

class Documento : public Serializable, public Printable {
    std::string conteudo;
public:
    Documento(const std::string &c) : conteudo(c) {}
    std::string serialize() const override { return conteudo; }
    void deserialize(const std::string &s) override { conteudo = s; }
    void print() const override { std::cout << conteudo << "\n"; }
};
```

---

## 6. Regra dos 3/5/0

**Regra dos 3** (antes de C++11): se você define um de {destrutor, copy constructor, copy assignment}, defina os três.

**Regra dos 5** (C++11): adicione move constructor e move assignment.

**Regra do 0**: prefira não definir nenhum — use tipos que gerenciam seus próprios recursos (string, vector, unique_ptr).

```cpp
class Buffer {
    int *data; int size;
public:
    Buffer(int n) : data(new int[n]), size(n) {}
    ~Buffer() { delete[] data; }

    // Copy constructor:
    Buffer(const Buffer &o) : data(new int[o.size]), size(o.size) {
        std::copy(o.data, o.data + size, data);
    }

    // Copy assignment:
    Buffer &operator=(const Buffer &o) {
        if (this == &o) return *this;
        delete[] data;
        data = new int[o.size]; size = o.size;
        std::copy(o.data, o.data + size, data);
        return *this;
    }
};
```

---

## Knowledge Check

Responda sem consultar o material. Se travar, releia a seção correspondente.

1. Por que o destrutor de uma classe base deve ser `virtual`? O que acontece se não for?
2. Qual a diferença entre `override` e apenas redeclarar o método na subclasse?
3. O que é uma função virtual pura? O que torna uma classe abstrata?
4. Como o compilador implementa polimorfismo dinâmico (vtable)?
5. Por que `operator<<` para `std::ostream` geralmente precisa ser `friend`?
6. O que a Regra dos 3 afirma? Dê um exemplo concreto onde violá-la causa bug.
7. Qual a diferença entre herança pública, protegida e privada?
8. O que é slicing? Como ele ocorre e como evitá-lo?

---

## Projeto — Sistema de Formas Geométricas

Implemente uma hierarquia de formas com polimorfismo completo.

**Hierarquia:**
- `Forma` (abstrata): `area()`, `perimetro()`, `nome()`, `operator<<`
- `Circulo`, `Retangulo`, `Triangulo`: implementações concretas
- `FormaComposta`: contém vetor de `Forma*`, soma áreas e perímetros

**Funcionalidades:**
- Criar formas de um arquivo de configuração (formato: `circulo 5.0`, `retangulo 3.0 4.0`)
- Listar todas as formas com área e perímetro
- Ordenar por área (usando `std::sort` com lambda)
- Calcular área total de uma `FormaComposta`

**Requisitos técnicos:**
- Sem memory leaks: usar `std::vector<std::unique_ptr<Forma>>`
- Compilar com `g++ -Wall -Wextra -Werror -std=c++17`
- Destrutor virtual em `Forma`

**Exemplo de execução:**
```
$ ./formas formas.txt
Circulo(r=5)       área=78.54  perímetro=31.42
Retangulo(3x4)     área=12.00  perímetro=14.00
Triangulo(3,4,5)   área= 6.00  perímetro=12.00

Ordenado por área (crescente):
1. Triangulo(3,4,5)  área= 6.00
2. Retangulo(3x4)    área=12.00
3. Circulo(r=5)      área=78.54

Área total: 96.54
```

---

## Exercícios

**ex01:** hierarquia de formas (Forma → Circulo, Retangulo, Triangulo); calcular área total de um vetor de formas
**ex02:** classe `Fracao` com todos os operadores (+,-,*,/,==,!=,<,<<)
**ex03:** interface `Comparable` e `Sortable`; implementar insertion sort genérico via interface

---

## Recursos Adicionais

Estes recursos são **opcionais** mas vão solidificar seu entendimento:

**Para ler/assistir agora:**
- **A Tour of C++** capítulo 4 (Classes) — herança e polimorfismo em ~30 páginas
- **Effective C++** — Scott Meyers, itens 32-41 (herança e OOP)

**Para consulta:**
- [cppreference: virtual functions](https://en.cppreference.com/w/cpp/language/virtual) — semântica completa
- [cppreference: abstract classes](https://en.cppreference.com/w/cpp/language/abstract_class)

**Para ir além:**
- **More Effective C++** — Scott Meyers, item 33 (make non-leaf classes abstract)
- [CppCon "Back to Basics: OOP"](https://www.youtube.com/watch?v=32tDTD9UJCE) — palestra fundamental
