# 04 — JVM Internals

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

## Exercícios

**ex01:** medir impacto do GC: criar e descartar objetos em loop; monitorar com jconsole; forçar GC com System.gc()

**ex02:** comparar performance de G1GC vs ParallelGC para uma tarefa de processamento batch

**ex03:** implementar um ClassLoader customizado que carrega classes de um diretório específico em runtime

---

## Referências

- **Java Performance** — Scott Oaks · O'Reilly
- **Understanding the JVM** — Zhou Zhiming
- **JVM Internals** — blog.jamesdbloom.com (série gratuita)
- OpenJDK source code — github.com/openjdk/jdk
