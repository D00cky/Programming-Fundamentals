# 03 — Templates & STL

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

## Exercícios

**ex01:** usar vector, map, set para contar frequência de palavras num texto
**ex02:** implementar template `MinHeap<T>` do zero (sem usar priority_queue)
**ex03:** reimplementar std::sort usando template + lambda comparator

---

## Referências

- **cppreference.com** — containers, algorithms, iterators
- **Effective STL** — Scott Meyers
- **C++ Primer** capítulos 9-11 (sequential containers, associative containers)
