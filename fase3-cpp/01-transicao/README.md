# 01 — Transição C → C++

> *"C++ is a better C, and then some."*

---

## Antes de começar

Certifique-se de que você já:

- [ ] Escreve C com ponteiros, structs e alocação dinâmica (`fase1-c/`)
- [ ] Entende o que são memory leaks e sabe detectá-los com Valgrind
- [ ] Implementou pelo menos uma estrutura de dados do zero em C
- [ ] Conhece o conceito de escopo de variáveis e funções

---

## O que você vai aprender

Ao final deste módulo você será capaz de:

- Usar referências C++ como alternativa mais segura a ponteiros
- Organizar código com namespaces para evitar colisões de nomes
- Usar `std::string` e I/O streams (`cout`, `cin`, `fstream`)
- Escrever classes com encapsulamento, construtor e destrutor
- Aplicar RAII para gerenciar recursos sem memory leaks
- Distinguir `new`/`delete` de `malloc`/`free` e saber quando usar cada um

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

## Knowledge Check

Responda sem consultar o material. Se travar, releia a seção correspondente.

1. Qual a diferença entre uma referência C++ e um ponteiro? Cite três diferenças.
2. Por que `using namespace std` é desaconselhado em arquivos header?
3. O que é RAII? Por que ele elimina a necessidade de chamar `free()` manualmente?
4. O que acontece se você `delete p` (sem colchetes) em um array alocado com `new int[10]`?
5. Por que `const std::string &s` é preferido a `std::string s` como parâmetro de função?
6. O que o destrutor `~Buffer()` garante que o C puro não garante?
7. Qual a diferença entre `std::cin >> s` e `std::getline(std::cin, s)`?
8. Por que `std::string` é mais seguro que `char *` para manipulação de strings?

---

## Projeto — Calculadora RPN com RAII

Implemente uma calculadora em notação polonesa reversa (RPN) usando C++.

**Funcionalidades:**
- Lê expressões como `3 4 + 2 * 7 /` do stdin
- Operadores suportados: `+`, `-`, `*`, `/`, `%`
- Detecta erros: divisão por zero, operandos insuficientes, token inválido
- Imprime o resultado ou mensagem de erro
- Histórico de cálculos com `h`; sair com `q`

**Requisitos técnicos:**
- Usar classe `Pilha` com RAII (sem memory leaks)
- Usar `std::string` e `std::istringstream` para parsing
- Compilar com `g++ -Wall -Wextra -Werror -std=c++17`
- Zero memory leaks verificado com Valgrind

**Exemplo de execução:**
```
rpn> 3 4 + 2 *
14
rpn> 10 2 /
5
rpn> 5 0 /
Erro: divisão por zero
rpn> 1 +
Erro: operandos insuficientes
rpn> q
Até logo.
```

---

## Exercícios

**ex01:** classe `Complexo` com soma, subtração, módulo, impressão
**ex02:** classe `Pilha<int>` usando array dinâmico com new/delete, RAII
**ex03:** classe `Arquivo` que abre no construtor e fecha no destrutor (RAII para FILE*)

---

## Recursos Adicionais

Estes recursos são **opcionais** mas vão solidificar seu entendimento:

**Para ler/assistir agora:**
- **A Tour of C++** — Bjarne Stroustrup, capítulos 1-4 — introdução concisa pelo criador da linguagem
- **C++ Primer** (5ª ed) — capítulos 1-7 — mais didático e detalhado

**Para consulta:**
- [cppreference.com](https://cppreference.com) — std::string, std::cout, classes, tudo
- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines) — boas práticas oficiais

**Para ir além:**
- **Effective C++** — Scott Meyers, itens 1-20 — armadilhas comuns na transição de C para C++
- [CppCon talks (YouTube)](https://www.youtube.com/user/CppCon) — palestras sobre C++ moderno
