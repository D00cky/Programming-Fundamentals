# minishell

> Fase 2 — Sistemas

Mini shell POSIX: parsing, execução, pipes, redirecionamentos, builtins e sinais.

## Critérios de conclusão

- [ ] Executa comandos via `execve` com `PATH` correto
- [ ] Suporta pipes multi-stage (`cmd1 | cmd2 | cmd3`)
- [ ] Redirecionamentos: `<`, `>`, `>>`, `<<` (here_doc)
- [ ] Builtins: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`
- [ ] Sinais: `Ctrl+C` (novo prompt), `Ctrl+D` (sai), `Ctrl+\` (ignorado)

## Recursos

- [Especificação 42 School](https://projects.intra.42.fr/projects/minishell)
- [Writing a Shell in C — Stephen Brennan](https://brennan.io/2015/01/16/write-a-shell-in-c/)
- [Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html)
