#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define NUM_THREADS 4

int **matrix_a;
int **matrix_b;
int **matrix_result;

double execution_time = 0.0;

void *multiply_matrices(void *thread_id, int n)
{
    clock_t begin = clock();

    long tid = (long)thread_id;
    printf("Hello World! It's me, thread #%ld!\n", tid);

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
    pthread_exit(NULL);
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
    pthread_t threads[NUM_THREADS];
    srand(time(NULL));

    if (argc < 3) {
        printf("You must provide the two arguments: size of the matrix and show matrix (0 or 1)");
        return 0;
    }

    int n = atoi(argv[1]);
    short show_matrix = atoi(argv[2]);

    matrix_a = (int **) malloc(n * sizeof(int *));
    matrix_b = (int **) malloc(n * sizeof(int *));
    matrix_result = (int **) malloc(n * sizeof(int *));

    for (int i = 0; i < n; i++)
    {
        matrix_a[i] = (int *) malloc(n * sizeof(int));
        matrix_b[i] = (int *) malloc(n * sizeof(int));
        matrix_result[i] = (int *) malloc(n * sizeof(int));
    }

    read_matrix(matrix_a, n);
    read_matrix(matrix_b, n);

    // Threads
    int created_thread;
    long t;
    for (t = 0; t < NUM_THREADS; t++) {
        printf("In main: creating thread %ld\n", t);
        created_thread = pthread_create(&threads[t], NULL, (void *(*)(void *)) multiply_matrices, (void *)t);
        if (created_thread) {
            printf("ERROR; return code from pthread_create() is %d\n", created_thread);
            exit(-1);
        }
    }

    // for (t = 0; t < NUM_THREADS; t++) {
    // if (pthread_join(threads[t], NULL)) {
    //   printf("Error joining thread %ld\n", t);
    // }
    // }

    // Print matrices
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

    pthread_exit(NULL);
}