#include <stdio.h>

/*
** ex03-operadores
**
** Objetivos:
**   - Praticar operadores aritméticos, relacionais, lógicos e bitwise
**   - Entender precedência de operadores
**   - Ver operações bitwise na prática
**
** Compile:
**   gcc -Wall -Wextra -Werror main.c -o operadores
*/

/* Imprime um número em binário (sem stdlib) */
void	print_binary(unsigned int n)
{
	int	i;

	i = 31;
	while (i >= 0)
	{
		printf("%d", (n >> i) & 1);
		if (i % 4 == 0 && i > 0)
			printf(" ");
		i--;
	}
	printf("\n");
}

int	main(void)
{
	/* --- Aritméticos ----------------------------------------------------- */
	printf("=== Aritméticos ===\n");
	printf("17 / 5  = %d\n", 17 / 5);
	printf("17 %% 5 = %d\n", 17 % 5);
	printf("-7 / 2  = %d\n", -7 / 2);   // trunca em direção a zero em C99+
	printf("-7 %% 2 = %d\n", -7 % 2);

	/* --- Bitwise --------------------------------------------------------- */
	printf("\n=== Bitwise ===\n");
	unsigned int a = 0b00001100;  // 12
	unsigned int b = 0b00001010;  // 10

	printf("a       = "); print_binary(a); printf("          = %u\n", a);
	printf("b       = "); print_binary(b); printf("          = %u\n", b);
	printf("a & b   = "); print_binary(a & b); printf("          = %u  (AND)\n", a & b);
	printf("a | b   = "); print_binary(a | b); printf("          = %u  (OR)\n", a | b);
	printf("a ^ b   = "); print_binary(a ^ b); printf("          = %u  (XOR)\n", a ^ b);
	printf("~a      = "); print_binary(~a); printf(" = %u (NOT)\n", ~a);
	printf("a << 2  = "); print_binary(a << 2); printf("          = %u  (×4)\n", a << 2);
	printf("a >> 1  = "); print_binary(a >> 1); printf("          = %u  (÷2)\n", a >> 1);

	/* --- Truques com bitwise --------------------------------------------- */
	printf("\n=== Truques com bitwise ===\n");
	int n = 13;
	printf("%d é %s\n", n, (n & 1) ? "ímpar" : "par");
	printf("bit 2 de %d: %d\n", n, (n >> 2) & 1);

	/* --- Precedência ----------------------------------------------------- */
	printf("\n=== Precedência ===\n");
	printf("2 + 3 * 4   = %d  (esperado: 14)\n", 2 + 3 * 4);
	printf("(2 + 3) * 4 = %d  (esperado: 20)\n", (2 + 3) * 4);
	printf("1 + 2 > 2   = %d  (esperado: 1 — > tem prioridade menor que +)\n", 1 + 2 > 2);

	/* --- TODO ------------------------------------------------------------ */
	/*
	** 1. Usando apenas operações bitwise (sem *, /), calcule:
	**    - n * 8   (dica: shift left)
	**    - n / 4   (dica: shift right)
	**    - verifica se o bit 3 de n está setado
	**
	** 2. Escreva uma expressão que zera o bit 1 de n sem afetar os outros.
	**    (dica: AND com uma máscara)
	**
	** 3. Escreva uma expressão que seta o bit 4 de n sem afetar os outros.
	**    (dica: OR com uma máscara)
	*/

	return (0);
}
