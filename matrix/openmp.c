#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#define num_max 10

double execution_time = 0.0;
int **matrix_a;
int **matrix_b;
int **matrix_result;
int n;

void multiply_matrices()
{
#pragma omp parallel
    {
        int id = omp_get_thread_num();
        int num_threads = omp_get_num_threads();
        int block_size = (n - 1) / num_threads + 1; // tamaño de bloque que cada hilo debe procesar

        int start = id * block_size;
        int end = (id + 1) * block_size;
        if (end > n) end = n;

        for (int i = start; i < end; i++) {
            for (int j = 0; j < n; j++) {
                int add = 0;

                for (int k = 0; k < n; k++) {
                    add += matrix_a[j][k] * matrix_b[k][i];
                }

#pragma omp critical
                matrix_result[i][j] = add;
            }
        }
    }

}


void read_matrix()
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            matrix_a[i][j] = rand() % num_max;
            matrix_b[i][j] = rand() % num_max;
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc > 2)
    {
        printf("You must provide one argument: size of the matrix");
        return 0;
    }

    n = atoi(argv[1]); // tamano de la matriz

    matrix_a = (int **)malloc(n * sizeof(int *));
    matrix_b = (int **)malloc(n * sizeof(int *));
    matrix_result = (int **)malloc(n * sizeof(int *));

    for (int i = 0; i < n; i++)
    {
        matrix_a[i] = (int *)malloc(n * sizeof(int));
        matrix_b[i] = (int *)malloc(n * sizeof(int));
        matrix_result[i] = (int *)malloc(n * sizeof(int));
    }

    read_matrix();

    double start_time, end_time;
    start_time = omp_get_wtime();
    multiply_matrices();
    end_time = omp_get_wtime();
    execution_time = end_time - start_time;

    printf("%f\n", execution_time);

    for (int i = 0; i < n; i++)
    {
        free(matrix_a[i]);
        free(matrix_b[i]);
        free(matrix_result[i]);
    }

    free(matrix_a);
    free(matrix_b);
    free(matrix_result);

    return 0;
}
