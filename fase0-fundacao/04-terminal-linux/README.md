# 04 — Terminal Linux

> O terminal é sua ferramenta principal. Quanto mais fluente você for nele, mais produtivo você será em tudo.

## Navegação

```bash
pwd                  # onde estou? (print working directory)
ls                   # lista arquivos no diretório atual
ls -la               # lista com detalhes e arquivos ocultos
cd /home/d00cky      # vai para diretório absoluto
cd ~/Projects        # ~ = seu home directory
cd ..                # sobe um nível
cd -                 # volta ao diretório anterior
```

## Arquivos e Diretórios

```bash
# Criar
mkdir pasta              # cria diretório
mkdir -p a/b/c           # cria hierarquia de diretórios
touch arquivo.c          # cria arquivo vazio (ou atualiza timestamp)

# Copiar, mover, remover
cp arquivo.c copia.c     # copia arquivo
cp -r pasta/ destino/    # copia diretório recursivamente
mv arquivo.c novo.c      # move/renomeia
rm arquivo.c             # remove arquivo
rm -rf pasta/            # remove diretório e tudo dentro (CUIDADO!)

# Ler
cat arquivo.c            # imprime conteúdo inteiro
less arquivo.c           # scroll interativo (q para sair)
head -20 arquivo.c       # primeiras 20 linhas
tail -20 arquivo.c       # últimas 20 linhas
tail -f log.txt          # segue o arquivo em tempo real
```

## Busca

```bash
# Encontrar arquivos
find . -name "*.c"                    # arquivos .c no diretório atual e subdiretórios
find . -name "*.c" -not -path "*/.git/*"  # excluindo .git

# Buscar conteúdo dentro de arquivos
grep "malloc" *.c                    # linhas com "malloc" em arquivos .c
grep -r "main" .                     # busca recursiva
grep -n "TODO" arquivo.c             # mostra número da linha
grep -i "erro" log.txt               # case-insensitive

# Busca rápida de comandos anteriores
history | grep gcc                   # comandos anteriores com "gcc"
Ctrl+R                               # busca interativa no histórico
```

## Pipes e Redirecionamento

```bash
# Redirecionamento de output
./programa > saida.txt       # stdout para arquivo (sobrescreve)
./programa >> saida.txt      # stdout para arquivo (acrescenta)
./programa 2> erros.txt      # stderr para arquivo
./programa > saida.txt 2>&1  # stdout e stderr para o mesmo arquivo

# Pipes: output de um vira input do próximo
ls -la | grep ".c"           # lista arquivos .c
cat arquivo.c | wc -l        # conta linhas
ps aux | grep bash | grep -v grep
ls | sort | head -5          # 5 primeiros em ordem alfabética
```

## Processos

```bash
# Executar em background
./programa &             # roda em background
jobs                     # lista processos em background
fg %1                    # traz processo 1 para foreground
Ctrl+C                   # mata processo em foreground
Ctrl+Z                   # suspende processo em foreground

# Informações de processo
ps aux                   # todos os processos
ps aux | grep meu_prog   # encontrar processo específico
kill 1234                # mata processo com PID 1234
kill -9 1234             # força matar (SIGKILL)
killall nome             # mata todos com esse nome
```

## Variáveis e Ambiente

```bash
# Variáveis de ambiente
echo $HOME               # seu diretório home
echo $PATH               # onde o shell busca executáveis
echo $USER               # seu usuário

# Variáveis locais
NOME="mundo"
echo "Hello $NOME"

# Exportar para subprocessos
export MINHA_VAR="valor"
```

## Permissões (essencial para programas C)

```bash
chmod +x programa        # torna executável
chmod 755 programa       # rwxr-xr-x
chmod 644 arquivo.c      # rw-r--r--

# Rodar como root (para operações do sistema)
sudo comando             # executa como root
```

## Atalhos Essenciais do Terminal

| Atalho | Ação |
|--------|------|
| `Ctrl+C` | Cancela/mata processo atual |
| `Ctrl+Z` | Suspende processo atual |
| `Ctrl+D` | End of file / sair do shell |
| `Ctrl+L` | Limpa a tela |
| `Ctrl+R` | Busca no histórico |
| `Tab` | Autocomplete |
| `↑ / ↓` | Navega no histórico |
| `Ctrl+A` | Vai ao início da linha |
| `Ctrl+E` | Vai ao fim da linha |
| `Ctrl+U` | Apaga do cursor ao início |

## Compilando C no Terminal

```bash
# Compilar um arquivo
gcc hello.c -o hello

# Compilar com flags (será o padrão nos seus projetos)
gcc -Wall -Wextra -Werror hello.c -o hello

# Executar
./hello

# Compilar com debug info (para usar com GDB)
gcc -g hello.c -o hello

# Compilar múltiplos arquivos
gcc -Wall main.c utils.c -o programa
```

## Fluxo de trabalho típico

```bash
# Ciclo que você vai repetir constantemente:
vim hello.c         # ou nano, ou qualquer editor
gcc -Wall -Wextra -Werror hello.c -o hello
./hello
# -> viu erro? volta pro editor
# -> funcionou? commita no git
```

---

## Exercícios Práticos

1. Crie a estrutura: `mkdir -p ~/test/{src,bin,include}` e explore com `ls -R ~/test`
2. Escreva um arquivo com `echo "hello world" > ~/test/src/hello.txt`, leia com `cat`
3. Use `find ~/Projects -name "*.md"` para listar todos os markdowns do projeto
4. Redirecione o output de `ls -la` para um arquivo e depois leia ele
5. Use `grep -r "Fase" ~/Projects/Programming-Fundamentals/roadmap.md`

---

## Referências

- **The Linux Command Line** — William Shotts (gratuito: linuxcommand.org)
- **tldr** — versão resumida dos man pages: `tldr grep`, `tldr find` (instalar: `sudo pacman -S tldr`)
- **explainshell.com** — cola qualquer comando e explica cada parte
- `man <comando>` — documentação completa de qualquer comando
