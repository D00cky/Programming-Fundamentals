#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
** ex03-malloc-free
**
** Compile: gcc -Wall -Wextra -Werror -g main.c -o malloc_free
** Valgrind: valgrind --leak-check=full ./malloc_free
*/

/* Cria array dinâmico e preenche com quadrados */
int	*cria_quadrados(int n)
{
	int	*arr;
	int	i;

	arr = malloc(n * sizeof(int));
	if (!arr)
		return (NULL);
	i = 0;
	while (i < n)
	{
		arr[i] = (i + 1) * (i + 1);
		i++;
	}
	return (arr);
}

/* Duplica um array — caller é responsável por free */
int	*duplica_array(int *src, int n)
{
	int	*copia;

	copia = malloc(n * sizeof(int));
	if (!copia)
		return (NULL);
	memcpy(copia, src, n * sizeof(int));
	return (copia);
}

/* Exemplo de realloc: cresce array dinamicamente */
void	demo_realloc(void)
{
	int	*arr;
	int	cap;
	int	len;

	cap = 4;
	len = 0;
	arr = malloc(cap * sizeof(int));
	if (!arr)
		return ;

	printf("=== Crescimento dinâmico com realloc ===\n");
	for (int i = 0; i < 10; i++)
	{
		if (len == cap)
		{
			cap *= 2;
			int *novo = realloc(arr, cap * sizeof(int));
			if (!novo) { free(arr); return ; }
			arr = novo;
			printf("  realloc → capacidade agora %d\n", cap);
		}
		arr[len++] = i * i;
	}
	printf("Array: ");
	for (int i = 0; i < len; i++) printf("%d ", arr[i]);
	printf("\n");
	free(arr);
}

int	main(void)
{
	/* malloc básico */
	printf("=== malloc + free ===\n");
	int n = 8;
	int *quadrados = cria_quadrados(n);
	if (!quadrados)
	{
		fprintf(stderr, "malloc falhou\n");
		return (1);
	}
	printf("Quadrados: ");
	for (int i = 0; i < n; i++) printf("%d ", quadrados[i]);
	printf("\n");

	/* calloc: aloca e inicializa com zero */
	printf("\n=== calloc ===\n");
	int *zeros = calloc(5, sizeof(int));
	if (!zeros) { free(quadrados); return (1); }
	printf("calloc(5): ");
	for (int i = 0; i < 5; i++) printf("%d ", zeros[i]);
	printf("(todos zero)\n");

	/* duplicar */
	printf("\n=== duplica_array ===\n");
	int *copia = duplica_array(quadrados, n);
	if (!copia) { free(quadrados); free(zeros); return (1); }
	copia[0] = 999;
	printf("original[0]=%d  copia[0]=%d\n", quadrados[0], copia[0]);

	free(quadrados);
	free(zeros);
	free(copia);

	/* realloc */
	printf("\n");
	demo_realloc();

	/*
	** TODO:
	** 1. Implemente char *ft_strdup(const char *s):
	**    aloca e retorna cópia de s no heap.
	**    Caller deve fazer free().
	**
	** 2. Implemente char **ft_split(const char *s, char sep):
	**    divide s pelo separador sep, retorna array de strings.
	**    Ex: ft_split("hello world foo", ' ') → ["hello","world","foo",NULL]
	**    Não esqueça de free cada string e depois o array.
	**
	** 3. Crie intencionalmente um leak e confirme com Valgrind que ele detecta.
	**    Depois corrija e confirme "All heap blocks were freed".
	*/

	return (0);
}
