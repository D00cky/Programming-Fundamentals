#include <stdio.h>

/*
** ex06-arrays
**
** Objetivos:
**   - Manipular arrays
**   - Implementar busca linear e bubble sort
**   - Entender que arrays são passados como ponteiro para funções
**
** Compile:
**   gcc -Wall -Wextra -Werror main.c -o arrays
*/

#define TAM 10

/* Imprime um array */
void	print_array(int *arr, int n)
{
	printf("[");
	for (int i = 0; i < n; i++)
		printf("%d%s", arr[i], i < n - 1 ? ", " : "");
	printf("]\n");
}

/* Busca linear: retorna índice ou -1 se não encontrar */
int	busca_linear(int *arr, int n, int alvo)
{
	for (int i = 0; i < n; i++)
		if (arr[i] == alvo)
			return (i);
	return (-1);
}

/* Bubble sort: O(n²) — simples, mas lento */
void	bubble_sort(int *arr, int n)
{
	int	temp;
	int	trocou;

	for (int i = 0; i < n - 1; i++)
	{
		trocou = 0;
		for (int j = 0; j < n - 1 - i; j++)
		{
			if (arr[j] > arr[j + 1])
			{
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
				trocou = 1;
			}
		}
		if (!trocou)   /* array já ordenado — otimização */
			break ;
	}
}

/* Inverte o array in-place */
void	inverte_array(int *arr, int n)
{
	int	i;
	int	temp;

	i = 0;
	while (i < n / 2)
	{
		temp = arr[i];
		arr[i] = arr[n - 1 - i];
		arr[n - 1 - i] = temp;
		i++;
	}
}

int	main(void)
{
	int arr[TAM] = {42, 7, 19, 3, 55, 1, 28, 14, 99, 8};

	printf("Original: ");
	print_array(arr, TAM);

	/* Busca */
	printf("\n=== Busca Linear ===\n");
	int alvo = 28;
	int idx = busca_linear(arr, TAM, alvo);
	if (idx != -1)
		printf("arr[%d] = %d\n", idx, alvo);
	else
		printf("%d não encontrado\n", alvo);

	/* Ordenação */
	printf("\n=== Bubble Sort ===\n");
	bubble_sort(arr, TAM);
	printf("Ordenado: ");
	print_array(arr, TAM);

	/* Inversão */
	printf("\n=== Inversão ===\n");
	inverte_array(arr, TAM);
	printf("Invertido: ");
	print_array(arr, TAM);

	/* Matriz 2D */
	printf("\n=== Matriz de Multiplicação 3x3 ===\n");
	int matriz[3][3];
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			matriz[i][j] = (i + 1) * (j + 1);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			printf("%3d", matriz[i][j]);
		printf("\n");
	}

	/* TODO ---------------------------------------------------------------- */
	/*
	** 1. Encontre o maior e menor elemento do array (antes de ordenar).
	**    Faça em uma única passagem pelo array.
	**
	** 2. Calcule a média dos elementos do array.
	**
	** 3. Implemente int array_unico(int *arr, int n):
	**    retorna 1 se todos os elementos são únicos, 0 se há duplicatas.
	**
	** 4. Implemente void remove_duplicatas(int *arr, int *n):
	**    remove duplicatas do array in-place, atualizando n.
	**    Ex: [1,2,2,3,3,3] → [1,2,3], n=3
	**
	** 5. Implemente a busca binária (requer array ordenado):
	**    int busca_binaria(int *arr, int n, int alvo)
	**    Complexidade: O(log n) — muito mais rápida que linear para n grande.
	*/

	return (0);
}
