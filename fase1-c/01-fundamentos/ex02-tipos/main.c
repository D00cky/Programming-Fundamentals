#include <stdio.h>
#include <limits.h>   // INT_MAX, INT_MIN, CHAR_MAX, etc.
#include <float.h>    // FLT_MAX, DBL_MAX, etc.

/*
** ex02-tipos
**
** Objetivos:
**   - Ver o tamanho real de cada tipo na sua máquina
**   - Entender overflow de inteiros
**   - Entender truncamento em divisão inteira
**   - Praticar casting
**
** Compile:
**   gcc -Wall -Wextra -Werror main.c -o tipos
*/

int	main(void)
{
	/* --- sizeof ---------------------------------------------------------- */
	printf("=== sizeof ===\n");
	printf("char:        %zu byte(s)\n", sizeof(char));
	printf("short:       %zu byte(s)\n", sizeof(short));
	printf("int:         %zu byte(s)\n", sizeof(int));
	printf("long:        %zu byte(s)\n", sizeof(long));
	printf("long long:   %zu byte(s)\n", sizeof(long long));
	printf("float:       %zu byte(s)\n", sizeof(float));
	printf("double:      %zu byte(s)\n", sizeof(double));

	/* --- Limites --------------------------------------------------------- */
	printf("\n=== Limites de inteiros ===\n");
	printf("INT_MAX  = %d\n", INT_MAX);
	printf("INT_MIN  = %d\n", INT_MIN);
	printf("UINT_MAX = %u\n", UINT_MAX);
	printf("CHAR_MAX = %d\n", CHAR_MAX);

	/* --- Overflow -------------------------------------------------------- */
	printf("\n=== Overflow (undefined behavior — observe o que acontece) ===\n");
	int max = INT_MAX;
	printf("INT_MAX     = %d\n", max);
	printf("INT_MAX + 1 = %d\n", max + 1);  // overflow!

	/* --- Divisão inteira ------------------------------------------------- */
	printf("\n=== Divisão inteira vs float ===\n");
	int a = 7, b = 2;
	printf("%d / %d = %d   (inteira, trunca)\n", a, b, a / b);
	printf("%d / %d = %.1f (float, precisa de cast)\n", a, b, (double)a / b);

	/* --- Char como número ------------------------------------------------ */
	printf("\n=== char é um número ===\n");
	char c = 'A';
	printf("'A' como char:   %c\n", c);
	printf("'A' como int:    %d\n", c);
	printf("'A' + 1 = '%c' = %d\n", c + 1, c + 1);

	/* --- TODO: Implemente abaixo ---------------------------------------- */
	/*
	** 1. Declare uma variável `unsigned int` com o valor UINT_MAX.
	**    Some 1 a ela e imprima. O que acontece?
	**
	** 2. Declare um `double` com o valor de PI (3.14159265358979).
	**    Imprima com %f, %.2f, %.10f, %e. Note a diferença.
	**
	** 3. Faça o casting de um double para int e observe o truncamento:
	**    double d = 3.99;
	**    int i = (int)d;  // qual valor?
	*/

	return (0);
}
