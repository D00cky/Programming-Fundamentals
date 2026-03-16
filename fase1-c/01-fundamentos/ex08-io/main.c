#include <stdio.h>

/*
** ex08-io
**
** Objetivos:
**   - Praticar printf com diferentes especificadores
**   - Usar scanf e entender seus problemas
**   - Ler strings com espaço usando fgets
**   - Ler caracteres com getchar
**
** Compile:
**   gcc -Wall -Wextra -Werror main.c -o io
*/

/* Lê um inteiro com validação — repete enquanto input inválido */
int	leia_inteiro(const char *prompt, int min, int max)
{
	int	n;
	int	ok;

	do {
		printf("%s [%d-%d]: ", prompt, min, max);
		ok = scanf("%d", &n);
		while (getchar() != '\n')   /* limpa o buffer até o '\n' */
			;
		if (!ok)
		{
			printf("  Input inválido. Tente novamente.\n");
			continue ;
		}
		if (n < min || n > max)
			printf("  Fora do intervalo. Tente novamente.\n");
	} while (!ok || n < min || n > max);
	return (n);
}

int	main(void)
{
	/* --- printf: especificadores ----------------------------------------- */
	printf("=== printf: especificadores ===\n");
	printf("|%-15s|%5d|%8.2f|\n", "banana", 42, 3.14159);
	printf("|%-15s|%5d|%8.2f|\n", "abacaxi", 1000, 2.71828);
	printf("|%-15s|%5d|%8.2f|\n", "mango", -7, 0.0);

	printf("\n=== Bases numéricas ===\n");
	int n = 255;
	printf("decimal:     %d\n", n);
	printf("hexadecimal: %x  (ou %X maiúsculo)\n", n, n);
	printf("octal:       %o\n", n);
	/* printf("binário: não tem especificador nativo em C\n"); */

	/* --- fgets: lê linha inteira (incluindo espaços) --------------------- */
	printf("\n=== fgets (lê com espaços) ===\n");
	char nome[64];
	printf("Digite seu nome completo: ");
	fgets(nome, sizeof(nome), stdin);

	/* fgets mantém o '\n' no final — remover se precisar */
	int i = 0;
	while (nome[i] != '\0' && nome[i] != '\n')
		i++;
	if (nome[i] == '\n')
		nome[i] = '\0';

	printf("Olá, %s!\n", nome);

	/* --- scanf com validação --------------------------------------------- */
	printf("\n=== scanf com validação ===\n");
	int idade = leia_inteiro("Sua idade", 0, 150);
	printf("Idade registrada: %d\n", idade);

	/* --- getchar: caractere a caractere ----------------------------------- */
	printf("\n=== getchar: conta vogais ===\n");
	printf("Digite uma frase (Enter para terminar):\n> ");

	int c;
	int vogais = 0;
	int total = 0;
	while ((c = getchar()) != '\n' && c != EOF)
	{
		total++;
		if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
		    c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U')
			vogais++;
	}
	printf("Total de chars: %d | Vogais: %d | Proporção: %.1f%%\n",
		total, vogais, total > 0 ? (double)vogais / total * 100 : 0.0);

	/* TODO ---------------------------------------------------------------- */
	/*
	** 1. Peça ao usuário um número e imprima sua tabuada de multiplicação
	**    (de 1 a 10).
	**
	** 2. Leia números inteiros até o usuário digitar 0.
	**    Imprima: soma, quantidade, média, maior e menor.
	**
	** 3. Leia uma string e imprima:
	**    - O número de palavras (palavras separadas por espaço)
	**    - A string com todas as letras maiúsculas
	**    - A string invertida
	*/

	return (0);
}
