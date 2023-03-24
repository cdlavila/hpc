#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int size;
int **matrix_a;
int **matrix_b;
int **matrix_result;
double execution_time = 0.0;

void multiply_matrices() {
    const int block_size = 32;
    for (int jj = 0; jj < size; jj += block_size) {
        for (int kk = 0; kk < size; kk += block_size) {
            for (int j = jj; j < jj + block_size && j < size; j++) {
                for (int k = kk; k < kk + block_size && k < size; k++) {
                    int temp = matrix_b[k][j];
                    matrix_b[k][j] = matrix_b[j][k];
                    matrix_b[j][k] = temp;
                }
            }
        }
        for (int ii = 0; ii < size; ii += block_size) {
            for (int j = jj; j < jj + block_size && j < size; j++) {
                for (int i = ii; i < ii + block_size && i < size; i++) {
                    int add = 0;
                    for (int k = 0; k < size; k++) {
                        add += matrix_a[j][k] * matrix_b[i][k];
                    }
                    matrix_result[j][i] = add;
                }
            }
        }
    }
}

void read_matrices() {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix_a[i][j] = rand() % 10;
            matrix_b[i][j] = rand() % 10;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("You must provide the argument: size");
        return 0;
    }

    srand(time(NULL));
    size = atoi(argv[1]);

    matrix_a = (int **)malloc(size * sizeof(int *));
    matrix_b = (int **)malloc(size * sizeof(int *));
    matrix_result = (int **)malloc(size * sizeof(int *));

    for (int i = 0; i < size; i++) {
        matrix_a[i] = (int *)malloc(size * sizeof(int));
        matrix_b[i] = (int *)malloc(size * sizeof(int));
        matrix_result[i] = (int *)malloc(size * sizeof(int));
    }

    read_matrices();

    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    multiply_matrices();
    clock_gettime(CLOCK_REALTIME, &end);
    execution_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("%f,", execution_time);

    for (int i = 0; i < size; i++) {
        free(matrix_a[i]);
        free(matrix_b[i]);
        free(matrix_result[i]);
    }

    free(matrix_a);
    free(matrix_b);
    free(matrix_result);

    return 0;
}
