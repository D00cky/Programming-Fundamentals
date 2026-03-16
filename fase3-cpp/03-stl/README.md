# 03 — Templates & STL

> *"Generic programming is about abstracting and classifying algorithms and data structures."* — Alexander Stepanov

---

## Antes de começar

Certifique-se de que você já:

- [ ] Escreveu classes C++ com herança e polimorfismo (`fase3-cpp/02`)
- [ ] Usa referências, `const`, e `override` corretamente
- [ ] Entende a Regra dos 3 e quando definir destrutor customizado
- [ ] Implementou pelo menos uma estrutura de dados genérica em C

---

## O que você vai aprender

Ao final deste módulo você será capaz de:

- Escrever templates de função e de classe para código genérico
- Usar os containers principais da STL: `vector`, `map`, `unordered_map`, `set`, `stack`, `queue`
- Iterar com range-based for, iteradores e `auto`
- Usar algoritmos de `<algorithm>`: `sort`, `find`, `count_if`, `transform`, `remove_if`
- Escolher o container STL correto para cada situação baseado em complexidade

---

## 1. Templates de Função

Templates permitem escrever código genérico — funciona para qualquer tipo:

```cpp
// Sem template: duplicar código para cada tipo
int max_int(int a, int b)       { return a > b ? a : b; }
double max_double(double a, double b) { return a > b ? a : b; }

// Com template: escreve uma vez, funciona para qualquer tipo comparável
template <typename T>
T max_val(T a, T b) { return a > b ? a : b; }

max_val(3, 5)           // int
max_val(3.14, 2.71)     // double
max_val('a', 'z')       // char
```

---

## 2. Templates de Classe

```cpp
template <typename T>
class Pilha {
private:
    std::vector<T> data;
public:
    void push(const T &val) { data.push_back(val); }
    T    pop()              { T v = data.back(); data.pop_back(); return v; }
    T    peek() const       { return data.back(); }
    bool empty() const      { return data.empty(); }
    int  size()  const      { return data.size(); }
};

Pilha<int>         pi;
Pilha<std::string> ps;
Pilha<double>      pd;
```

---

## 3. Containers da STL

### vector — array dinâmico

```cpp
#include <vector>

std::vector<int> v = {3, 1, 4, 1, 5, 9};
v.push_back(2);        // adiciona no fim: O(1) amortizado
v.pop_back();          // remove do fim: O(1)
v[i]                   // acesso por índice: O(1)
v.at(i)                // acesso com bounds check: O(1)
v.size()               // número de elementos
v.empty()              // true se vazio
v.front() / v.back()   // primeiro / último
v.insert(v.begin() + 2, 99)  // inserção no meio: O(n)
v.erase(v.begin() + 2)       // remoção no meio: O(n)
```

### map — árvore BST ordenada por chave

```cpp
#include <map>

std::map<std::string, int> frequencia;
frequencia["hello"]++;
frequencia["world"]++;
frequencia.count("hello")   // 1 se existe, 0 se não
frequencia.find("hello")    // iterator (ou end() se não existe)
frequencia.erase("world")

// Iterar em ordem de chave:
for (const auto &[chave, valor] : frequencia)
    std::cout << chave << ": " << valor << "\n";
```

### unordered_map — hash table O(1)

```cpp
#include <unordered_map>
std::unordered_map<std::string, int> um;
// Mesma interface que map, mas sem ordenação e O(1) médio
```

### set e unordered_set

```cpp
#include <set>
std::set<int> s = {3, 1, 4, 1, 5};  // {1, 3, 4, 5} — sem duplicatas, ordenado
s.insert(9);
s.count(3)   // 1 ou 0
s.find(3)    // iterator

std::unordered_set<int> us;   // hash set, O(1) médio, sem ordenação
```

### Outros containers

```cpp
std::list<T>        // lista duplamente ligada: insert/erase O(1) dado iterator
std::deque<T>       // fila dupla: push/pop_front e back todos O(1)
std::stack<T>       // adaptador: LIFO sobre deque
std::queue<T>       // adaptador: FIFO sobre deque
std::priority_queue<T>  // max-heap: top() = maior elemento
```

---

## 4. Iteradores

```cpp
std::vector<int> v = {1, 2, 3, 4, 5};

// Forma longa:
for (std::vector<int>::iterator it = v.begin(); it != v.end(); ++it)
    std::cout << *it << " ";

// Com auto:
for (auto it = v.begin(); it != v.end(); ++it)
    std::cout << *it << " ";

// Range-based for (C++11):
for (int x : v)
    std::cout << x << " ";

// Com referência (sem cópia):
for (const auto &x : v)
    std::cout << x << " ";
```

---

## 5. Algorithms

```cpp
#include <algorithm>

std::sort(v.begin(), v.end());                  // ordena in-place: O(n log n)
std::sort(v.begin(), v.end(), std::greater<>());// ordem decrescente
std::sort(v.begin(), v.end(), [](int a, int b){ return a % 2 < b % 2; });

std::find(v.begin(), v.end(), 42);   // iterator ou end()
std::count(v.begin(), v.end(), 3);   // quantas ocorrências de 3
std::min_element(v.begin(), v.end());
std::max_element(v.begin(), v.end());
std::reverse(v.begin(), v.end());
std::unique(v.begin(), v.end());     // remove duplicatas consecutivas (requer sort)

std::accumulate(v.begin(), v.end(), 0);  // soma (require <numeric>)

std::transform(v.begin(), v.end(), v.begin(), [](int x){ return x * 2; });
std::remove_if(v.begin(), v.end(), [](int x){ return x % 2 == 0; });
```

---

## Knowledge Check

Responda sem consultar o material. Se travar, releia a seção correspondente.

1. Qual a diferença entre `std::map` e `std::unordered_map`? Quando usar cada um?
2. Por que `std::vector::push_back` tem complexidade amortizada O(1) e não sempre O(1)?
3. O que é o "erase-remove idiom"? Por que `remove_if` sozinho não remove de um vector?
4. Como um template de função difere de uma função normal em termos de geração de código?
5. Qual a diferença entre `v[i]` e `v.at(i)`? Quando cada um lança exceção?
6. Por que `std::list` raramente é preferível a `std::vector` na prática moderna?
7. O que faz `std::unique`? Por que requer que o container esteja ordenado?
8. Qual container usar para implementar uma fila de prioridade (max-heap)?

---

## Projeto — Analisador de Texto

Implemente um programa que analisa texto de um arquivo e produz estatísticas usando STL.

**Funcionalidades:**
- Ler arquivo de texto passado como argumento
- Contar frequência de cada palavra (case-insensitive, ignorar pontuação)
- Listar as 20 palavras mais frequentes em ordem decrescente
- Listar palavras únicas em ordem alfabética
- Contar total de palavras, palavras únicas e tamanho médio de palavra

**Requisitos técnicos:**
- Usar `std::unordered_map<std::string, int>` para contagem
- Usar `std::sort` com lambda para ordenar por frequência
- Usar `std::set<std::string>` para palavras únicas
- Compilar com `g++ -Wall -Wextra -Werror -std=c++17`

**Exemplo de execução:**
```
$ ./textanalyzer livro.txt
=== Análise de livro.txt ===
Total de palavras:  12.453
Palavras únicas:     2.891
Tamanho médio:       4.7 chars

=== Top 10 mais frequentes ===
 1. the       : 842
 2. and       : 721
 3. a         : 654
...
```

---

## Exercícios

**ex01:** usar vector, map, set para contar frequência de palavras num texto
**ex02:** implementar template `MinHeap<T>` do zero (sem usar priority_queue)
**ex03:** reimplementar std::sort usando template + lambda comparator

---

## Recursos Adicionais

Estes recursos são **opcionais** mas vão solidificar seu entendimento:

**Para ler/assistir agora:**
- [cppreference.com](https://cppreference.com) — containers, algorithms, iterators — a referência definitiva
- **C++ Primer** capítulos 9-11 (sequential containers, associative containers)

**Para consulta:**
- **Effective STL** — Scott Meyers — 50 maneiras de usar a STL corretamente
- Complexidades garantidas de cada container: [cppreference containers](https://en.cppreference.com/w/cpp/container)

**Para ir além:**
- [CppCon "STL Algorithms — Why You Should Use Them"](https://www.youtube.com/watch?v=2olsGf6JIkU) — Sean Parent
- **C++17 The Complete Guide** — Nicolai Josuttis (structured bindings, std::optional, etc.)
