
#include <stdio.h>
#include <math.h>
#include "nrutil.h"
#include "nrutil.c"

#define IT_MAX 12
#define N 7

float v_x_v(float* v, float* w)
{
	float temp = 0;
	for (int i = 0; i < N; i++)
    {
		temp += v[i]*w[i];
	}
	return temp;
}

float* m_x_v(float** m, float* v) 
{	
    float* temp = vector(0, N-1);

	for (int i = 0; i < N; i++)
    {
		temp[i] = 0;

		for (int j = 0; j < N; j++) {
			temp[i] += m[i][j] * v[j];
		}
	}
	return temp;
}

void orto(float* v, float* u) 
{
    float* temp = vector(0, N-1);
	for (int i = 0; i < N; i++) 
    {
		temp[i] = v[i] + u[i] * -1 * v_x_v(v, u);
	}
	for (int i = 0; i < N; i++)
    {
		v[i] = temp[i];
	}
	free_vector(temp, 0, N-1);
}

float** m_x_m(float** matrix1, float** matrix2) 
{
	float** temp = matrix(0, N-1, 0, N-1);
	
    for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			double sum = 0;
			for (int k = 0; k < N; k++) {
				sum += matrix1[i][k] * matrix2[k][j];
			}
			temp[i][j] = sum;
		}
	}
	return temp;
}

int main() 
{
	float** A = matrix(0, N-1, 0, N-1);
	float** X = matrix(0, N-1, 0, N-1);
	float** XTransposed = matrix(0, N-1, 0, N-1);
	float* xprev = vector(0, N-1);
	float* xcurr = vector(0, N-1);

	FILE* file = fopen("lambda.dat", "w");
	FILE* file2 = fopen("matrix.dat", "w");

    // wypelnienie symetrycznej macierzy A
	for (int i = 0; i < N; i++) 
    {
    	for (int j = 0; j < N; j++) 
        {
			A[i][j] = 1/sqrt(2+abs(i-j));
		}
	}

    // Wyznaczenie wartości własnych iteracyjnie, przy użyciu metody potęgowej
	for (int k = 0; k < N; k++)
    {
        //Wypelnienie wektora jedynkami - incjalizacja wektora startowego
		for (int i = 0; i < N; i++) 
        {
		    xprev[i] = 1;
	    }

		for (int i = 1; i <= IT_MAX; i++)
        {
			free_vector(xcurr, 0, N-1);
            //xprev(i+1) = A X xprev(i)
			xcurr = m_x_v(A, xprev);

            // Ortogonalizacja G-S
			for (int j = 0; j < k; j++) 
            {
				orto(xcurr, XTransposed[j]);
			}

            //Nie potrzebna jest transpozycja
            //v_x_v nie wymaga tego
			float lambda = ( v_x_v(xcurr, xprev) ) / ( v_x_v(xprev, xprev) );
			fprintf(file, "%d %f\n", i, lambda);
			
            //Wypelnienie wektora xprev(i)
            // xcurr / ||xcurr||2
            for (int i = 0; i < N; i++) 
            {
				xprev[i] = xcurr[i]/sqrt(v_x_v(xcurr, xcurr));
			}
		}
		fprintf(file, "\n\n");
 
        //zapis wektorow do macierzy transponowanej
		for (int i = 0; i < N; i++) 
        {
			XTransposed[k][i] = xprev[i];
		}
	}

    // zapis wektorow do 'zwyklej' macierzy
	for (int i = 0; i < N; i++)
    {
		for (int j = 0; j < N; j++)
        {
			X[i][j] = XTransposed[j][i];
		}
	}

    //Obliczenie koncowej macierzy
	XTransposed = m_x_m(XTransposed, A);
	XTransposed = m_x_m(XTransposed, X);

    //zapis macierzy do pliku
	for (int i = 0; i < N; i++)
    {
		for (int j = 0; j < N; j++) 
        {
			fprintf(file2, "%g ", XTransposed[i][j]);
		}
		fprintf(file2 ,"\n");
	}

	fclose(file);
    free_vector(xprev, 0, N-1);
	free_vector(xcurr, 0, N-1);
	free_matrix(A, 0, N-1, 0, N-1);
	free_matrix(X, 0, N-1, 0, N-1);
	free_matrix(XTransposed, 0, N-1, 0, N-1);
	return 0;
}