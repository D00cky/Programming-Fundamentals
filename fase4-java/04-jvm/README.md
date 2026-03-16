# 04 — JVM Internals

> *"The JVM is the best virtual machine ever engineered."* — Martin Thompson

---

## Antes de começar

Certifique-se de que você já:

- [ ] Escreveu código Java com herança, generics e concorrência (`fase4-java/01-03`)
- [ ] Entende o que é compilação e bytecode em nível conceitual
- [ ] Usou `ExecutorService` e sabe que threads Java mapeiam para threads do SO
- [ ] Conhece os conceitos de heap e stack de `fase1-c/02`

---

## O que você vai aprender

Ao final deste módulo você será capaz de:

- Explicar as regiões de memória da JVM (Heap, Metaspace, Stack, PC Register)
- Descrever como o Garbage Collector funciona (Mark & Sweep, gerações)
- Distinguir os principais GCs (G1GC, ZGC, ParallelGC) e quando usar cada um
- Entender como o JIT compila bytecode para código nativo (C1/C2, tiered)
- Usar `jmap`, `jstack`, `jconsole` e `async-profiler` para diagnósticos
- Ler bytecode com `javap -c`

---

## 1. O que é a JVM

A JVM é uma máquina virtual que executa bytecode Java. O compilador (`javac`) converte `.java` → `.class` (bytecode). A JVM interpreta ou compila JIT esse bytecode para código nativo da CPU.

```
Código Java → javac → Bytecode (.class) → JVM → Código Nativo
                                          ↑
                               Qualquer SO com JVM instalada
                               (Linux, Windows, macOS, ARM...)
```

---

## 2. Estrutura de Memória da JVM

```
┌─────────────────────────────────────────────────────┐
│  Method Area / Metaspace                            │  Classes, métodos, constantes
│  Heap                                               │  Objetos (GC gerencia aqui)
│    ├── Young Generation                             │
│    │     ├── Eden Space        (novos objetos)      │
│    │     └── Survivor (S0, S1) (objetos sobrevivos) │
│    └── Old Generation          (objetos longos)     │
│  JVM Stack (por thread)                             │  Stack frames, variáveis locais
│  PC Register (por thread)                           │  Próxima instrução a executar
│  Native Method Stack                                │  JNI
└─────────────────────────────────────────────────────┘
```

---

## 3. Garbage Collector

O GC libera memória de objetos sem referências. Você não chama `free()` — o GC faz isso.

**Algoritmo básico — Mark & Sweep:**
1. **Mark:** percorre todas as referências a partir dos roots (stack, statics), marca objetos alcançáveis
2. **Sweep:** varre o heap, libera objetos não marcados

**Stop-The-World:** durante alguns GC pauses, todas as threads de aplicação param. Isso é por que GC pauses afetam latência.

### GCs disponíveis (JDK 17+)

| GC | Uso | Latência | Throughput |
|----|-----|----------|------------|
| G1GC (padrão) | Geral | Baixa | Alta |
| ZGC | Baixíssima latência | <1ms pauses | Boa |
| Parallel GC | Batch/processamento | Alta | Máxima |
| Shenandoah | Baixa latência | <1ms | Boa |

```bash
# Flags JVM para GC:
java -XX:+UseG1GC -Xms512m -Xmx2g Main
java -XX:+UseZGC Main
java -XX:+PrintGCDetails -XX:+PrintGCDateStamps -Xloggc:gc.log Main
```

---

## 4. JIT Compiler

A JVM interpreta bytecode, mas métodos frequentes ("hot") são compilados JIT para código nativo:

- **C1 (Client):** compilação rápida, otimizações básicas — warm-up rápido
- **C2 (Server):** compilação lenta, otimizações agressivas — peak performance
- **Tiered Compilation (padrão):** começa com C1, escala para C2

```bash
# Ver métodos compilados JIT:
java -XX:+PrintCompilation Main

# Desabilitar JIT (interpretado puro, muito lento):
java -Xint Main
```

---

## 5. ClassLoader

Carrega classes `.class` em memória na primeira referência:

```
Bootstrap ClassLoader → carrega classes do JDK (java.lang, etc.)
    ↓ delega para pai primeiro (Parent Delegation Model)
Extension/Platform ClassLoader → módulos do JDK
    ↓
Application ClassLoader → classes do seu projeto (classpath)
    ↓
Custom ClassLoader → carregamento dinâmico (plugins, hot reload)
```

---

## 6. Monitoramento e Profiling

```bash
# Heap dump (para analisar memory leaks):
jmap -dump:format=b,file=heap.hprof <pid>

# Thread dump (para analisar deadlocks, high CPU):
jstack <pid>
kill -3 <pid>   # alternativa no Linux

# Monitoramento visual:
jconsole        # GUI básica — vem com JDK
jvisualvm       # análise de heap, CPU, threads (visualvm.github.io)
async-profiler  # profiler low-overhead para produção

# Flags úteis:
-Xms256m        # heap inicial
-Xmx1g          # heap máximo
-XX:MaxMetaspaceSize=256m
-XX:+HeapDumpOnOutOfMemoryError   # dump automático em OOM
```

---

## 7. bytecode e javap

```bash
# Ver bytecode de uma classe:
javap -c MinhaClasse.class
javap -verbose MinhaClasse.class

# Exemplo: int x = a + b;
# Bytecode:
# iload_1   (push a)
# iload_2   (push b)
# iadd      (soma)
# istore_3  (armazena em x)
```

---

## Knowledge Check

Responda sem consultar o material. Se travar, releia a seção correspondente.

1. Qual a diferença entre Young Generation e Old Generation no heap da JVM?
2. O que é Stop-The-World e por que ele afeta latência de aplicações?
3. Por que ZGC é melhor para aplicações de baixa latência que G1GC?
4. O que o JIT faz que a interpretação pura não faz? Por que é mais rápido?
5. O que é o Parent Delegation Model no ClassLoader? Por que é importante?
6. Como você diagnosticaria um memory leak em uma aplicação Java em produção?
7. O que `jstack` produz? Como identificar um deadlock na saída?
8. O que `-Xms` e `-Xmx` controlam? Por que defini-los iguais pode ser uma boa prática?

---

## Projeto — Benchmark de GC

Implemente um programa que mede o impacto de diferentes configurações de GC.

**Funcionalidades:**
- Alocar e descartar objetos em padrões diferentes (curta vida, longa vida, mix)
- Medir throughput (operações/segundo) e latência (p99) com cada GC
- Gerar relatório comparativo: G1GC vs ParallelGC vs ZGC
- Monitorar com JMX/jconsole durante a execução

**Requisitos técnicos:**
- Usar `System.nanoTime()` para medições precisas
- Executar com flags JVM: `-Xms512m -Xmx512m -XX:+UseG1GC`
- Logging de GC ativo: `-Xlog:gc*:file=gc.log`

**Exemplo de execução:**
```
$ java -Xms512m -Xmx512m -XX:+UseG1GC Benchmark
=== Benchmark de GC — G1GC ===
Throughput: 892.341 ops/s
Latência média: 1.2ms
Latência p99: 8.7ms
GC pauses: 12 vezes, max=45ms

$ java -Xms512m -Xmx512m -XX:+UseZGC Benchmark
=== Benchmark de GC — ZGC ===
Throughput: 756.123 ops/s
Latência média: 0.9ms
Latência p99: 1.1ms
GC pauses: 0 Stop-The-World
```

---

## Exercícios

**ex01:** medir impacto do GC: criar e descartar objetos em loop; monitorar com jconsole; forçar GC com System.gc()

**ex02:** comparar performance de G1GC vs ParallelGC para uma tarefa de processamento batch

**ex03:** implementar um ClassLoader customizado que carrega classes de um diretório específico em runtime

---

## Recursos Adicionais

Estes recursos são **opcionais** mas vão solidificar seu entendimento:

**Para ler/assistir agora:**
- **Java Performance** — Scott Oaks (O'Reilly) — GC, JIT, profiling em profundidade
- [JVM Internals](https://blog.jamesdbloom.com/JVMInternals.html) — série de artigos gratuitos

**Para consulta:**
- [Understanding the JVM Advanced Features and Best Practices](https://www.oracle.com/technical-resources/articles/java/architect-evans-pt1.html)
- [OpenJDK source code](https://github.com/openjdk/jdk) — código-fonte da JVM

**Para ir além:**
- **Understanding the JVM** — Zhou Zhiming — análise profunda da implementação da JVM
- [async-profiler](https://github.com/async-profiler/async-profiler) — profiler de produção com flame graphs
