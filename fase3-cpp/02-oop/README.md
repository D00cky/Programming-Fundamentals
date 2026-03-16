# 02 — OOP: Herança, Polimorfismo & Interfaces

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

## Exercícios

**ex01:** hierarquia de formas (Forma → Circulo, Retangulo, Triangulo); calcular área total de um vetor de formas
**ex02:** classe `Fracao` com todos os operadores (+,-,*,/,==,!=,<,<<)
**ex03:** interface `Comparable` e `Sortable`; implementar insertion sort genérico via interface

---

## Referências

- **A Tour of C++** capítulo 4 (Classes)
- **Effective C++** — Scott Meyers, itens 32-41 (herança e OOP)
- cppreference.com: virtual functions, abstract classes
