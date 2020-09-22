#include <stdio.h>
#include <math.h>
#include <iostream>
#include "nrutil.h"
#include "nrutil.c"
#include <time.h>
#define N 1000
#define MVALUE 5


double* add( int n, double* v, double* w, double alfa ) {
	
	double* x = dvector(0, n-1);
	for ( int i = 0; i < n; i++ ) 
	{
		x[i] = v[i] + alfa*w[i];
	}
	return x;
}

double scalar( int n, double* v, double* w ) {
	
	double x = 0;
	for ( int i = 0; i < n; i++ ) 
	{
		x += v[i]*w[i];
	}
	return x;
}

double* mXv( int n, double** M, double* v ) {
	
	double* x = dvector(0, n-1);
	for ( int i = 0; i < n; i++ ) 
	{
		x[i] = 0;
		for ( int j = 0; j < n; j++ ) 
		{
			x[i] += M[i][j]*v[j];
		}
	}
	return x;
}

void gradientFunc(int len, double** A, double* x, double* b) {
	
	double* r = dvector(0, len-1);
	double* v = mXv(len, A, x);
	for ( int i = 0; i < len; i++ )
	{
		r[i] = v[i] = b[i] - v[i];
	}

	int counter = 0;
	double alpha, beta, prevRxR;
	alpha = beta = prevRxR = 0.0;

	//std::cout << counter << " "<< sqrt(scalar(len, r, r)) << " " << alpha << " " << beta<< " " << sqrt(scalar(len, x, x)) << std::endl;

	while (sqrt(scalar(len, r, r)) > 0.000001) {
		prevRxR = scalar(len, r, r);

		double* avprod = mXv(len, A, v);
		alpha = prevRxR / scalar( len, v, avprod);
		
		double* xPlusAlfaV = add( len, x, v, alpha);
		for (int i = 0; i < len; i++) {
			x[i] = xPlusAlfaV[i];		
		}

		free_dvector( xPlusAlfaV, 0, len-1 );
		xPlusAlfaV = add( len, r, avprod, -alpha );
		free_dvector(avprod, 0, len-1);
		free_dvector( r, 0, len-1 );
		r = xPlusAlfaV;
		beta = scalar( len, r, r )/prevRxR;
		xPlusAlfaV = add( len, r, v, beta );
		free_dvector( v, 0, len-1 );
		v = xPlusAlfaV;
		counter++;

		//std::cout << counter << " "<< sqrt(scalar(len, r, r)) << " " << alpha << " " << beta<< " " << sqrt(scalar(len, x, x)) << std::endl;

    }
	free_dvector( v, 0, len-1 );
	free_dvector( r, 0, len-1 );
}

int main() {

	double** m = dmatrix(0, N-1, 0, N-1);
	double* b = dvector(0, N-1);
	double* x = dvector(0, N-1);

time_t time0 = clock();
	for ( int i = 0; i < N; i++ ) 
	{
		for ( int j = 0; j < N; j++ ) 
		{
			if ( fabs(i-j) <= MVALUE )	m[i][j] = 1.0/(1+fabs(i-j));
			else	m[i][j] = 0;
		}
		x[i] = 0;
		b[i] = i+1;
	}

	gradientFunc(N, m, x, b);
time_t time2 = clock();
std::cout << time2-time0;
	free_dmatrix(m, 0, N-1, 0, N-1);
	free_dvector(x, 0, N-1);
	free_dvector(b, 0, N-1);
}
