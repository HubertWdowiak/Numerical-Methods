#define _USE_MATH_DEFINES
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <vector>
#include <time.h>
#define X_MAX (2*M_PI)
#define X_MIN 0

//Funkcje aproksymowane podane w tresci
double f1( double x) { return 2*sin(x) + sin(2*x) + 2*sin(3*x); }
double f2( double x) { return 2*sin(x) + sin(2*x) + 2*cos(x) + cos(2*x); }
double f3( double x) { return 2*sin(1.1*x) + sin(2.1*x) + 2*sin(3.1*x); }
double alpha() { return rand()/ (RAND_MAX + 1.0) - 0.5; }
double f1_plus_alpha( double x) { return 2.0*sin(x) + sin(2.0*x) + 2.0*sin(3.0*x) + alpha(); }
//Funkcja aproksymujaca
double F( double x, double* a, double* b, int MS, int MC)
{
    double suma = 0;
    for( int i = 1; i <= MS; i++ ) { suma += a[i]*sin(i*x); }
    for( int i = 1; i <= MC; i++ ) { suma += b[i]*cos(i*x); }
    return suma + b[0]/2.0;
}
int main( int argc, char const *argv[])
{
    srand(time(0));

    //Otwarcie wszystkich plików potrzebnych do zapisu wynikow
    FILE* pkt = fopen("pkt.dat", "w");
    FILE* approx1 = fopen("approx1.dat", "w");
    FILE* approx2 = fopen("approx2.dat", "w");
    FILE* approx3 = fopen("approx3.dat", "w");
    FILE* approx4 = fopen("approx4.dat", "w");
    FILE* ab = fopen("ab4.dat", "w");

    std::vector<double (*) ( double)> funkcje = {f1, f2, f3, f1_plus_alpha};

    //utworzenie zmiennych, tablic

    //liczba wezlow
    const int n = 100;
    const int MS[] = {5, 10, 30};
    const int MC[] = {5, 10, 30};
    //odleglosc miedzy wezlami
    const double h = (X_MAX-X_MIN)/ (n-1);
    const double dx = 0.01;
    
    double* a = new double[MS[0] + 2];
    double* b = new double[MC[0] + 2];

    double* _a = new double[MS[1] + 2];
    double* _b = new double[MC[1] + 2];
    
    double* __a = new double[MS[2] + 2];
    double* __b = new double[MC[2] + 2];
    
    double* parametry_a[] = {a, _a, __a};
    double* parametry_b[] = {b, _b, __b};

    //tablica argumentow, rownomiernie od siebie oddalonych o odlegosc h
    double* x = new double[n];
    //tablice wartosci funkcji w punktach tablicy x
    double* y1 = new double[n];
    double* y2 = new double[n];
    double* y3 = new double[n];
    double* y4 = new double[n];
    
    //wypelnienie tablic wartosciami funkcji w wezlach
    for( int i = 0; i < n; i++ )
    {
        x[i] = i*h;
        y1[i] = f1(x[i]);
        y2[i] = f2(x[i]);
        y3[i] = f3(x[i]);
        y4[i] = f1_plus_alpha(x[i]);
    }

    //zapis do pliku pkt.dat
    for( int i = 0; i < funkcje.size()-1; i++ )
    {
        for( int j = 0; j < n; j++ ) { fprintf(pkt, "%f %f\n", x[j], funkcje[i](x[j])); }
        fprintf(pkt, "\n\n");
    }
    for( int i = 0; i < n; i++ ) { fprintf(pkt, "%f %f\n", x[i], y4[i]); }
    fclose(pkt);

    // zapis do pliku approx1.dat
    for( int i = 0; i <= MS[0] + 1; i++ )
    {
        double suma = 0;
        for( int j = 0; j < n; j++ )
            suma += y1[j]*sin( i*x[j] );
        a[i] = 2.0*suma/ n;
    }
    for( int i = 0; i <= MC[0] + 1; i++ )
    {
        double suma = 0;
        for( int j = 0; j < n; j++ ) 
            suma += y1[j]*cos( i*x[j] );
        b[i] = 2*suma/ n;
    }
    for( double d = X_MIN; d <= X_MAX; d+=dx )
        fprintf( approx1, "%f %f\n", d, F(d, a, b, MS[0], MC[0]) ); 
    fclose(approx1);

    // Zapis do pliku approx2.dat
    for( int i = 0; i <= MS[0] + 1; i++ )
    {
        double suma = 0;
        for( int j = 0; j < n; j++ )
            suma += y2[j]*sin(i*x[j]);
        a[i] = 2.0*suma / n;
    }
    for( int i = 0; i <= MC[0] + 1; i++ )
    {
        double suma = 0.0;
        for( int j = 0; j < n; j++ )
            suma += y2[j]*cos( i*x[j] );
        b[i] = 2.0*suma / n;
    }
    for( double d = X_MIN; d <= X_MAX; d+=dx)
        fprintf(approx2, "%f %f\n", d, F(d, a, b, MS[0], MC[0]));
    fclose(approx2);


   // Zapis do pliku approx3.dat
    for( int i = 0; i <= MS[0] + 1; i++ )
    {
        double suma = 0;
        for( int j = 0; j < n; j++ )
            suma += y3[j]*sin(i*x[j]);
        a[i] = 2.0*suma/ n;
    }
    double suma = 0.0;
    for( int j = 0; j < n; j++ )
        suma += y3[j];
    b[0] = 2.0*suma/ n;

    for( double d = X_MIN; d <= X_MAX; d+=dx)
        fprintf(approx3, "%f %f\n", d, F(d, a, b, MS[0], 0));
    fprintf(approx3, "\n\n");

   //Pozostale wartosci 
    for( int m = 0; m < 2; m++ )
    {
        for( int i = 0; i <= MS[m] + 1; i++ )
        {
            double suma = 0.0;
            for( int j = 0; j < n; j++ )
                suma += y3[j]*sin(i*x[j]);
            parametry_a[m][i] = 2.0*suma/ n;
        }
        for( int i = 0; i <= MC[m] + 1; i++ )
        {
            double suma = 0.0;
            for( int j = 0; j < n; j++ )
                suma += y3[j]*cos(i*x[j]);
            parametry_b[m][i] = 2.0*suma/ n;
        }
        for( double d = X_MIN; d <= X_MAX; d+=dx)
            fprintf(approx3, "%f %f\n", d, F(d, parametry_a[m], parametry_b[m], MS[m], MC[m]));
        fprintf(approx3, "\n\n");
    }
    fclose(approx3);

   // Zapis do pliku approx4.dat
    for( int m = 0; m < 3; m+=2)
    {
        for( int i = 0; i <= MS[m] + 1; i++ )
        {
            double suma = 0;
            for( int j = 0; j < n; j++ )
                suma += y4[j]*sin(i*x[j]);
            parametry_a[m][i] = 2.0*suma/ n;
        }
        for( int i = 0; i <= MC[m] + 1; i++ )
        {
            double suma = 0;
            for( int j = 0; j < n; j++ )
                suma += y4[j]*cos(i*x[j]);
            parametry_b[m][i] = 2.0*suma/ n;
        }
        for( double d = X_MIN; d <= X_MAX; d+=dx)
            fprintf(approx4, "%f %f\n", d, F(d, parametry_a[m], parametry_b[m], MS[m], MC[m]));
        fprintf(approx4, "\n\n");
    }
    fclose(approx4);

   // Zapis do ab.dat
    for( int m = 0; m < 3; m+=2)
    {
        for( int i = 0; i <= MS[m]; i++ )
            fprintf(ab, "%d, %f, %f\n", i, parametry_a[m][i], parametry_b[m][i]);
        fprintf(ab, "\n\n");
    }
    fclose(ab);

    delete x;
    delete y1;
    delete y2;
    delete y3;
    delete y4;
    delete a;
    delete b;
    delete _a;
    delete _b;
    delete __a;
    delete __b;
}
