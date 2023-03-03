#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double execution_time = 0.0;

void multiply_matrices(int **matrix_a, int **matrix_b, int **matrix_result, int n)
{
    clock_t begin = clock();

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int suma = 0;

            for (int k = 0; k < n; k++) {
                suma += matrix_a[j][k] * matrix_b[k][i];
            }
            matrix_result[j][i] = suma;
        }
    }

    clock_t fin = clock();
    execution_time = (double)(fin - begin) / CLOCKS_PER_SEC;
}

int generate_random_number(int max) {
    return rand() % max;
}

void print_matrix(int **matrix, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void read_matrix(int **matrix, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            matrix[i][j] = generate_random_number(10);
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 3) {
        printf("You must provide the two arguments: size of the matrix and show matrix (0 or 1)");
        return 0;
    }

    srand(time(NULL));
    int n = atoi(argv[1]);
    short show_matrix = atoi(argv[2]);

    int **matrix_a = (int **) malloc(n * sizeof(int *));
    int **matrix_b = (int **) malloc(n * sizeof(int *));
    int **matrix_result = (int **) malloc(n * sizeof(int *));

    for (int i = 0; i < n; i++)
    {
        matrix_a[i] = (int *) malloc(n * sizeof(int));
        matrix_b[i] = (int *) malloc(n * sizeof(int));
        matrix_result[i] = (int *) malloc(n * sizeof(int));
    }

    read_matrix(matrix_a, n);
    read_matrix(matrix_b, n);
    multiply_matrices(matrix_a, matrix_b, matrix_result, n);

    if (show_matrix)
    {
        printf("Matrix A:\n");
        print_matrix(matrix_a, n);
        printf("Matrix B:\n");
        print_matrix(matrix_b, n);
        printf("Matrix Result:\n");
        print_matrix(matrix_result, n);
    }

    for (int i = 0; i < n; i++)
    {
        free(matrix_a[i]);
        free(matrix_b[i]);
        free(matrix_result[i]);
    }
    free(matrix_a);
    free(matrix_b);
    free(matrix_result);

    printf("Execution time: %f seconds", execution_time);

    return 0;
}