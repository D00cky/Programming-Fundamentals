# 04 — Modern C++ (C++11/14/17/20)

> *"Effective C++ is mostly about avoiding pitfalls. Effective Modern C++ is about using new features effectively."* — Scott Meyers

---

## Antes de começar

Certifique-se de que você já:

- [ ] Usa herança, polimorfismo e templates C++ (`fase3-cpp/02` e `03`)
- [ ] Implementou a Regra dos 3 e entende copy constructor e copy assignment
- [ ] Usa `std::vector`, `std::map` e algoritmos da STL
- [ ] Entende o problema de memory leaks e como RAII os resolve

---

## O que você vai aprender

Ao final deste módulo você será capaz de:

- Usar `auto` e `decltype` para inferência de tipos
- Gerenciar memória com `unique_ptr` e `shared_ptr` sem `new`/`delete` manual
- Escrever lambdas com captura por valor e por referência
- Implementar move semantics com rvalue references (`&&`)
- Usar `std::optional`, `std::variant` e `constexpr` (C++17)
- Aplicar `if constexpr` para código condicional em tempo de compilação

---

## 1. auto e Type Inference

```cpp
auto i    = 42;               // int
auto d    = 3.14;             // double
auto s    = std::string("hi");// std::string
auto v    = std::vector<int>{1,2,3};  // std::vector<int>

// Especialmente útil com iterators:
auto it = mapa.begin();       // em vez de: std::map<std::string,int>::iterator it

// decltype: tipo de uma expressão
int x = 5;
decltype(x) y = 10;          // int y = 10
```

---

## 2. Smart Pointers — Adeus, Memory Leaks

### unique_ptr — propriedade exclusiva

```cpp
#include <memory>

std::unique_ptr<int> p = std::make_unique<int>(42);
*p = 99;                       // usa como ponteiro normal
p.get()                        // ponteiro raw subjacente
// Sem necessidade de delete — destrutor limpa automaticamente

// Não pode ser copiado, só movido:
auto p2 = std::move(p);        // p agora é nullptr, p2 possui o recurso
```

### shared_ptr — propriedade compartilhada (reference counting)

```cpp
auto sp1 = std::make_shared<std::string>("hello");
auto sp2 = sp1;                // ambos compartilham a mesma string
sp1.use_count()                // 2 — dois donos
// Libera quando o último shared_ptr é destruído

// weak_ptr: referência sem propriedade (evita ciclos):
std::weak_ptr<std::string> wp = sp1;
if (auto locked = wp.lock())   // lock() retorna shared_ptr ou nullptr
    std::cout << *locked;
```

**Regra:** prefira `unique_ptr`. Use `shared_ptr` só quando precisar de propriedade compartilhada. Evite `new`/`delete` em código novo.

---

## 3. Lambdas

Funções anônimas — objetos callable criados inline:

```cpp
// Sintaxe: [captura](parâmetros) -> retorno { corpo }
auto soma = [](int a, int b) { return a + b; };
soma(3, 4)  // 7

// Captura de variáveis:
int offset = 10;
auto adiciona_offset = [offset](int x) { return x + offset; };  // por valor
auto incrementa     = [&offset](int x) { offset++; return x; }; // por referência

// Com algoritmos:
std::vector<int> v = {5, 2, 8, 1, 9};
std::sort(v.begin(), v.end(), [](int a, int b){ return a > b; }); // decrescente

auto pares = std::count_if(v.begin(), v.end(), [](int x){ return x % 2 == 0; });
```

---

## 4. Move Semantics

Permite transferir recursos sem copiar — essencial para performance:

```cpp
// Sem move: cópia cara (aloca nova memória, copia bytes)
std::vector<int> v1 = {1,2,3,...};    // 1 milhão de elementos
std::vector<int> v2 = v1;             // CÓPIA — duplica a memória

// Com move: transfer de propriedade — O(1), sem cópia
std::vector<int> v3 = std::move(v1);  // v1 agora está vazio, v3 tem os dados

// Move constructor em classe própria:
class Buffer {
    int *data; int size;
public:
    Buffer(Buffer &&other) noexcept   // && = rvalue reference
        : data(other.data), size(other.size) {
        other.data = nullptr;         // invalida o objeto movido
        other.size = 0;
    }
};
```

---

## 5. Range-based for e Structured Bindings

```cpp
// Range-based for (C++11):
for (const auto &elem : container) { /* ... */ }

// Structured bindings (C++17) — decompor pares e tuples:
std::map<std::string, int> m = {{"a", 1}, {"b", 2}};
for (const auto &[chave, valor] : m)
    std::cout << chave << "=" << valor << "\n";

auto [x, y] = std::make_pair(10, 20);
```

---

## 6. Novas Features Úteis

```cpp
// nullptr (substitui NULL — type-safe):
int *p = nullptr;           // em vez de int *p = NULL;

// override e final:
virtual void func() override;  // erro de compilação se não sobrescreve
class Concreta final {};       // não pode ser herdada

// constexpr — avaliado em tempo de compilação:
constexpr int fatorial(int n) {
    return n <= 1 ? 1 : n * fatorial(n - 1);
}
constexpr int f5 = fatorial(5);  // calculado em compile time!

// std::optional (C++17) — valor que pode estar ausente:
#include <optional>
std::optional<int> busca(int val) {
    if (encontrou) return val;
    return std::nullopt;
}
if (auto r = busca(42)) std::cout << *r;

// std::variant (C++17) — union type-safe:
std::variant<int, double, std::string> v = "hello";
std::get<std::string>(v)    // "hello"
```

---

## 7. if constexpr — Compilação Condicional

```cpp
template <typename T>
void processa(T val) {
    if constexpr (std::is_integral_v<T>)
        std::cout << "Inteiro: " << val << "\n";
    else if constexpr (std::is_floating_point_v<T>)
        std::cout << "Float: " << val << "\n";
    else
        std::cout << "Outro\n";
}
```

---

## Knowledge Check

Responda sem consultar o material. Se travar, releia a seção correspondente.

1. Qual a diferença entre `unique_ptr` e `shared_ptr`? Quando usar cada um?
2. O que `std::move()` faz? O estado do objeto movido é garantido?
3. O que `[=]` vs `[&]` na captura de uma lambda significa?
4. Por que `make_unique<T>()` é preferível a `new T()` diretamente?
5. O que `weak_ptr` resolve que `shared_ptr` não resolve sozinho?
6. Como `constexpr` difere de `const`?
7. O que é uma rvalue reference (`&&`)? Como o compilador decide se usa move ou copy?
8. O que `std::optional` resolve que `nullptr` não resolve elegantemente?

---

## Projeto — Factory com Smart Pointers

Implemente um sistema de plugins usando factory method + unique_ptr.

**Funcionalidades:**
- Interface abstrata `Plugin` com `nome()`, `executar(std::string input)`, `resultado()`
- Implementações: `PluginUpper` (maiúsculas), `PluginReverse` (inverter string), `PluginWordCount` (contar palavras)
- Factory `PluginFactory::criar(std::string tipo)` → `unique_ptr<Plugin>`
- Pipeline: encadear plugins em sequência, passando saída de um para entrada do próximo

**Requisitos técnicos:**
- Sem `new`/`delete` explícito — apenas `make_unique`
- Usar lambdas para filtros opcionais no pipeline
- Compilar com `g++ -Wall -Wextra -Werror -std=c++17`
- Zero memory leaks

**Exemplo de execução:**
```
$ ./plugins
pipeline> upper | reverse | wordcount
input: hello world foo bar
[upper]     → HELLO WORLD FOO BAR
[reverse]   → RAB OOF DLROW OLLEH
[wordcount] → 4 palavras
```

---

## Exercícios

**ex01:** refatorar a classe Buffer do módulo 01 usando unique_ptr — eliminar o destrutor manual
**ex02:** factory function que retorna unique_ptr<Forma> dado um tipo string ("circulo", "retangulo")
**ex03:** pipeline de transformações com lambdas: filtrar, mapear, reduzir um vetor

---

## Recursos Adicionais

Estes recursos são **opcionais** mas vão solidificar seu entendimento:

**Para ler/assistir agora:**
- **Effective Modern C++** — Scott Meyers — os 42 itens essenciais do C++ moderno
- [CppCon "Back to Basics: Move Semantics"](https://www.youtube.com/watch?v=St0MNEU5b0o) — Klaus Iglberger

**Para consulta:**
- [cppreference: smart pointers](https://en.cppreference.com/w/cpp/memory) — unique_ptr, shared_ptr, weak_ptr
- [cppreference: lambdas](https://en.cppreference.com/w/cpp/language/lambda) — sintaxe e captura

**Para ir além:**
- **C++17 The Complete Guide** — Nicolai Josuttis — std::optional, std::variant, std::any
- [CppCon "Cpp Type Erasure"](https://www.youtube.com/watch?v=tbUCHifyT24) — padrão avançado usando concepts
