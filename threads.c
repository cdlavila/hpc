#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

int n;
int num_threads;
int **matrix_a;
int **matrix_b;
int **matrix_result;
double execution_time = 0.0;

void *multiply_matrices(void *arg) {
    long pos_thread = (long)arg;
    int rows_per_thread = n / num_threads;
    int start_row = pos_thread * rows_per_thread;
    int end_row = (pos_thread + 1) * rows_per_thread;

    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < n; j++) {
            int add = 0;
            for (int k = 0; k < n; k++) {
                add += matrix_a[j][k] * matrix_b[k][i];
            }
            matrix_result[j][i] = add;
        }
    }

    pthread_exit(NULL);
}

int generate_random_number(int max) {
    return rand() % max;
}

void read_matrices() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix_a[i][j] = generate_random_number(100);
            matrix_b[i][j] = generate_random_number(100);
        }
    }
}

int main(int argc, char *argv[]) {
    if ( argc < 3 ) {
        printf("You must provide two arguments: size of the matrix and number of threads\n");
        exit(1);
    }

    srand(time(NULL));
    n = atoi(argv[1]);
    num_threads = atoi(argv[2]);

    if ( num_threads > n ) {
        printf("The number of threads must be less than or equal to the size of the matrix");
        exit(1);
    }

    pthread_t threads[num_threads];

    matrix_a = (int **)malloc(n * sizeof(int *));
    matrix_b = (int **)malloc(n * sizeof(int *));
    matrix_result = (int **)malloc(n * sizeof(int *));

    for (int i = 0; i < n; i++) {
        matrix_a[i] = (int *)malloc(n * sizeof(int));
        matrix_b[i] = (int *)malloc(n * sizeof(int));
        matrix_result[i] = (int *)malloc(n * sizeof(int));
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

    for (int i = 0; i < n; i++) {
        free(matrix_a[i]);
        free(matrix_b[i]);
        free(matrix_result[i]);
    }

    free(matrix_a);
    free(matrix_b);
    free(matrix_result);

    pthread_exit(NULL);
}