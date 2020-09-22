#include <stdio.h>
#include <math.h>
#include <utility>
#include <iostream>
#define DX  1.e-4
#define DY  1.e-4
#define ITER_MAX 1000

double f(double x, double y)
{
    return 5.0/2.0*(x*x - y)*(x*x - y) + (1.0 - x)*(1.0 - x);
}
double dfdx(double rx, double ry)
{  
    return ( f(rx + DX, ry) - f(rx - DX, ry) )/( 2.0*DX);
}
double dfdy(double rx, double ry)
{
    return ( f(rx, ry + DY) - f(rx, ry-DY) )/(2.0*DY);
}

std::pair<double, double> find_min_loc(double x, double y, FILE* file, double epsilon)
{
    double H = 0.1;
    int iter_count;
    for (int it = 1; it <= ITER_MAX; ++it)
    {
       // if(it > 50)
       //     H = 0.99*H;
        double x1 = x - H * dfdx(x, y);
        double y1 = y - H * dfdy(x, y);

        //zapis do pliku
        fprintf(file, "%g %g\n", x1, y1);
        
        //Sprawdzenie warunku zatrzymania petli
        double diff = sqrt((x1 - x)*(x1 -x) + (y1 - y)*(y1 - y));
        if(diff < epsilon) 
            break;

        x = x1;
        y = y1;
        iter_count=it;
    }
    std::cout <<"iterations: " << iter_count << std::endl;
    std::cout <<"results: " << x << " " << y << std::endl;
    return std::pair<double, double>(x, y);
}

int main()
{
    FILE* file_1 = fopen("eps1.dat", "w");
    FILE* file_2 = fopen("eps2.dat", "w");

    std::pair<double, double> result1 = find_min_loc(-0.75, 1.75, file_1, 1.e-2);
    std::pair<double, double> result2 = find_min_loc(-0.75, 1.75, file_2, 1.e-3);

    fclose(file_1);
    fclose(file_2);
}
