#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

double execution_time = 0.0;

void multiply_matrices(int **matrix_a, int **matrix_b, int **matrix_result, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int sum = 0;
            for (int k = 0; k < size; k++) {
                sum += matrix_a[j][k] * matrix_b[k][i];
            }
            matrix_result[j][i] = sum;
        }
    }
}

void read_matrices(int **matrix_a, int **matrix_b, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix_a[i][j] = rand() % 10;
            matrix_b[i][j] = rand() % 10;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("You must provide the two arguments: size of the matrix and show matrix (0 or 1)");
        return 0;
    }

    srand(time(NULL));
    int size = atoi(argv[1]);

    int rank, num_procs;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    int **matrix_a = (int **) malloc(size * sizeof(int *));
    int **matrix_b = (int **) malloc(size * sizeof(int *));
    int **matrix_result = (int **) malloc(size * sizeof(int *));

    for (int i = 0; i < size; i++) {
        matrix_a[i] = (int *) malloc(size * sizeof(int));
        matrix_b[i] = (int *) malloc(size * sizeof(int));
        matrix_result[i] = (int *) malloc(size * sizeof(int));
    }

    read_matrices(matrix_a, matrix_b, size);

    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    multiply_matrices(matrix_a, matrix_b, matrix_result, size);

    clock_gettime(CLOCK_REALTIME, &end);
    execution_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    for (int i = 0; i < size; i++) {
        free(matrix_a[i]);
        free(matrix_b[i]);
        free(matrix_result[i]);
    }
    free(matrix_a);
    free(matrix_b);
    free(matrix_result);

    if (rank == 0) {
        printf("Execution time: %f seconds\n", execution_time);
    }

    MPI_Finalize();

    return 0;
}