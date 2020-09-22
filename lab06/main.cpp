#include "math.h"
#include <iostream>
#define N 5
#define IT_MAX 30

double licz_r(double* a, double* b, int n, double xj)
{
	b[n] = 0;
	for(int k = n-1; k >= 0; k--)
		b[k] = a[k + 1] + xj*b[k + 1];
	double R = a[0] + xj*b[0];
	return R;
}

int main()
{
	double a[N + 1];
	double b[N + 1];
	double x0, x1, x2, R0, R1, R2;
	int n;
	FILE * file = fopen("zero.dat", "w");
	fprintf(file, "| %s  %13s  %13s  %13s |\n", "L", "it", "x2", "R2");

	//wspolczynniki wielomianu
	a[0] = 240;
	a[1] = -196;
	a[2] = -92;
	a[3] = 33;
	a[4] = 14;
	a[5] = 1;

	for(int L=1; L<=N; L++)
	{
		//ustalamy aktualny stopien wielomanu:
		n = N-L+1;
		//inicjalizacja wzoru iteracyjnego:
		x0 = 0.0;
		x1 = 0.1;
		R0 = licz_r(a, b, n, x0);
		R1 = licz_r(a, b, n, x1);

		for(int it = 1;  it <= IT_MAX; it++)
		{
			x2 = x1 - R1*(x1 - x0)/(R1 - R0);
			//wyznaczamy R2
			R2 = licz_r(a, b, n, x2);
			//zachowujemy dane do kolejnej iteracji
			R0 = R1;
			R1 = R2;
			x0 = x1;
			x1 = x2;
			//zapis do pliku
			fprintf(file, "| %d  %13d  %13g  %13g |\n", L, it, x2, R2);
			//warunek zakonczenia
			if(fabs(x1 - x0) < 1.0E-7)
				break;
		}
		for(int i=0; i<=(n-1); i++)
			a[i] = b[i];
	}
	fclose(file);
	return 0;
}