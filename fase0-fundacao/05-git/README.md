# 05 — Git

> Controle de versão não é opcional. É hábito desde o primeiro `hello.c`.

## O que é Git?

Git é um sistema de controle de versão distribuído. Ele rastreia mudanças no seu código ao longo do tempo, permitindo:
- Voltar para qualquer versão anterior
- Trabalhar em paralelo sem medo de perder código
- Colaborar com outras pessoas
- Entender **o que** mudou, **quando** e **por quê**

---

## Conceitos Fundamentais

### Os 3 estados de um arquivo no Git

```
Working Directory  →  Staging Area  →  Repository (.git)
   (suas edições)     (git add)         (git commit)
```

- **Working Directory**: onde você edita os arquivos normalmente
- **Staging Area (index)**: o que vai entrar no próximo commit
- **Repository**: histórico permanente de commits

### O que é um commit?

Um commit é um snapshot do projeto em um momento. Cada commit tem:
- Um hash SHA-1 único (ex: `a3f5c7d`)
- Mensagem descritiva
- Autor e timestamp
- Ponteiro para o commit anterior

---

## Comandos Essenciais

### Configuração inicial (uma vez só)

```bash
git config --global user.name "Seu Nome"
git config --global user.email "seu@email.com"
git config --global core.editor "vim"  # ou nano, code, etc.
```

### Criar e inicializar

```bash
git init                    # cria repositório no diretório atual
git clone <url>             # copia repositório remoto
```

### Status e histórico

```bash
git status                  # o que mudou? o que está staged?
git log                     # histórico de commits
git log --oneline           # histórico resumido (uma linha por commit)
git log --oneline --graph   # histórico com branches visual
git diff                    # o que mudou (não staged)
git diff --staged           # o que vai entrar no próximo commit
```

### Adicionar e commitar

```bash
git add arquivo.c           # adiciona arquivo específico ao staging
git add .                   # adiciona tudo (use com cuidado)
git add -p                  # adiciona interativamente (escolhe pedaços)
git commit -m "mensagem"    # cria commit
git commit                  # abre editor para mensagem
```

### Branches

```bash
git branch                  # lista branches
git branch nova-feature     # cria branch
git checkout nova-feature   # muda para branch
git checkout -b nova-feature # cria e muda de uma vez (atalho)
git merge nova-feature      # une branch atual com nova-feature
git branch -d nova-feature  # deleta branch (após merge)
```

### Remoto (GitHub/GitLab)

```bash
git remote -v               # mostra remotes configurados
git remote add origin <url> # adiciona remote
git push origin main        # envia para remote
git pull                    # baixa e integra mudanças do remote
git fetch                   # baixa mudanças sem integrar
```

### Desfazer coisas

```bash
git restore arquivo.c       # descarta mudanças no working dir
git restore --staged arquivo.c  # remove do staging (não descarta edição)
git revert <hash>           # desfaz um commit criando um novo commit
git stash                   # salva mudanças temporariamente
git stash pop               # recupera mudanças salvas
```

---

## .gitignore

Arquivo que diz ao Git o que **não** rastrear:

```bash
# .gitignore típico para projetos C
*.o          # object files
*.a          # static libraries
a.out        # executável padrão do gcc
meu_programa # executável específico

# IDEs
.vscode/
*.swp        # vim swap files

# Sistema
.DS_Store    # macOS
```

---

## Boas mensagens de commit

Uma boa mensagem de commit explica **o quê** e **por quê**, não o **como**.

```bash
# RUIM
git commit -m "fix"
git commit -m "mudanças"
git commit -m "atualizar arquivo"

# BOM
git commit -m "Add boundary check in ft_strlen to handle NULL input"
git commit -m "Fix memory leak in linked list when removing last node"
git commit -m "Implement bubble sort in O(n^2) as baseline comparison"
```

**Formato convencional** (muito usado em projetos reais):
```
<tipo>: <descrição curta>

<corpo opcional explicando o porquê>
```
Tipos: `feat`, `fix`, `refactor`, `docs`, `test`, `chore`

---

## Fluxo de trabalho deste projeto

```bash
# 1. Antes de começar a trabalhar
git pull                          # garante que está atualizado

# 2. Durante o trabalho
# ... edita arquivo.c ...
git status                        # vê o que mudou
git diff                          # revisa as mudanças
git add arquivo.c
git commit -m "feat: implement ft_strlen"

# 3. Ao terminar uma sessão ou tópico
git push origin main              # sobe para o repositório remoto

# 4. Para um novo exercício ou projeto
git checkout -b fase1/libft       # branch dedicada
# ... trabalha ...
git merge main                    # integra de volta
```

---

## Exercícios

1. Configure seu git com nome e email
2. Faça um commit de um arquivo `.c` simples com uma mensagem descritiva
3. Use `git log --oneline` para ver o histórico
4. Crie uma branch, faça uma mudança, e faça merge de volta para main
5. Adicione um `.gitignore` no diretório `fase0-fundacao` que ignore `*.o` e executáveis

---

## Referências

- **Pro Git** — Scott Chacon (gratuito: git-scm.com/book) — **a referência definitiva**
- **Oh Shit, Git!?!** — ohshitgit.com — como resolver problemas comuns
- **Learn Git Branching** — learngitbranching.js.org (interativo visual)
- `man git-<comando>` — ex: `man git-commit`
