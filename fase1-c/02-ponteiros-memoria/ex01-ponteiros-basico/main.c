#include <stdio.h>

/*
** ex01-ponteiros-basico
**
** Compile: gcc -Wall -Wextra -Werror main.c -o ponteiros
*/

void	incrementa(int *n)
{
	(*n)++;
}

void	troca(int *a, int *b)
{
	int	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

int	main(void)
{
	int x = 10;
	int *p = &x;

	printf("=== Básico ===\n");
	printf("x    = %d\n", x);
	printf("&x   = %p\n", (void *)&x);
	printf("p    = %p\n", (void *)p);
	printf("*p   = %d\n", *p);
	printf("&p   = %p\n", (void *)&p);   /* endereço do próprio ponteiro */

	*p = 99;
	printf("\nDepois de *p = 99:\n");
	printf("x  = %d\n", x);
	printf("*p = %d\n", *p);

	printf("\n=== Passagem por referência ===\n");
	int n = 5;
	printf("antes de incrementa: %d\n", n);
	incrementa(&n);
	printf("depois de incrementa: %d\n", n);

	int a = 100, b = 200;
	printf("\nantes de troca: a=%d b=%d\n", a, b);
	troca(&a, &b);
	printf("depois de troca: a=%d b=%d\n", a, b);

	/*
	** TODO:
	** 1. Escreva void zera(int *p) que coloca 0 no valor apontado.
	**
	** 2. Escreva void min_max(int *arr, int n, int *min, int *max)
	**    que retorna o menor e maior elemento via ponteiros.
	**
	** 3. Escreva int soma_via_ptr(int *a, int *b) — soma valores apontados.
	**    Chame com: soma_via_ptr(&x, &y)
	*/

	return (0);
}
