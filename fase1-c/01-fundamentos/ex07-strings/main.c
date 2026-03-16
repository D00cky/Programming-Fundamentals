#include <stdio.h>
#include <string.h>

/*
** ex07-strings
**
** Objetivos:
**   - Entender que strings são arrays de char terminados em '\0'
**   - Implementar funções de string do zero (como na libft)
**   - Comparar com as funções de string.h
**
** Compile:
**   gcc -Wall -Wextra -Werror main.c -o strings
*/

/* --- Implementações do zero (estilo libft) ------------------------------- */

/* Comprimento da string (sem contar '\0') */
int	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

/* Copia src para dest, retorna dest */
char	*ft_strcpy(char *dest, const char *src)
{
	int	i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

/* Compara s1 e s2: 0 se iguais, diferença do primeiro char diferente */
int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

/* Converte char minúsculo para maiúsculo */
char	ft_toupper(char c)
{
	if (c >= 'a' && c <= 'z')
		return (c - 32);
	return (c);
}

/* Converte string para maiúsculas in-place */
void	str_toupper(char *s)
{
	while (*s)
	{
		*s = ft_toupper(*s);
		s++;
	}
}

/* Inverte string in-place */
void	str_reverse(char *s)
{
	int	len;
	int	i;
	char	temp;

	len = ft_strlen(s);
	i = 0;
	while (i < len / 2)
	{
		temp = s[i];
		s[i] = s[len - 1 - i];
		s[len - 1 - i] = temp;
		i++;
	}
}

int	main(void)
{
	char s1[] = "Hello, World!";
	char s2[50];

	printf("=== strlen ===\n");
	printf("ft_strlen(\"%s\") = %d\n", s1, ft_strlen(s1));
	printf("strlen(\"%s\")    = %zu\n", s1, strlen(s1));

	printf("\n=== strcpy ===\n");
	ft_strcpy(s2, s1);
	printf("ft_strcpy → \"%s\"\n", s2);

	printf("\n=== strcmp ===\n");
	printf("ft_strcmp(\"abc\", \"abc\") = %d (esperado: 0)\n",
		ft_strcmp("abc", "abc"));
	printf("ft_strcmp(\"abc\", \"abd\") = %d (esperado: <0)\n",
		ft_strcmp("abc", "abd"));
	printf("ft_strcmp(\"abd\", \"abc\") = %d (esperado: >0)\n",
		ft_strcmp("abd", "abc"));
	printf("ERRO COMUM: if (strcmp(s1,s2)) → verdadeiro quando DIFERENTES!\n");

	printf("\n=== Iteração sobre chars ===\n");
	char palavra[] = "programacao";
	printf("Letras de \"%s\":\n", palavra);
	for (int i = 0; palavra[i]; i++)       /* '\0' é 0 = falso, para o loop */
		printf("  [%d] '%c' = %d\n", i, palavra[i], palavra[i]);

	printf("\n=== toupper ===\n");
	char teste[] = "hello world";
	str_toupper(teste);
	printf("str_toupper → \"%s\"\n", teste);

	printf("\n=== reverse ===\n");
	char rev[] = "abcdef";
	printf("antes:  \"%s\"\n", rev);
	str_reverse(rev);
	printf("depois: \"%s\"\n", rev);

	/* TODO ---------------------------------------------------------------- */
	/*
	** 1. Implemente ft_strcat(char *dest, const char *src):
	**    concatena src ao final de dest.
	**
	** 2. Implemente ft_strchr(const char *s, int c):
	**    retorna ponteiro para primeira ocorrência de c em s, ou NULL.
	**
	** 3. Implemente int ft_ispalindrome(const char *s):
	**    retorna 1 se a string é palíndromo.
	**    Ex: "racecar", "madam", "level" → 1
	**
	** 4. Implemente int ft_atoi(const char *s):
	**    converte string para inteiro (ignora espaços iniciais, trata sinal).
	**    Ex: "  -42abc" → -42
	**
	** 5. Implemente void ft_itoa_simples(int n, char *buf):
	**    converte inteiro para string.
	**    Ex: 42 → "42", -7 → "-7"
	*/

	return (0);
}
