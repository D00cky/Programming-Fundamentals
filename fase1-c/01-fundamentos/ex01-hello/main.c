#include <stdio.h>

/*
** ex01-hello
**
** Objetivos:
**   - Compilar e rodar o primeiro programa em C
**   - Entender o que `return 0` significa
**   - Ver o exit code com `echo $?`
**
** Compile:
**   gcc -Wall -Wextra -Werror main.c -o hello
**
** Rode:
**   ./hello
**   echo $?          # deve imprimir 0
**
** Experimentos:
**   1. Mude o return para 42 e veja o que `echo $?` mostra
**   2. Adicione mais printf's com \n, \t, \\, \"
**   3. O que acontece se remover o #include <stdio.h>?
*/

int	main(void)
{
	printf("Hello, World!\n");
	return (0);
}
