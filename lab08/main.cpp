#include <stdio.h>
#include <stdlib.h>
#include "nrutil.h"
#include "nrutil.c"
#include "gaussj.c"
#include <iostream>
#include <vector>

#define XMIN -5.0
#define XMAX 5.0
#define DX 0.01

float f1( float x ) 
{
    return 1.0f/( 1 + x*x );
}

float f2( float x )
{
    return cos( 2*x );
}

float phi( float x, int i, float* xw, float h )
{
    if ( x>=xw[i-2] && x<xw[i-1] )
        return pow( x-xw[i-2], 3 ) / pow( h, 3 );
    else if ( x>=xw[i-1] && x<xw[i] )
        return ( pow( h, 3 ) + 3*h*h*( x-xw[i-1] ) + 3*h*pow( x-xw[i-1], 2 ) - 3*pow( x-xw[i-1], 3 ) ) / pow( h, 3 );
    else if ( x >= xw[i] && x < xw[i+1] )
        return ( pow( h, 3 ) + 3*h*h*( xw[i+1]-x ) + 3*h*pow( xw[i+1]-x, 2 ) - 3*pow( xw[i+1]-x, 3 ) ) / pow( h, 3 );
    else if ( x >= xw[i+1] && x <= xw[i+2] )
        return pow( xw[i+2]-x, 3 ) / pow( h, 3 );
    else 
        return 0;
}

float s( float x, float* xw, float* c, int n, float h )
{
    float temp = 0.0;
    for ( int i=0; i<=n+1; i++ )
    { temp += c[i] * phi( x, i, xw, h ); }
    return temp;
}

int main()
{
    FILE* file1 = fopen( "f1.dat", "w" );
    FILE* file2 = fopen( "f2.dat", "w" );

    std::vector< FILE* > files = { file1, file2 };
    std::vector< std::vector<int> > sizes =  {{5,6,10,20}, {6,7,14}};
    //tablica funkcji
    float (*fun_tab[2])(float) = {f1, f2};


    for (int fun = 0; fun<sizeof( fun_tab )/sizeof( *fun_tab ); fun++)
    {
        
        for ( int i = 0; i < sizes[fun].size(); i++ )
        {
            //liczba węzłów interpolacji w przedziale [XMIN, XMAX]
            int n = sizes[fun][i];
            //alpha = df(x) / DX    |   x=xmin
            float alpha = ( fun_tab[fun]( XMIN+DX ) - fun_tab[fun]( XMIN-DX ) ) / ( 2*DX );
            //beta = df(x) / DX    |   x=xmax
            float beta = ( fun_tab[fun]( XMAX+DX ) - fun_tab[fun]( XMAX-DX ) ) / ( 2*DX );
            //wektor współczynnikow
            float* c = vector( 0, n+1 );
            //Odległość między węzłami
            float h = ( XMAX-XMIN ) / ( n-1 );
            //Wektor położeń równoodległych węzłów
            float* xw = vector( -2, n+3 );
            //Wektor wartości funkcji w węzłach
            float* yw = vector( 1, n );
            //macierz A
            float** A = matrix( 1, n, 1, n );
            //Wektor wyrazów wolnych
            float** b = matrix( 1, n, 1, 1 );

            //Wypelnienie xw
            for ( int i = -2; i <= n+3; i++ ) 
                xw[i] = XMIN + ( i-1 )*h;
            
            //Uzupelnienie wartosci wg wzoru(4)
            for ( int i = 1; i <= n; i++ )
            {
                //Poczatkowe zerowanie macierzy A
                for ( int j = 1; j <= n; j++ )
                {
                    A[i][j] = 0.0f;
                }
                A[i][i] = 4;
                b[i][1] = fun_tab[fun]( xw[i] );
                yw[i] = fun_tab[fun]( xw[i] );
            }
            

            //Wypelnienie b na krańcach w zależnosci od parametrów
            b[1][1] += h/3.0f * alpha;
            b[n][1] -= h/3.0f * beta;


            //Wypelnienie przekątnych obok głównej przekatnej w macierzy A
            for ( int i = 1; i < n; i++ )
            {
                A[i+1][i] = 1.0f;
                A[i][i+1] = 1.0f;
            }
            A[1][2] = A[n][n-1] = 2.0f;


            //Obliczanie współczynników c
            gaussj( A, n, b, 1 );

            std::cout << "Wspolczynniki C " << n << ":" << "\n";
            for ( int i = 1; i <= n; i++ )
            {
                c[i] = b[i][1];
                std::cout << c[i] << " ";
            }   
            c[0] = c[2] - h/3.0f * alpha;
            c[n+1] = c[n-1] + h/3.0f * beta;
            std::cout << c[n+1];
            std::cout << c[0] << " ";

            std::cout << "\n";            
            
            //Zapis do pliku
            for ( double d = XMIN; d <= XMAX; d+=DX )
                fprintf( files[fun], "%g %g\n", d, s( d, xw, c, n, h ));
            fprintf( files[fun], "\n\n" );
            
            free_matrix( A, 1, n, 1, n );
            free_matrix( b, 1, n, 1, 1 );
            free_vector( c, 0, n+1 );
            free_vector( xw, -2, n+3 );
            free_vector( yw, 1, n );
        }
    }

    fclose( file1 );
    fclose( file2 );

    return 0;
}
