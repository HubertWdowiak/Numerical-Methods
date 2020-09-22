#include <stdio.h>
#include <iostream>

#define N 6
#define krok 0.1 //przy mniejszym kroku wykres nie jest tak gladki

//analizowana funkcja
double f(double x)
{
    return 1.0 / ( 1 + x*x );
}

void iloraz_roznicowy(double* fm, int n, double* ym, double* xm)
{
    //dla kazdego i nalezacego do {0, 1, 2 , ... n }
    for ( int i = 0; i < n; i++ )
    {
        //suma od 0 do i
        for ( int j = 0; j <= i; j++ )
        {
            //iloczyn od 0 do i
            double wsp = 1;
            for ( int l = 0; l <= i; l++ )
            {
                if (j != l) { wsp *= 1.0 / ( xm[j]-xm[l] ); }
            }
            fm[i] += wsp * ym[j];
            std::cout << fm[i] << " " ;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

double interpolacja(double x, int n, double* xm, double* fm)
{
    double inter_wynik = 0;

    // suma (od 0 do n)
    for ( int j = 0; j <= n; j++ )
    {
        // iloczyn (od 0 do j-1)
        double wsp = 1;
        for ( int i = 0; i < j; i++ )  { wsp *= x-xm[i]; }
        inter_wynik += wsp * fm[j];
    }
    return inter_wynik;
}

int main(int argc, char const *argv[])
{
    //uprzednio podana tablica wezlow
    double xm[] = {-5.0, -2.0, -0.5, 0.0, 0.5, 2.0, 5.0};
    //tablica rownomiernie rozlozonych wezlow
    double xm_rownomierne[N+1];

    //tablice wartosci funkcji dla wezlow 
    double ym[N+1];
    double ym_rownomierne[N+1];

    //tablice wartosci iloczynow roznicowych
    double fm_rownomierne[N+1];
    double fm[N+1];

    //poczatkowe wartosci
    for ( int i = 0; i < N+1; i++ )
    {
        //wartosci startowe dla iloczynow roznicowych = 0
        fm_rownomierne[i] = 0.0;
        fm[i] = 0.0;

        //rownomiernie rozlozone wezly
        xm_rownomierne[i] = -5.0 + i * (10.0/(N));
        //wartosci funkcji f dla tych wezlow
        ym_rownomierne[i] = f(xm_rownomierne[i]);

        //wartosci dla podanych uprzednio wezlow
        ym[i] = f(xm[i]);

    }


    FILE* file= fopen("zad_1.dat", "w");
    FILE* file_rownomierne = fopen("zad_2.dat", "w");

    iloraz_roznicowy( fm_rownomierne, N+1, ym_rownomierne, xm_rownomierne );
    iloraz_roznicowy( fm, N+1, ym, xm );

    for ( double x = -5.0; x <= 5.0; x += krok )
    {
        fprintf( file, "%.1f %f\n", x, interpolacja(x, N, xm, fm) );
        fprintf( file_rownomierne, "%.1f %f\n", x, interpolacja(x, N, xm_rownomierne, fm_rownomierne) );
    }
    
    fclose( file_rownomierne );
    fclose( file );
}
