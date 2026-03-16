#include <stdio.h>

/*
** ex02-aritmetica
** Aritmética de ponteiro e relação ponteiro-array
**
** Compile: gcc -Wall -Wextra -Werror main.c -o aritmetica
*/

/* Tamanho de array via ponteiros */
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

/* Soma com ponteiro em vez de índice */
int	soma_array(int *inicio, int *fim)
{
	int	total;

	total = 0;
	while (inicio < fim)
		total += *inicio++;
	return (total);
}

/* Implementação de memcpy manual */
void	*ft_memcpy(void *dest, const void *src, int n)
{
	unsigned char		*d;
	const unsigned char	*s;

	d = dest;
	s = src;
	while (n--)
		*d++ = *s++;
	return (dest);
}

/* Implementação de memset manual */
void	*ft_memset(void *s, int c, int n)
{
	unsigned char	*p;

	p = s;
	while (n--)
		*p++ = (unsigned char)c;
	return (s);
}

int	main(void)
{
	int arr[] = {10, 20, 30, 40, 50};
	int n = ARRAY_SIZE(arr);

	printf("=== Array vs Ponteiro ===\n");
	int *p = arr;
	for (int i = 0; i < n; i++)
	{
		printf("arr[%d]=%d  *(arr+%d)=%d  *(p+%d)=%d  p[%d]=%d\n",
			i, arr[i], i, *(arr + i), i, *(p + i), i, p[i]);
	}

	printf("\n=== Aritmética ===\n");
	printf("p   aponta para arr[0]: %d\n", *p);
	p++;
	printf("p++ aponta para arr[1]: %d\n", *p);
	p += 2;
	printf("p+=2 aponta para arr[3]: %d\n", *p);
	printf("p - arr = %ld (distância em elementos)\n", p - arr);

	printf("\n=== Ponteiro como iterador ===\n");
	int total = soma_array(arr, arr + n);
	printf("soma = %d\n", total);

	printf("\n=== memcpy e memset ===\n");
	int dst[5];
	ft_memcpy(dst, arr, sizeof(arr));
	printf("dst após memcpy: ");
	for (int i = 0; i < n; i++) printf("%d ", dst[i]);
	printf("\n");

	ft_memset(dst, 0, sizeof(dst));
	printf("dst após memset(0): ");
	for (int i = 0; i < n; i++) printf("%d ", dst[i]);
	printf("\n");

	/*
	** TODO:
	** 1. Implemente int *ft_memchr(void *s, int c, int n):
	**    retorna ponteiro para primeira ocorrência de c, ou NULL.
	**
	** 2. Implemente void inverte_com_ponteiros(int *arr, int n):
	**    use dois ponteiros (inicio e fim) caminhando para o centro.
	**
	** 3. Dado int arr[10] com valores aleatórios, ache a posição do
	**    maior elemento usando aritmética de ponteiros (sem índices).
	*/

	return (0);
}
