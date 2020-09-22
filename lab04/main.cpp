#include <math.h>
#include <gsl/gsl_eigen.h>

#define n 200
#define L 10.0
#define N 1.0
#define delta (L / (n+1))

double p(double x, double alfa)
{
    return 1 + 4 * alfa * x * x;
}

bool del(int i, int j) 
{
	if(i==j)
        return true;
    else
        return false;
}

int main() 
{
	gsl_eigen_gensymmv_workspace* gsl_workspace = gsl_eigen_gensymmv_alloc(n);
	FILE* edge_results = fopen("evec.dat", "w");
	FILE* all_results = fopen("eval.dat", "w");

	gsl_matrix* A = gsl_matrix_calloc(n, n);
	gsl_matrix* B = gsl_matrix_calloc(n, n);
	gsl_matrix* evec = gsl_matrix_calloc(n, n);
	gsl_vector* eval = gsl_vector_calloc(n);

	for (int alfa = 0; alfa <= 100; alfa+=2)
    {
		for (int i = 0; i < n; i++)
        {
			double x = -L/2.0 + delta*(i+1);
			for (int j = 0; j < n; j++) 
            {
                //Wypelnienie macierzy A i B wg wzorow
				gsl_matrix_set(A, i, j, (-1*del(i, j+1) + 2*del(i, j) - del(i, j-1)) / (delta*delta));
				gsl_matrix_set(B, i, j, p(x, alfa)/N * del(i, j));
			}
		}

        //rozwiazanie uogolnionego problemu wlasnego dla macierzy A i B
		gsl_eigen_gensymmv(A, B, eval, evec, gsl_workspace);
        //sortowanie w porzadku rosnacym
		gsl_eigen_gensymmv_sort(eval, evec, GSL_EIGEN_SORT_ABS_ASC);

		//Zapis wynikow dla kazdej alfy do pliku wg schematu: alfa l0 l1 l2 l3 l4 l5 
		fprintf(all_results, "%3d ", alfa);
		for (int i = 0; i < 6; i++)    {fprintf(all_results, "%lf ", sqrt(gsl_vector_get(eval, i)));}
        fprintf(all_results, "\n");

        bool space = true;

		//Zapis wynikow dla skrajnych alf do pliku
		if (alfa == 0 || alfa == 100)
        {
			for (int i = 0; i < n; i++)
            {
				double x = delta*(i+1) - L/2.0;
				fprintf(edge_results, "%lf ", x);

				for (int j = 0; j < 6; j++)    {fprintf(edge_results, "%lf ", gsl_matrix_get(evec, i, j));}
				fprintf(edge_results, "\n");
			}
            //Dwie linie przerwy w polowie pliku
			if (space)
            {
				fprintf(edge_results, "\n");
				fprintf(edge_results, "\n");
                space = false;
            }
		}
	}
    fclose(edge_results);
	fclose(all_results);
	gsl_eigen_gensymmv_free(gsl_workspace);
	gsl_matrix_free(A);
	gsl_matrix_free(B);
	gsl_vector_free(eval);
	gsl_matrix_free(evec);
}