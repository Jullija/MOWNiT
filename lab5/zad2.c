#include <stdio.h>
#include <stdlib.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_sf_bessel.h>
#include <sys/times.h>
#include <unistd.h>
#include <fcntl.h>
struct tms timeStart, timeEnd;
clock_t clockStart, clockEnd;

void startTimer(){
    clockStart = times(&timeStart);
}
double endTimer(){
    clockEnd = times(&timeEnd);
    int tiktak = sysconf(_SC_CLK_TCK);
    double real = (double)(clockEnd-clockStart) / tiktak;
    return real;
}
void naive_multiplication(double** A, double** B, double** C, int size){
    for (int i = 0; i < size;i++){
        for (int j = 0; j < size; j++){
            for (int k = 0; k < size; k ++){
                C[i][j] += A[i][k]*B[k][j];
            }
        }     
    }
}
void better_multiplication(double** A, double** B, double** C, int size){
    for (int i = 0; i < size; i++){
        for (int k = 0; k < size; k ++){
            for (int j = 0; j < size; j++){
                C[i][j] += A[i][k]*B[k][j];
            }
        }
    }    
}

void blas_multiplication(double* a, double* b, double* c, int rows){
    gsl_matrix_view D = gsl_matrix_view_array(a, rows, rows);
    gsl_matrix_view E = gsl_matrix_view_array(b, rows, rows);
    gsl_matrix_view F = gsl_matrix_view_array(c, rows, rows);
    gsl_blas_dgemm (CblasNoTrans, CblasNoTrans,
                  1.0, &D.matrix, &E.matrix,
                  0.0, &F.matrix);
}

double** createMatrix(int size){
    double** A = calloc(size, sizeof(double*));
    for(int i = 0; i < size; i++){
        A[i] = calloc(size, sizeof(double));
        for(int j = 0; j < size; j++){
            A[i][j] = rand()%10;
        }
    }

    return A;

}

double** createMatrixZeros(int size){
    double** A = calloc(size, sizeof(double*));
    for(int i = 0; i < size; i++){
        A[i] = calloc(size, sizeof(double));
        for(int j = 0; j < size; j++){
            A[i][j] = 0;
        }
    }

    return A;

}

void freeMatrix(double** A, int size){
    for(int i = 0; i < size; i++){
        free(A[i]);
    }
    free(A);
}

int main(int argc, char** argv){
    
    double timeNaive, timeBetter, timeBLAS;
    FILE* result = fopen("result.csv", "w");
    fprintf(result, "%s, %s, %s", "Size", "Type", "Time");

    for(int i = 1; i < 500; i+=10){
        for (int j = 1; j < 10; j++){
            double** A, **B, **C;
            A = createMatrix(i);
            B = createMatrix(i);
            C = createMatrixZeros(i);


            //naive
            startTimer();
            naive_multiplication(A, B, C, i);
            timeNaive = endTimer();
            freeMatrix(C, i);
            fprintf(result, "\n%d, %s, %f", i, "Naive", timeNaive);

            //better
            C = createMatrixZeros(i);
            startTimer();
            better_multiplication(A, B, C, i);
            timeBetter = endTimer();
            fprintf(result, "\n%d, %s, %f", i, "Better", timeBetter);


            freeMatrix(A,i);
            freeMatrix(B,i);
            freeMatrix(C,i);

            //BLAS
            double *a = calloc(i*i, sizeof(double));
            double *b = calloc(i*i, sizeof(double));
            double *c = calloc(i*i, sizeof(double));
            for (int k = 0; k < i*i; k++){
                a[i] = rand()%10;
                b[i] = rand()%10;
                c[i] = 0.0;
            }

            startTimer();
            blas_multiplication(a, b, c, i);
            timeBLAS = endTimer();
            fprintf(result, "\n%d, %s, %f", i, "BLAS", timeBLAS);
            free(a);
            free(b);
            free(c);

            
        }
    }
    fclose(result);
    return 0;
}