#include <math.h>
#include <stdio.h>
#include <vector>
double f1(double x) 
{
	return sin(x)/x;
}

double f2(double x)
{
	return (cos(x)-exp(x)) / sin(x);
}

double f3(double x)
{
	return exp(-1/x)/x;
}

double handle(double x, double (*func)(double))
{
    if(fabs(x)<1e-8)
		return func(x + 1e-8);
    else 
        return func(x);
}

double ** createMatrix(int n){
    double** matrix=new double*[n]; 
	for(int i=0; i<n; i++)
		matrix[i]=new double[n];
    return matrix;
}

void freeMatrix(double** matrix, int n) {
	for(int i=0; i<n; i++)
		delete matrix[i];
	delete matrix;
}

void Calculate(double ** matrix, int n, double a, double b, double (*func)(double), FILE* file) {

	matrix[0][0] = 0.5*(b-a) * (handle(a, func) + handle(b, func));

	for(int i=1; i<=n; i++)
    {
        double temp = 0;
		double hw = (b-a) / pow(2, i);
		for(int j=1; j<=pow(2, i-1); j++) 
			temp += handle(a + (2 * j-1)*hw, func);
		matrix[i][0]=0.5 * matrix[i-1][0] + hw * temp;
	}

	for(int i=1; i<=n; i++) {
		for(int j=i; j<=n; j++)
			matrix[j][i]=(pow(4, i) * matrix[j][i-1] - matrix[j-1][i-1]) / (pow(4, i) - 1);
	}
	for(int i=0; i<=n; i++) 
		fprintf(file, "%d %.10f   %.10f\n", i, matrix[i][0], matrix[i][i]);
	fclose(file);

}

int main() {

    std::vector<double(*)(double)> func_vector = {f1, f2, f3};
    std::vector<int> size_vector = {8, 16, 8};
    std::vector<double> a_vector = {0, -1, 0};
    std::vector<double> b_vector = {1, 1, 1};
    std::vector<FILE*> file_vector = {fopen("file1.dat", "w"), fopen("file2.dat", "w"), fopen("file3.dat", "w")};

    for( int i=0; i<file_vector.size(); i++ )
    {
        double** matrix = createMatrix(size_vector[i]);
        Calculate(matrix, size_vector[i]-1, a_vector[i], b_vector[i], func_vector[i], file_vector[i]);
        freeMatrix(matrix, size_vector[i]);
    }
	return 0;
}

