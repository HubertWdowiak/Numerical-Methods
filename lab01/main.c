#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Dyrektywy zakladajace, ze te trzy pliki sa skopiowane do aktualnego katalogu. */
#include "nrutil.h"
#include "nrutil.c" // To mozna usunac, jesli plik jest dodany w poleceniu kompilacji.
#include "gaussj.c" // To tez mozna usunac, jesli plik jest dodany w poleceniu kompilacji.

/* Dyrektywy dla Taurusa (nie wymagaja kopiowania plikow, ale Taurus musi dzialac...) */
// #include "/opt/NR/numerical_recipes.c/nrutil.h"
// #include "/opt/NR/numerical_recipes.c/nrutil.c"
// #include "/opt/NR/numerical_recipes.c/gaussj.c"

#define N 400	 // rozmiar macierzy M: NxN
#define STEP 0.1

int main(void)
{
	float **M, **b;
	//	Alokacja macierzy
	M = matrix(1, N, 1, N);
	b = matrix(1, N, 1, 1);

	// 	Wypelnienie macierzy M i wektora b
	for (int i = 1; i <= N; ++i)
	{
		b[i][1] = 0.0;
		for (int j = 1; j <= N; ++j)
			M[i][j] = 0.0;
	}
	b[1][1] = 1.0;

	M[1][1] = 1.0;
	M[2][1] = -1.0;
	M[2][2] = 1.0;

	for (int i = 3; i <= 400; i++) {
		M[i][i-2] = 1.0;
		M[i][i-1] = STEP*STEP - 2;
		M[i][i] = 1.0;
	}

	//	Rozwiazanie ukladu rownan Mx=b - wywolanie procedury:
	gaussj(M, N, b, 1);

	//	Wypisanie rozwiazania, ktore procedura gaussj(M, N, b, 1); zapisala w wektorze b.
	for (int i = 1; i <= N; ++i)
		printf("%f %g\n", (i-1)*STEP, b[i][1]);

	//	Zwolnienie pamieci
	free_matrix(M, 1, N, 1, N);
	free_matrix(b, 1, N, 1, 1);

	return 0;
}
