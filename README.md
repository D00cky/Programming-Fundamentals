# Programming Fundamentals

Roadmap de estudo focado em construir uma base sólida de Engenharia de Software, com progressão **C → C++ → Java**. Inspirado na metodologia da 42SP, filosofia do Akita e currículo da IME-USP.

---

```mermaid
flowchart LR
    F0("⚙️ Fase 0\nFundação"):::done
    F1("💻 Fase 1\nC"):::active
    F2("🔩 Fase 2\nSistemas"):::todo
    F3("🟡 Fase 3\nC++"):::todo
    F4("☕ Fase 4\nJava"):::todo
    F5("🧠 Fase 5\nTeoria CS"):::todo
    F6("🏗️ Fase 6\nEng. Software"):::todo

    F0 --> F1 --> F2 --> F3 --> F4 --> F5 --> F6

    classDef done  fill:#2d6a4f,color:#fff,stroke:#1b4332
    classDef active fill:#2d6a9f,color:#fff,stroke:#1a3a5c
    classDef todo  fill:#3a3a3a,color:#aaa,stroke:#555
```

---

## Índice

| Fase | Descrição | Conteúdo |
|------|-----------|----------|
| [⚙️ Fase 0](fase0-fundacao/README.md) | **Fundação da Máquina** — Como tudo funciona antes de escrever uma linha de C | CPU, memória, binário, terminal, git, compilação |
| [💻 Fase 1](fase1-c/README.md) | **C: A Linguagem da Máquina** — Ponteiros, memória, estruturas de dados, algoritmos | libft, ft_printf, push_swap, pipex |
| [🔩 Fase 2](fase2-sistemas/README.md) | **Sistemas** — Processos, threads, sockets no nível do SO | Philosophers, Minishell |
| [🟡 Fase 3](fase3-cpp/README.md) | **C++** — OOP, templates, STL, Modern C++ | Módulos 42, CPP Containers |
| [☕ Fase 4](fase4-java/README.md) | **Java** — OOP empresarial, JVM, design patterns | CRUD, projetos concorrentes |
| [🧠 Fase 5](fase5-teoria-cs/README.md) | **Teoria CS** — Grafos, DP, complexidade computacional | — |
| [🏗️ Fase 6](fase6-eng-software/README.md) | **Engenharia de Software** — SOLID, Clean Code, testes, arquitetura | — |

Para o mapa visual completo com todos os tópicos de cada fase: [roadmap.md](roadmap.md)
