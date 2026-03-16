#include <stdio.h>

/*
** ex05-funcoes
**
** Objetivos:
**   - Escrever e organizar funções
**   - Entender passagem por valor
**   - Implementar recursão corretamente
**
** Compile:
**   gcc -Wall -Wextra -Werror main.c -o funcoes
*/

/* --- Declarações (prototypes) -------------------------------------------- */
int		fatorial(int n);
int		fib_recursivo(int n);
int		fib_iterativo(int n);
int		potencia(int base, int exp);
int		mdc(int a, int b);          /* Algoritmo de Euclides */
void	troca(int a, int b);        /* Não funciona! Por quê? */

/* --- Implementações ------------------------------------------------------- */

int	fatorial(int n)
{
	if (n <= 1)
		return (1);
	return (n * fatorial(n - 1));
}

/* Recursão ingênua: O(2^n) — lento para n grande */
int	fib_recursivo(int n)
{
	if (n <= 1)
		return (n);
	return (fib_recursivo(n - 1) + fib_recursivo(n - 2));
}

/* Iterativo: O(n) — sempre prefira isso */
int	fib_iterativo(int n)
{
	int	a;
	int	b;
	int	temp;

	if (n <= 1)
		return (n);
	a = 0;
	b = 1;
	while (n-- > 1)
	{
		temp = a + b;
		a = b;
		b = temp;
	}
	return (b);
}

int	potencia(int base, int exp)
{
	if (exp == 0)
		return (1);
	return (base * potencia(base, exp - 1));
}

/* MDC pelo algoritmo de Euclides: mdc(a,b) = mdc(b, a%b) */
int	mdc(int a, int b)
{
	if (b == 0)
		return (a);
	return (mdc(b, a % b));
}

/* Esta função NÃO modifica a e b no chamador — passagem por valor! */
void	troca(int a, int b)
{
	int	temp;

	temp = a;
	a = b;
	b = temp;
	printf("  dentro de troca: a=%d, b=%d\n", a, b);
}

int	main(void)
{
	/* Fatorial */
	printf("=== Fatorial ===\n");
	for (int i = 0; i <= 10; i++)
		printf("%2d! = %d\n", i, fatorial(i));

	/* Fibonacci comparando recursivo vs iterativo */
	printf("\n=== Fibonacci ===\n");
	for (int i = 0; i <= 10; i++)
	{
		int r = fib_recursivo(i);
		int it = fib_iterativo(i);
		printf("fib(%2d) = %3d  [recursivo=%d iterativo=%d match=%s]\n",
			i, r, r, it, r == it ? "SIM" : "NÃO");
	}

	/* Potência */
	printf("\n=== Potência ===\n");
	printf("2^10 = %d\n", potencia(2, 10));
	printf("3^5  = %d\n", potencia(3, 5));

	/* MDC */
	printf("\n=== MDC (Euclides) ===\n");
	printf("mdc(48, 18) = %d  (esperado: 6)\n", mdc(48, 18));
	printf("mdc(100, 75) = %d (esperado: 25)\n", mdc(100, 75));

	/* Passagem por valor */
	printf("\n=== Passagem por VALOR ===\n");
	int x = 10, y = 20;
	printf("antes: x=%d, y=%d\n", x, y);
	troca(x, y);
	printf("depois: x=%d, y=%d  ← NÃO mudou!\n", x, y);
	printf("Para trocar de verdade, precisamos de ponteiros (módulo 02)\n");

	/* TODO ---------------------------------------------------------------- */
	/*
	** 1. Implemente int abs_val(int n) que retorna o valor absoluto de n
	**    sem usar <stdlib.h>.
	**
	** 2. Implemente int conta_digitos(int n) que conta quantos dígitos
	**    tem um inteiro. Ex: conta_digitos(12345) = 5.
	**    Implemente a versão recursiva E a iterativa.
	**
	** 3. Implemente int soma_digitos(int n). Ex: soma_digitos(123) = 6.
	**
	** 4. Implemente int inverte(int n). Ex: inverte(1234) = 4321.
	**
	** 5. Implemente int eh_palindromo(int n) que retorna 1 se o número
	**    é palíndromo. Ex: 121, 1331, 12321.
	*/

	return (0);
}
