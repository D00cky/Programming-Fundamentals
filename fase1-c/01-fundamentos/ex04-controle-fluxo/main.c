#include <stdio.h>

/*
** ex04-controle-fluxo
**
** Objetivos:
**   - Praticar if/else, for, while, switch
**   - Implementar problemas clássicos de lógica
**
** Compile:
**   gcc -Wall -Wextra -Werror main.c -o controle
*/

/* FizzBuzz: para i de 1 a n:
**   se divisível por 3 → "Fizz"
**   se divisível por 5 → "Buzz"
**   se divisível por ambos → "FizzBuzz"
**   senão → o número
*/
void	fizzbuzz(int n)
{
	int	i;

	printf("=== FizzBuzz até %d ===\n", n);
	i = 1;
	while (i <= n)
	{
		if (i % 15 == 0)
			printf("FizzBuzz");
		else if (i % 3 == 0)
			printf("Fizz");
		else if (i % 5 == 0)
			printf("Buzz");
		else
			printf("%d", i);
		printf(i < n ? " " : "\n");
		i++;
	}
}

/* Sequência de Fibonacci até o n-ésimo termo */
void	fibonacci(int n)
{
	int	a;
	int	b;
	int	temp;
	int	i;

	printf("=== Fibonacci (%d termos) ===\n", n);
	a = 0;
	b = 1;
	i = 0;
	while (i < n)
	{
		printf("%d", a);
		if (i < n - 1)
			printf(", ");
		temp = a + b;
		a = b;
		b = temp;
		i++;
	}
	printf("\n");
}

/* Verifica se n é primo */
int	is_prime(int n)
{
	int	i;

	if (n < 2)
		return (0);
	i = 2;
	while (i * i <= n)
	{
		if (n % i == 0)
			return (0);
		i++;
	}
	return (1);
}

int	main(void)
{
	fizzbuzz(20);

	printf("\n");
	fibonacci(10);

	/* Primos até 50 */
	printf("\n=== Primos até 50 ===\n");
	for (int i = 2; i <= 50; i++)
		if (is_prime(i))
			printf("%d ", i);
	printf("\n");

	/* TODO ---------------------------------------------------------------- */
	/*
	** 1. Imprima um triângulo de asteriscos de altura n:
	**    n=4:
	**    *
	**    **
	**    ***
	**    ****
	**
	** 2. Imprima a tabuada de multiplicação de 1 a 10 (tabela 10x10).
	**
	** 3. Dado um número inteiro positivo n, imprima todos os seus divisores.
	**    Exemplo: divisores(12) → 1 2 3 4 6 12
	**
	** 4. Usando switch: leia um número de 1 a 7 e imprima o dia da semana.
	**    (use uma variável int dia = 3; e teste com switch)
	*/

	return (0);
}
