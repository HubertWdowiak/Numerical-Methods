#define _USE_MATH_DEFINES
#include <math.h>
#include "nrutil.h"
#include "nrutil.c"
#include "four1.c"
#include <ctime>

float fun(float val, float period)
{
    double temp = 2 * M_PI / period;
    return sin(val*temp) + sin(val*temp*2) + sin(val*temp*3); 
}
float g(float val, float sigma)
{ 
    return (1.0 / (sigma * sqrt(2*M_PI))) * exp(-1*(val*val) / (2*sigma*sigma)); 
}

int main()
{
    //otwarcie plikow
    std::srand(std::time(nullptr));
    FILE* file1 = fopen("file1.dat", "w");
    FILE* file2 = fopen("file2.dat", "w");
    FILE* file3 = fopen("file3.dat", "w");
    FILE* files[] = {file1, file2, file3};
    int f_num = 0;

    for (int k = 8; k <= 12; k += 2)
    {
        //inicjalizacja wartosci poczatkowych i wektorow
        int n = pow(2, k);
        float *f, *g1, *g2;
        float T = 1;
        float tmax = 3 * T;
        float dt = tmax/n;
        float sigma = T/20.0;
        f = vector(1, 2*n);
        g1 = vector(1, 2*n);
        g2 = vector(1, 2*n);

        //wypelnienie sygnalu szumem
        for (int i = 1; i <= n; i++)
        {
            //wypenienie rzeczywistych wartosci sygnalu
            f[2*i-1] = fun(dt*( i-1 ), 1) + (float)rand()/RAND_MAX - 0.5;
            //wypelnienie urojonych wartosci sygnalu
            f[2*i] = 0;
            g1[2*i] = 0;
            g2[2*i] = 0;
            //wypisanie rzeczywistych wartosci tablicy f
            fprintf(files[f_num], "%g %g\n", dt*(i-1), f[2*i-1]);
 
            //wypelnienie nieparzystych elementow wartosciami wg wzoru g(dt*(i-1), sigma)
            g1[2*i-1] = g(dt*(i-1), sigma);
            g2[2*i-1] = g(dt*(i-1), sigma);

        }
        fprintf(files[f_num], "\n\n");

        //wykonanie szybkiej tranformaty fouriera dla kazdej z tablic ( dla f, g1 normalnie, dla f2 odwrotnie )
        four1(f, n, 1);
        four1(g1, n, 1);
        four1(g2, n, -1);


        //obliczenie tranformaty splotu
        for (int i = 1; i <= n; i++)
        {
            float f_nieparz = f[2*i-1];
            float g_sum_nieparz = g1[2*i-1] + g2[2*i-1];
            
            float f_parz = f[2*i];
            float g_sum_parz = g1[2*i] + g2[2*i];
            
            f[2*i-1] = f_nieparz*g_sum_nieparz - f_parz*g_sum_parz;
            f[2*i] = f_nieparz*g_sum_parz + g_sum_nieparz*f_parz;
        }

        //obliczenie odwrotnej transformaty, ktorej wynikiem jest splot f(t) * g(t)
        four1(f, n, -1);

        //wyszukiwanie elementu rzeczywistego o najwiekszym module
        float max_abs = 0;
        for (int i = 1; i <= n; i++)
        {
            if (fabs(f[2*i-1]) > max_abs)
                max_abs = fabs(f[2*i-1]);
        }

        //wypisanie rzeczywistych elementow tablicy, znormalizowanych, poprzedzonych czasem
        for (int i = 1; i <= n; i++)
        {
            fprintf(files[f_num], "%g %g\n", dt*(i-1), f[2*i-1]/( max_abs/2.5 ) );
        }
        f_num++;
        free_vector(g2, 1, 2 * n);
        free_vector(g1, 1, 2 * n);
        free_vector(f, 1, 2 * n);
    }

    fclose(file1);
    fclose(file2);
    fclose(file3);

    return 0;
}
