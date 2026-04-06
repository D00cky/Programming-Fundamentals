# IA no Aprendizado de Programação

> *"IA não substitui o programador que entende o que está fazendo. Ela comprime quem não entende."* — Fabio Akita

---

## Antes de começar

- [ ] Você já leu os módulos 01 a 06 desta fase (ou entende pelo menos o básico de terminal, compilação e git)
- [ ] Você tem acesso a algum assistente de IA (Claude, ChatGPT ou similar)
- [ ] Você já tentou usar IA para resolver algum exercício e ficou confuso com o resultado

---

## O que você vai aprender

- Por que IA é ferramenta, não substituto de fundamentos
- O que é "Vibe Coding" e por que ele tem limites reais
- Por que o mito do "one-shot prompt" pode destruir seu aprendizado
- Como usar IA para estudar sem parar de pensar
- Quais LLMs funcionam de verdade para código (e quais inventam APIs)
- Como proteger seu sistema ao usar agentes de IA
- O que o mercado de trabalho espera de juniors em 2026

---

## O contexto: Akita e a IA em 2026

Fabio Akita passou **37 dias** de imersão em Vibe Coding no início de 2026 — mais de 650 commits, ~144 mil linhas de código, quase 10 projetos publicados. A conclusão não foi "IA é inútil" nem "IA faz tudo". Foi mais sutil e mais importante para quem está aprendendo.

### O que é Vibe Coding

Vibe Coding é o uso de agentes de IA (Claude Code, Cursor, Windsurf etc.) para gerar e iterar código em alta velocidade, com o desenvolvedor no papel de diretor mais do que de digitador. O nome vem da sensação de estar "no flow" sem escrever cada linha manualmente.

Isso funciona. Mas tem implicações sérias para quem ainda está aprendendo.

---

## As lições mais importantes

### 1. Velocidade virou commodity — o que importa agora é entendimento

Antes de 2024, "saber programar rápido" era diferencial. Hoje qualquer um com um agente de IA consegue gerar 10 mil linhas de código em dias. O que **não** se commoditizou:

- Saber **o que** construir (entender o problema de verdade)
- Saber **por que** uma solução funciona (ou vai falhar em produção)
- Saber **revisar** o código que a IA gerou (e encontrar os bugs que ela não vê)
- Saber **iterar** quando o primeiro prompt não resolve

Se você usa IA para copiar respostas sem entender, você não está aprendendo programação — está aprendendo a copiar respostas.

### 2. One-shot prompt é um mito

A fantasia do "prompt perfeito que gera um sistema completo e funcional" não existe na prática. Akita documentou isso ao acompanhar quatro projetos pós-deploy:

- **The M.Akita Chronicles**: estava "pronto" com 274 commits e 1.323 testes. Acabou com 335 commits e 1.422 testes.
- **Frank Sherlock**: foi de v0.1 a v0.7 com adição de detecção de rostos e outras features
- **FrankMega**: construído em um dia, mas exigiu correções assim que usuários reais apareceram

Software nunca está "pronto". E isso se aplica duplamente ao código gerado por IA, que não tem como antecipar edge cases do mundo real.

**Para o seu aprendizado:** quando a IA resolver um exercício para você de primeira, desconfie. Tente mudar os dados de entrada. Tente um caso limite. Entenda por que funcionou.

### 3. IA não vai substituir programadores — vai comprimir o mercado junior

A posição do Akita é precisa: *"IA não vai substituir programadores como eu"*. O que vai acontecer é diferente:

- O mercado de **juniors** vai encolher, pois empresas usam IA para as tarefas de entrada
- Com menos juniors, há menos seniorização natural no futuro
- Os que conseguirem crescer são os que entenderem fundamentos de verdade

O caminho do junior de 2026 não é mais "faço tarefas simples até vir experiência". É: **construir entendimento real desde o início**, porque as tarefas simples já têm automação.

### 4. A IA erra — e erra confiante

Nos benchmarks feitos por Akita em abril de 2026 testando os principais LLMs em um desafio real de código:

| Modelo | Resultado |
|--------|-----------|
| Claude Sonnet 4.6 / Opus 4.6 | ✅ Funcionou |
| GLM 5 / 5.1 (Z.AI) | ✅ Funcionou (~89% mais barato que Opus) |
| GPT 5.4 | ✅ Funcionou |
| Kimi, DeepSeek, MiniMax, Qwen, Gemini, Grok 4.20 | ❌ Inventaram APIs que não existem |

Modelos que falham não falham timidamente — eles geram código que parece correto, compila, mas usa funções que não existem na biblioteca. Sem fundamentos para revisar o resultado, você não percebe o problema.

### 5. Como usar IA para aprender (não para copiar)

**Fluxo ruim:**
```
Não entendo o exercício → peço para a IA resolver → copio → sigo em frente
```

**Fluxo bom:**
```
Tento resolver → trava em algo específico → pergunto para a IA sobre aquele conceito
→ entendo a explicação → volto ao código → resolvo eu mesmo
```

Formas produtivas de usar IA no estudo:

- **"Explica esse conceito como se eu nunca tivesse visto ponteiros"** — melhor que ler documentação fria
- **"O que está errado nesse código? Não me dê a resposta, me dê uma dica"** — mantém você pensando
- **"Quais são os casos de teste que minha implementação ignora?"** — expande sua visão
- **"Por que essa abordagem é mais lenta que aquela?"** — vai fundo em complexidade

Evite:
- Pedir código completo antes de tentar
- Aceitar a primeira resposta sem questionar
- Usar IA para gerar código em linguagem que você não sabe ler

### 6. Segurança com agentes de IA

Agentes como Claude Code, Cursor e Windsurf precisam de acesso ao seu sistema para funcionar: leem arquivos, executam compiladores, rodam scripts. Isso cria vetores de risco.

Boas práticas:
- Use **bubblewrap** ou containers para isolar agentes em projetos sensíveis
- Revise os comandos que o agente propõe antes de confirmar (especialmente `rm`, `git push`, chamadas de rede)
- Nunca deixe um agente com acesso irrestrito ao seu sistema de arquivos inteiro
- Lembre-se: o agente pode vazar dados para APIs externas dependendo de como está configurado

O projeto [ai-jail](https://github.com/akitaonrails/ai-jail) do próprio Akita implementa esse sandboxing. Vale estudar.

---

## Como a IA se encaixa nesse roadmap

| Fase | Use IA para | Não use IA para |
|------|-------------|-----------------|
| Fase 0 | Explicar conceitos de hardware, SO | Substituir a leitura |
| Fase 1 (C) | Revisar sua implementação, explicar erros de segfault | Escrever o código por você |
| Fase 2 (Sistemas) | Depurar race conditions, entender syscalls | Implementar processos/threads sem entender |
| Fase 3 (C++) | Navegar a STL, entender templates | Copiar padrões sem saber o porquê |
| Fase 4 (Java) | Gerar boilerplate, checar design patterns | Substituir o entendimento de OOP |
| Fase 5 (Teoria) | Visualizar algoritmos, checar provas | Resolver exercícios de complexidade |
| Fase 6 (Eng.) | Revisar code review, sugerir testes | Tomar decisões de arquitetura por você |

---

## Knowledge Check

1. Por que "velocidade de execução" deixou de ser diferencial competitivo para programadores em 2026?
2. O que acontece com o mercado de juniors quando empresas passam a usar IA para tarefas de entrada?
3. O que é o mito do "one-shot prompt" e como ele se manifesta no aprendizado?
4. Por que um LLM que gera código com erro pode ser mais perigoso do que um que simplesmente falha?
5. Qual é a diferença entre usar IA para **copiar** código e usar IA para **entender** código?
6. Por que agentes de IA representam um risco de segurança e como mitigar isso?
7. Em que tipo de tarefa a IA é mais útil ao longo desse roadmap? Em que tipo ela atrapalha mais?
8. Se um LLM resolver seu exercício de C na primeira tentativa, que passos você deve tomar antes de considerar o exercício concluído?
9. Como o fato de "software nunca estar pronto" muda a forma de avaliar o que você aprende no curso?
10. Por que Akita diz que IA não vai substituir programadores como ele, mas vai comprimir o mercado de iniciantes?

---

## Projeto

### Experimento de Calibração de IA

**Objetivo:** Desenvolver senso crítico sobre saídas de IA através de experimentação deliberada.

**Parte 1 — O teste da confiança:**

Pegue um exercício do módulo `fase1-c/01-fundamentos` que você **já resolveu**. Peça para um LLM resolvê-lo. Compare:
- A solução é diferente da sua? Em que aspectos?
- A solução da IA funciona para todos os casos de teste que você testou?
- A IA cometeu algum erro que você consegue identificar por ter resolvido antes?

**Parte 2 — O teste do limite:**

Invente um caso de teste que quebre a solução da IA. Se não conseguir, pergunte à IA: *"Quais inputs podem fazer essa solução falhar?"*. Implemente a correção você mesmo.

**Parte 3 — O diário:**

Crie um arquivo `ia-journal.md` na raiz do seu fork desse repositório. Durante a Fase 1 inteira, registre:
- Quando você usou IA
- Para quê usou
- Se a resposta estava correta
- O que você aprendeu com isso (mesmo quando a IA errou)

**Critério de sucesso:** Ao final da Fase 1, você consegue identificar quando a IA está errada sem precisar testar o código.

---

## Recursos Adicionais

### Para ler/assistir agora

- [RANT: IA acabou com os programadores?](https://akitaonrails.com/2026/02/08/rant-ia-acabou-com-programadores) — Akita (2026)
- [RANT: o Akita abriu as pernas pra IA??](https://akitaonrails.com/2026/02/24/rant-o-akita-abriu-as-pernas-pra-ia) — Akita (2026)
- [37 dias de Imersão em Vibe Coding: Conclusão](https://akitaonrails.com/2026/03/05/37-dias-de-imers%C3%A3o-em-vibe-coding-conclus%C3%A3o-quanto-a-modelos-de-neg%C3%B3cio) — Akita (2026)
- [Software Nunca Está 'Pronto' — Por Que One-Shot Prompt É Mito](https://akitaonrails.com/2026/03/01/software-nunca-esta-pronto-4-projetos-a-vida-pos-deploy-e-por-que-one-shot-prompt-e-mito) — Akita (2026)

### Para consulta

- [AI Agents: Garantindo a Proteção do seu Sistema](https://akitaonrails.com/2026/01/10/ai-agents-garantindo-a-protecao-do-seu-sistema) — sandboxing com Bubblewrap
- [Testando LLMs Open Source e Comerciais](https://akitaonrails.com/2026/04/05/testando-llms-open-source-e-comerciais-quem-consegue-bater-o-claude-opus) — qual LLM usar para código
- [AI Agents: Qual é o melhor?](https://akitaonrails.com/2026/01/24/ai-agents-qual-e-o-melhor-opencode-crush-claude-code-gpt-codex-gopilot-cursor-windsurf-antigravity) — comparativo de agentes de código

### Para ir além

- [Vibe Code: Do Zero à Produção em 6 DIAS](https://akitaonrails.com/2026/02/16/vibe-code-do-zero-a-producao-em-6-dias-the-m-akita-chronicles) — workflow real com IA
- [AI Agents: Qual seria a melhor Linguagem para LLMs?](https://akitaonrails.com/2026/02/09/ai-agents-qual-seria-a-melhor-linguagem-de-programacao-para-llms) — por que LLMs preferem certas linguagens
- [O código fonte do Claude Code vazou](https://akitaonrails.com/2026/03/31/codigo-fonte-do-claude-code-vazou-o-que-achamos-dentro) — o que a arquitetura de um agente de IA revela
