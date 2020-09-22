#include "nrutil.h"
#include "nrutil.c"
#include "ludcmp.c"
#include "lubksb.c"

#define N 3

void printMatrix(float** m, int size, const char* msg) {
	printf("%s", msg);
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			printf("%g\t", m[i+1][j+1]);
		}
		printf("\n");
	}
}

int main(int argc, const char *argv[]) {
	float **mA, **mAor, **mBor, **mB, **mAr, **mBr, **mAproduct, **mBproduct, d, *xa, *xb, normA, normB, normAr, normBr, kA, kB;
	int* indx;

	mA = matrix(1, N, 1, N);
	mB = matrix(1, N, 1, N);
	mAor = matrix(1, N, 1, N);
	mBor = matrix(1, N, 1, N);
	mAr = matrix(1, N, 1, N);
	mBr = matrix(1, N, 1, N);
	mAproduct = matrix(1, N, 1, N);
	mBproduct = matrix(1, N, 1, N);

	indx = ivector(1, N);
	xa = vector(1, N);
	xb = vector(1, N);
	normA = normB = normAr = normBr = 0.0f;

	//Vectors initialization 
	for (int i = 1; i <= N; i++) {
		xa[i] = 0;
		xb[i] = 0;
	}

	//Matrices initialization
	for (int i = 0; i < 9; i++) {
		mA[i/N+1][i%N+1] = i+1;
		mB[i/N+1][i%N+1] = i+1;
		mAor[i/N+1][i%N+1] = i+1;
		mBor[i/N+1][i%N+1] = i+1;
		mAproduct[i/N+1][i%N+1] = 0;
		mBproduct[i/N+1][i%N+1] = 0;
	}
	mB[1][1] = 1.1f;
	mBor[1][1] = 1.1f;

	//Finding norm:
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			if (fabs(mA[i][j]) > normA) {
				normA = fabs(mA[i][j]);
			}
			if (fabs(mB[i][j]) > normB) {
				normB = fabs(mB[i][j]);
			}
		}
	}

	//Calling LU decompose for matrices
	ludcmp(mA, N, indx, &d);
	ludcmp(mB, N, indx, &d);

	//Filling reversed matrices
	for (int i = 1; i <= N; i++) {
		xa[i] = 1.0f;
		xb[i] = 1.0f;

		lubksb(mA, N, indx, xa);
		lubksb(mB, N, indx, xb);
		for (int j = 1; j <= N; j++) {
			mAr[j][i] = xa[j];
			mBr[j][i] = xb[j];
			xa[j] = xb[j] = 0;
		}
	}

	//Finding norm for reversed matrices:
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			if (fabs(mAr[i][j]) > normAr) {
				normAr = fabs(mAr[i][j]);
			}
			if (fabs(mBr[i][j]) > normBr) {
				normBr = fabs(mBr[i][j]);
			}
		}
	}

	printf("Normy: \nA: %g, A-1: %g\nB: %g, B-1: %g\n", normA, normAr, normB, normBr);
	printf("\n");
	kA = normA * normAr;
	kB = normB * normBr;

	//Calculating matrices product:
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			for (int k = 1; k <= N; k++) {
				mAproduct[i][j] += mAor[i][k]*mAr[k][j];
				mBproduct[i][j] += mBor[i][k]*mBr[k][j];
			}
		}
	}

	//Results
	printf("Wskazniki uwarunkowania: kA = %g, kB = %g\n", kA, kB);
	printMatrix(mAproduct, N, "Macierz A*A^-1\n");
	printf("\n");
	printMatrix(mBproduct, N, "Macierz B*B^-1\n");

	free_matrix(mA, 1, N, 1, N);
	free_matrix(mB, 1, N, 1, N);
	free_matrix(mAor, 1, N, 1, N);
	free_matrix(mBor, 1, N, 1, N);
	free_matrix(mAr, 1, N, 1, N);
	free_matrix(mBr, 1, N, 1, N);
	free_matrix(mAproduct, 1, N, 1, N);
	free_matrix(mBproduct, 1, N, 1, N);
	free_vector(xa, 1, N);
	free_vector(xb, 1, N);
	free_ivector(indx, 1, N);
	return 0;
}
