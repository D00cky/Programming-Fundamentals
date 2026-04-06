# IA no Aprendizado de Programação

> *"IA não substitui o programador que entende o que está fazendo. Ela comprime quem não entende."* — Fabio Akita

---

## Antes de começar

- [ ] Você já leu os módulos 01 a 06 desta fase (ou entende pelo menos o básico de terminal, compilação e git)
- [ ] Você tem acesso a algum assistente de IA (Claude, ChatGPT ou similar)
- [ ] Você já tentou usar IA para resolver algum exercício e ficou confuso com o resultado

---

## O que você vai aprender

- O que é Vibe Coding e por que ele tem limites reais para quem está aprendendo
- Por que o mito do "one-shot prompt" existe e como ele engana desenvolvedores
- O que a IA faz bem, o que ela faz mal, e o que ela não consegue fazer de jeito nenhum
- Como estruturar seu trabalho com IA para aprender de verdade (não só copiar)
- Qual LLM usar para código — e a resposta honesta do Akita sobre isso
- Como proteger seu sistema ao rodar agentes de IA localmente
- O que acontece com o mercado de trabalho para juniors em 2026

---

## O contexto: 37 dias de imersão real

Fabio Akita passou **37 dias** de imersão em Vibe Coding no início de 2026 — mais de 650 commits, ~144 mil linhas de código, quase 10 projetos publicados e em produção. Os projetos incluíam desde um editor de Markdown (FrankMD), um indexador de imagens com visão computacional (Frank Sherlock), um clone do Mega em Rails, um sistema de data mining, um gateway de IA reescrito em Rust (FrankClaw), até uma migração de 10 mil linhas de Python para Crystal.

A conclusão não foi "IA é inútil" nem "IA faz tudo". Foi mais sutil — e mais importante para quem está aprendendo.

### O que é Vibe Coding

Vibe Coding é o uso de agentes de IA (Claude Code, Cursor, Windsurf, OpenCode etc.) para gerar e iterar código em alta velocidade, com o desenvolvedor no papel de diretor mais do que de digitador. O nome vem da sensação de estar "no flow" sem escrever cada linha manualmente.

Isso funciona. Mas tem implicações sérias para quem ainda está aprendendo.

---

## O que a IA faz bem

A experiência dos projetos do Akita documenta onde a IA realmente entrega valor:

- **Boilerplate e scaffolding**: gerar estrutura de arquivos, classes, configurações iniciais
- **Tradução entre linguagens**: mapear padrões conhecidos de Python para Crystal, de C para Rust — quando o domínio do problema é claro
- **Testes unitários**: gerar casos de teste a partir de especificações bem definidas
- **Documentação**: transformar código em comentários e README
- **Refactoring estrutural**: renomear, reorganizar, extrair funções — quando a semântica é preservada
- **Tarefas que o dev acha chatas**: templates HTML/CSS, formatação, configs repetitivas — exatamente onde a IA remove o atrito que atrasava o início de projetos

Em 48 horas de trabalho, o Frank Sherlock saiu do zero para uma aplicação desktop cross-platform com 338 testes automatizados, binários para Linux/macOS/Windows, e integração com modelos de visão locais. Isso antes seria impossível para uma pessoa sozinha no prazo.

---

## O que a IA faz mal

Da mesma série de projetos, os pontos onde a IA falha ou precisa de supervisão constante:

**Decisões arquiteturais**: a IA gera soluções para o problema que você descreveu, não necessariamente para o problema que você tem. Akita teve que intervir manualmente em integração de sistemas, escolhas de banco de dados e estrutura de monorepo.

**Segurança e hardening**: o FrankClaw funcionava para casos simples, mas "segurança é onde o diferencial está" — prevenção de injection attacks, memory safety e privilege escalation requerem julgamento humano que a IA não consegue antecipar.

**Edge cases de produção**: o FrankMega foi construído em um dia e parecia pronto. Quando usuários reais apareceram, os edge cases começaram. A IA não consegue antecipar o que usuários vão fazer.

**Escolher a ferramenta errada com confiança**: no projeto de detecção de balões de manga, Akita passou dias tentando fazer OpenCV funcionar com heurísticas manuais (CLAHE, edge detection, watershed). A IA ajudava a implementar — mas a abordagem inteira estava errada. A solução correta era um modelo pré-treinado (RT-DETR-v2), que resolveu em horas o que dias de heurística não resolveram. **A IA implementa bem a abordagem errada sem avisar que é errada.**

**Infraestrutura em produção real**: ao migrar seu home server com Claude Code, Akita conseguiu resultados práticos — mas concluiu: *"se fosse um servidor de produção real, eu jamais faria isso sem review humano rigoroso em cada passo"*. SELinux + Docker foi o ponto de ruptura onde o conhecimento especializado foi insubstituível.

**Testes de integração**: testes unitários são a zona de conforto. Falhas reais emergem na integração — quando o formato do RSS muda, quando rate limits diferem entre ambientes, quando a serialização de dados não bate. Nenhum LLM antecipa isso sem dados de produção reais.

---

## As lições mais importantes

### 1. A pergunta errada revela o problema certo

Quando Akita publicou *"Vibe Code: Qual LLM é a MELHOR??"*, a resposta honesta foi: **"NENHUMA!"**

A própria pergunta revela um mal-entendido sobre desenvolvimento de software. Nenhum LLM elimina a necessidade de julgamento arquitetural, entendimento do domínio e garantia de qualidade. O sucesso com IA correlaciona diretamente com a competência técnica prévia do desenvolvedor — a IA amplifica o que você já sabe, não substitui o que você não sabe.

Para benchmarks de código específicos (abril de 2026, Akita testando todos os principais LLMs):

| Modelo | Resultado |
|--------|-----------|
| Claude Sonnet 4.6 / Opus 4.6 | ✅ Funcionou |
| GLM 5 / 5.1 (Z.AI) | ✅ Funcionou (~89% mais barato que Opus) |
| GPT 5.4 | ✅ Funcionou |
| Kimi, DeepSeek, MiniMax, Qwen, Gemini, Grok 4.20 | ❌ Inventaram APIs que não existem |

Modelos que falham não falham timidamente — geram código que parece correto, compila, mas usa funções que não existem na biblioteca. Sem fundamentos para revisar o resultado, você não percebe o problema.

No desafio de Zig (janeiro de 2026), o alerta foi diferente: o Akita comparou os principais LLMs num problema *"relativamente simples e curto"*. A lição foi não generalizar de um único benchmark. Performance varia dramaticamente com complexidade, domínio e contexto — teste sempre contra seu caso de uso específico.

### 2. One-shot prompt é um mito

A fantasia do "prompt perfeito que gera um sistema completo e funcional" não existe. Akita documentou isso acompanhando quatro projetos pós-deploy:

- **The M.Akita Chronicles**: estava "pronto" com 274 commits e 1.323 testes. Acabou com 335 commits e 1.422 testes. *"125 commits de pós-produção depois, posso confirmar: software 'done' não existe."*
- **Frank Sherlock**: foi de v0.1 a v0.7 com adição de face detection e outras features
- **FrankMD**: atraiu contribuidores externos e lançou v0.2.0
- **FrankMega**: construído em um dia, exigiu correções assim que usuários reais apareceram

Para o seu aprendizado: quando a IA resolver um exercício de primeira, desconfie. Mude os dados de entrada. Teste um caso limite. Entenda por que funcionou antes de seguir em frente.

### 3. Sunk cost: saber quando mudar de abordagem

Um dos maiores riscos com IA é implementar bem a coisa errada por muito tempo. A IA é ótima em ajudar a persistir numa abordagem — e isso pode ser um problema quando a abordagem inteira está errada.

Sinais de que você precisa pivotar (não otimizar):
- Você está adicionando workarounds em cima de workarounds
- A solução funciona para os casos que você testou mas quebra em casos novos
- Você está lutando contra a ferramenta em vez de resolver o problema

O antídoto: antes de implementar qualquer coisa, pesquise se já existe solução estabelecida para esse tipo de problema. Modelos pré-treinados, bibliotecas especializadas e algoritmos conhecidos costumam superar heurísticas customizadas — mesmo que a IA consiga implementar as heurísticas com perfeição.

### 4. IA não vai substituir programadores — vai comprimir o mercado junior

A posição do Akita é precisa: *"IA não vai substituir programadores como eu"*. O que vai acontecer é diferente:

- O mercado de **juniors** vai encolher, pois empresas usam IA para as tarefas de entrada
- Com menos juniors entrando, há menos seniorização natural no futuro
- Os que conseguirem crescer são os que entenderem fundamentos de verdade — não os que souberem usar o melhor agente

O caminho do junior de 2026 não é mais *"faço tarefas simples até vir experiência"*. As tarefas simples já têm automação. O diferencial é entendimento real desde o início.

### 5. Questione os defaults — inclusive os que a IA sugere

Ao construir o frontend do M.Akita Chronicles sem nenhum framework JavaScript (usando Tailwind v4 + Hugo + Hextra), Akita demonstrou algo importante: o mercado conflate "frontend" com "framework", mas soluções mais simples frequentemente funcionam melhor.

A IA vai sugerir o framework mais popular. Mas o framework mais popular pode não ser a ferramenta certa para o problema. **A pergunta correta é sempre: qual são os requisitos reais?** No caso do M.Akita Chronicles, o frontend precisava funcionar em blog, email e RSS simultaneamente — frameworks JS criariam mais problemas do que resolveriam.

---

## Como trabalhar com IA (não para, com)

### O workflow que funciona em produção

Akita documentou o processo real: *"Do zero à produção em 1 semana — 274 commits, 8 dias, 4 aplicações, 1.323 testes"*. O segredo não foi prompts mágicos. Foi disciplina:

1. **Especificação antes de geração**: o arquivo `CLAUDE.md` (ou equivalente) funciona como documento vivo de especificação. O agente consulta esse arquivo a cada sessão. Quanto mais claro o contexto, melhor o output.

2. **TDD como guia**: testes abrangentes funcionam como restrições formais para a IA. Com 1.323 testes, cada geração de código era validada automaticamente. A IA não tem como antecipar edge cases sem testes — mas falha nos testes de forma óbvia.

3. **Dados reais, não mocks**: testes unitários com mocks dão falsa confiança. Falhas reais de integração só aparecem com dados de produção. Akita usou rsync para sincronizar dados de produção no ambiente de teste.

4. **Iteração, não perfeição**: o par programador-IA trabalha em refinamento contínuo. Nenhuma interação é isolada — cada output informa o próximo prompt.

5. **Idempotência sempre**: operações de scraping, processamento de dados e jobs assíncronos devem poder re-executar sem criar duplicatas. A IA vai gerar código que funciona na primeira execução e quebra na segunda se você não especificar isso explicitamente.

### Prompts que funcionam no estudo

**Fluxo ruim:**
```
Não entendo o exercício → peço para a IA resolver → copio → sigo em frente
```

**Fluxo bom:**
```
Tento resolver → trava em algo específico → pergunto sobre aquele conceito
→ entendo → volto ao código → resolvo eu mesmo
```

Perguntas que funcionam:
- *"Explica esse conceito como se eu nunca tivesse visto ponteiros"*
- *"O que está errado nesse código? Não me dê a resposta, me dê uma dica"*
- *"Quais casos de teste minha implementação ignora?"*
- *"Por que essa abordagem é O(n²) e existe uma O(n log n)?"*
- *"Pesquisa se já existe uma solução estabelecida para esse tipo de problema antes de eu implementar do zero"*

Evite:
- Pedir código completo antes de tentar
- Aceitar a primeira resposta sem questionar
- Usar IA para gerar código em linguagem que você não sabe ler
- Continuar otimizando quando a abordagem toda pode estar errada

---

## Segurança ao usar agentes de IA

Agentes como Claude Code, Cursor e Windsurf precisam de acesso extenso ao seu sistema: leem arquivos, executam compiladores, rodam scripts, fazem chamadas de rede. Isso cria vetores de risco reais.

### Camadas de proteção

**Camada 1 — Sandboxing no nível do SO:**

- **Linux**: [Bubblewrap](https://github.com/containers/bubblewrap) cria namespaces isolados, restringindo acesso ao filesystem, rede e capacidades de processo. Apenas diretórios explicitamente permitidos ficam acessíveis.
- **macOS**: `sandbox-exec` oferece isolamento similar via framework da Apple.
- O projeto [ai-jail](https://github.com/akitaonrails/ai-jail) do Akita evoluiu de um script bash de 170 linhas para uma ferramenta Rust que configura isso automaticamente.

**Camada 2 — Git como safety net:**

Repositórios git permitem recuperação mesmo se um agente destruir arquivos. Antes de qualquer sessão de agente em projeto importante, garanta que há um commit limpo.

**Camada 3 — Review antes de confirmar:**

Antes de aprovar qualquer comando proposto pelo agente, revise especialmente:
- Comandos com `rm`, `rmdir` ou similares
- `git push`, `git reset --hard`, `git force`
- Chamadas de rede para endpoints externos
- Modificações em arquivos de configuração do sistema

**Camada 4 — Princípio do menor privilégio:**

Nunca dê ao agente acesso irrestrito ao filesystem inteiro. Restrinja ao diretório do projeto. O agente não precisa ver seus `.env`, SSH keys ou arquivos de configuração pessoal.

A regra geral: **use IA para seu home server sem problema. Para servidor de produção real, review humano em cada passo é inegociável.**

---

## Como a IA se encaixa nesse roadmap

| Fase | Use IA para | Não use IA para |
|------|-------------|-----------------|
| Fase 0 | Explicar conceitos de hardware, SO, compilação | Substituir a leitura e experimentação |
| Fase 1 (C) | Explicar segfaults, revisar sua implementação, gerar casos de teste | Escrever o código pelos você |
| Fase 2 (Sistemas) | Depurar race conditions, entender syscalls, man pages | Implementar processos/threads sem entender o modelo de concorrência |
| Fase 3 (C++) | Navegar a STL, entender templates, ver exemplos de RAII | Copiar padrões sem saber por quê |
| Fase 4 (Java) | Gerar boilerplate, verificar design patterns, sugerir testes | Substituir o entendimento de OOP e JVM |
| Fase 5 (Teoria) | Visualizar algoritmos, verificar provas, explorar complexidade | Resolver exercícios de otimização por você |
| Fase 6 (Eng.) | Code review, sugerir testes de integração, identificar code smells | Tomar decisões de arquitetura e segurança |

---

## Knowledge Check

1. Por que Akita responde "NENHUMA!" quando perguntado qual é o melhor LLM para Vibe Coding?
2. O que acontece com o mercado de juniors quando empresas passam a usar IA para tarefas de entrada?
3. O que é o mito do "one-shot prompt" e como ele se manifesta no aprendizado?
4. Por que um LLM que gera código com erro pode ser mais perigoso do que um que simplesmente não gera nada?
5. Em que tipo de tarefa a IA é melhor? Em que tipo ela é sistematicamente ruim?
6. Qual é a diferença entre usar IA para copiar código e usar IA para aprender?
7. O que é sandboxing de agentes de IA e por que é necessário mesmo com agentes "confiáveis"?
8. O que o projeto Frank Yomik (fracasso com OpenCV) ensina sobre o risco de otimizar a abordagem errada?
9. Por que testes unitários com mocks dão falsa confiança e qual é a alternativa?
10. Como o arquivo CLAUDE.md funciona como ferramenta de trabalho com agentes, além de documentação?

---

## Projeto

### Experimento de Calibração de IA

**Objetivo:** Desenvolver senso crítico sobre saídas de IA através de experimentação deliberada.

**Parte 1 — O teste da confiança:**

Pegue um exercício do módulo `fase1-c/01-fundamentos` que você **já resolveu**. Peça para um LLM resolvê-lo. Compare:
- A solução é diferente da sua? Em que aspectos?
- Funciona para todos os casos de teste que você testou?
- A IA cometeu algum erro que você consegue identificar por ter resolvido antes?

**Parte 2 — O teste do limite:**

Invente um caso de teste que quebre a solução da IA. Se não conseguir, pergunte: *"Quais inputs podem fazer essa solução falhar?"*. Implemente a correção você mesmo.

**Parte 3 — O teste da abordagem:**

Pegue um problema de ordenação do módulo `fase1-c/04-algoritmos-ordenacao`. Antes de implementar, pergunte à IA: *"Existe alguma biblioteca ou algoritmo estabelecido para esse problema específico? Quando faria sentido usar em vez de implementar do zero?"*. Use a resposta para entender o espaço de soluções antes de escrever código.

**Parte 4 — O diário:**

Crie um arquivo `ia-journal.md` no seu repositório. Durante a Fase 1 inteira, registre:
- Quando você usou IA
- Para quê usou
- Se a resposta estava correta
- O que aprendeu (mesmo quando a IA errou)

**Critério de sucesso:** Ao final da Fase 1, você consegue identificar quando a IA está errada sem precisar testar o código — e sabe quando a abordagem toda precisa mudar.

---

## Recursos Adicionais

### Para ler/assistir agora (em ordem)

- [RANT: IA acabou com os programadores?](https://akitaonrails.com/2026/02/08/rant-ia-acabou-com-programadores) — o contexto de mercado
- [RANT: o Akita abriu as pernas pra IA??](https://akitaonrails.com/2026/02/24/rant-o-akita-abriu-as-pernas-pra-ia) — esclarece a posição real do Akita sobre IA
- [Vibe Code: Qual LLM é a MELHOR?? Vamos falar a REAL](https://akitaonrails.com/2026/01/29/vibe-code-qual-llm-%C3%A9-a-melhor-vamos-falar-a-real) — a resposta honesta
- [37 dias de Imersão em Vibe Coding: Conclusão](https://akitaonrails.com/2026/03/05/37-dias-de-imers%C3%A3o-em-vibe-coding-conclus%C3%A3o-quanto-a-modelos-de-neg%C3%B3cio) — o balanço real após 37 dias
- [Software Nunca Está 'Pronto'](https://akitaonrails.com/2026/03/01/software-nunca-esta-pronto-4-projetos-a-vida-pos-deploy-e-por-que-one-shot-prompt-e-mito) — por que one-shot prompt é mito

### Para consulta

- [ai-jail: Sandbox para Agentes de IA](https://akitaonrails.com/2026/03/01/ai-jail-sandbox-para-agentes-de-ia-de-shell-script-a-ferramenta-real) — como sandboxing funciona na prática
- [AI Agents: Garantindo a Proteção do seu Sistema](https://akitaonrails.com/2026/01/10/ai-agents-garantindo-a-protecao-do-seu-sistema) — Bubblewrap e sandbox-exec
- [Testando LLMs Open Source e Comerciais](https://akitaonrails.com/2026/04/05/testando-llms-open-source-e-comerciais-quem-consegue-bater-o-claude-opus) — benchmark real de modelos
- [AI Agents: Comparando as principais LLM no Desafio de Zig](https://akitaonrails.com/2026/01/11/ai-agents-comparando-as-principais-llm-no-desafio-de-zig) — avaliação com problema simples e curto
- [AI Agents: Qual é o melhor agente?](https://akitaonrails.com/2026/01/24/ai-agents-qual-e-o-melhor-opencode-crush-claude-code-gpt-codex-gopilot-cursor-windsurf-antigravity) — comparativo de ferramentas

### Para ir além (projetos reais do Akita para estudar)

- [Do Zero à Pós-Produção em 1 Semana](https://akitaonrails.com/2026/02/20/do-zero-a-pos-producao-em-1-semana-como-usar-ia-em-projetos-de-verdade-bastidores-do-the-m-akita-chronicles) — workflow completo com IA
- [Meu primeiro fracasso com Vibe Code](https://akitaonrails.com/2026/03/05/meu-primeiro-fracasso-com-vibe-code-e-como-consertei-frank-yomik) — OpenCV vs modelos pré-treinados, sunk cost
- [Eu Fiz um Sistema de Data Mining — Dicas e Truques](https://akitaonrails.com/2026/03/04/eu-fiz-um-sistema-de-data-mining-pra-minha-namorada-influencer-dicas-e-truques) — prompts estruturados em YAML, idempotência, tool calling
- [Vibe Code: Fiz um Indexador de Imagens em 2 dias](https://akitaonrails.com/2026/02/23/vibe-code-fiz-um-indexador-inteligente-de-imagens-com-ia-em-2-dias) — benchmark-driven development, local-first com IA
- [Portando 10 Mil Linhas de Python pra Crystal com Claude](https://akitaonrails.com/2026/03/07/portando-10-mil-linhas-de-python-pra-crystal-com-claude-easy-subtitle) — migração de codebase com IA
- [Reescrevi o OpenClaw em Rust](https://akitaonrails.com/2026/03/16/reescrevi-o-openclaw-em-rust-funcionou-frankclaw) — scaffolding vs. segurança: onde a IA termina
- [Testes de Integração em MonoRepo](https://akitaonrails.com/2026/02/20/testes-de-integracao-em-monorepo-bastidores-do-the-m-akita-chronicles) — 7 camadas de teste, mocks vs. dados reais
- [Frontend Sem Framework](https://akitaonrails.com/2026/02/19/frontend-sem-framework-bastidores-do-the-m-akita-chronicles) — questionar defaults que a IA sugere
- [O código fonte do Claude Code vazou](https://akitaonrails.com/2026/03/31/codigo-fonte-do-claude-code-vazou-o-que-achamos-dentro) — o que a arquitetura interna de um agente revela
- [Migrando meu Home Server com Claude Code](https://akitaonrails.com/2026/03/31/migrando-meu-home-server-com-claude-code) — IA em infra: possível com cuidado, arriscado sem review
