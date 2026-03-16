# 01 — Transição C → C++

---

## 1. O que muda primeiro

```cpp
// C:
#include <stdio.h>
int main() { printf("hello\n"); }

// C++:
#include <iostream>
int main() { std::cout << "hello\n"; }
// Ou: using namespace std;  → cout << "hello\n";
```

Compilar:
```bash
g++ -Wall -Wextra -Werror -std=c++17 main.cpp -o programa
```

---

## 2. Referências

Referências são aliases — outro nome para a mesma variável. Diferente de ponteiros: não podem ser nulas, não podem ser reatribuídas, não precisam de `*`.

```cpp
int x = 10;
int &ref = x;    // ref É x (mesmo endereço de memória)

ref = 99;
std::cout << x;  // 99

// Passagem por referência:
void dobra(int &n) { n *= 2; }
dobra(x);   // x agora é 198 — sem &x, sem *n
```

Const reference — boa prática para parâmetros grandes (sem cópia, sem modificar):
```cpp
void imprime(const std::string &s) {
    std::cout << s << "\n";
}
```

---

## 3. Namespaces

Evitam colisões de nomes em projetos grandes:

```cpp
namespace meu_projeto {
    int valor = 42;
    void func() { /* ... */ }
}

// Uso:
meu_projeto::valor;
meu_projeto::func();

// Importar tudo (evitar em headers!):
using namespace meu_projeto;
```

---

## 4. String em C++

```cpp
#include <string>

std::string s = "hello";
std::string t = "world";

s + " " + t         // concatenação: "hello world"
s.length()          // ou s.size()
s[0]                // 'h' — acesso por índice
s.substr(1, 3)      // "ell" (posição, comprimento)
s.find("llo")       // 2 (índice)
s.find("xyz")       // std::string::npos se não encontrar
s == t              // comparação direta (sem strcmp!)
s.c_str()           // const char* para compatibilidade com C
```

---

## 5. Classes Básicas

```cpp
class Retangulo {
private:               // encapsulamento: membros privados
    double largura;
    double altura;

public:
    // Construtor
    Retangulo(double l, double h) : largura(l), altura(h) {}

    // Métodos
    double area() const { return largura * altura; }
    double perimetro() const { return 2 * (largura + altura); }

    // Getter/Setter
    double getLargura() const { return largura; }
    void   setLargura(double l) { largura = l; }
};

// Uso:
Retangulo r(3.0, 4.0);
std::cout << r.area() << "\n";  // 12
```

---

## 6. RAII — Resource Acquisition Is Initialization

Recursos (memória, arquivos, locks) são adquiridos no construtor e liberados no destrutor. Quando o objeto sai de escopo, o destrutor roda automaticamente.

```cpp
class Buffer {
private:
    int *data;
    int  size;

public:
    Buffer(int n) : size(n) {
        data = new int[n];   // adquire no construtor
    }

    ~Buffer() {              // destrutor: roda automaticamente
        delete[] data;       // libera sempre — sem memory leak possível
    }

    int &operator[](int i) { return data[i]; }
};

void func() {
    Buffer b(100);     // new int[100] chamado
    b[0] = 42;
}   // ← destrutor de b chamado automaticamente: delete[] chamado
```

---

## 7. Streams de I/O

```cpp
#include <iostream>
#include <fstream>
#include <sstream>

// stdout / stderr
std::cout << "valor: " << 42 << std::endl;
std::cerr << "erro: " << msg << "\n";

// Ler do stdin
int n;
std::cin >> n;           // lê int

std::string linha;
std::getline(std::cin, linha);  // lê linha inteira

// Arquivo
std::ifstream fin("entrada.txt");
std::ofstream fout("saida.txt");
std::string word;
while (fin >> word) { /* ... */ }

// String stream (útil para converter tipos)
std::ostringstream oss;
oss << "resultado: " << 3.14;
std::string str = oss.str();
```

---

## 8. new e delete vs malloc/free

```cpp
int  *p  = new int(42);      // aloca + inicializa
int  *arr = new int[100];    // array
delete p;                    // libera escalar
delete[] arr;                // libera array ([] obrigatório!)

// Regra: em C++ moderno, evite new/delete manualmente.
// Use smart pointers (std::unique_ptr, std::shared_ptr) — módulo 04.
```

---

## Exercícios

**ex01:** classe `Complexo` com soma, subtração, módulo, impressão
**ex02:** classe `Pilha<int>` usando array dinâmico com new/delete, RAII
**ex03:** classe `Arquivo` que abre no construtor e fecha no destrutor (RAII para FILE*)

---

## Referências

- **A Tour of C++** — capítulos 1-4
- **cppreference.com** — std::string, std::cout, classes
- **C++ Primer** (5ª ed) — capítulos 1-7
