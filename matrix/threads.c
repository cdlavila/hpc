#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

int size;
int num_threads;
int **matrix_a;
int **matrix_b;
int **matrix_result;
double execution_time = 0.0;

void *multiply_matrices(void *arg) {
    long pos_thread = (long)arg;
    int rows_per_thread = size / num_threads;
    int start_row = pos_thread * rows_per_thread;
    int end_row = (pos_thread + 1) * rows_per_thread;

    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < size; j++) {
            int add = 0;
            for (int k = 0; k < size; k++) {
                add += matrix_a[i][k] * matrix_b[k][j];
            }
            matrix_result[i][j] = add;
        }
    }
    pthread_exit(NULL);
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
    if (argc < 3) {
        printf("You must provide two arguments: size of the matrix and number of threads\n");
        exit(1);
    }

    srand(time(NULL));
    size = atoi(argv[1]);
    num_threads = atoi(argv[2]);

    if (num_threads > size) {
        printf("The number of threads must be less than or equal to the size of the matrix");
        exit(1);
    }

    pthread_t threads[num_threads];

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

    for (long i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, multiply_matrices, (void *)i);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_gettime(CLOCK_REALTIME, &end);
    execution_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("Execution Time: %f seconds\n", execution_time);

    for (int i = 0; i < size; i++) {
        free(matrix_a[i]);
        free(matrix_b[i]);
        free(matrix_result[i]);
    }

    free(matrix_a);
    free(matrix_b);
    free(matrix_result);

    pthread_exit(NULL);
}