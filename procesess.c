#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

int size;
int num_processes;
int **matrix_a;
int **matrix_b;
int **matrix_result;
double execution_time = 0.0;

void multiply_matrices(int start_row, int end_row) {
    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < size; j++) {
            int add = 0;
            for (int k = 0; k < size; k++) {
                add += matrix_a[i][k] * matrix_b[k][j];
            }
            matrix_result[i][j] = add;
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
    if (argc < 3) {
        printf("You must provide two arguments: size of the matrix and number of processes\n");
        exit(1);
    }

    srand(time(NULL));
    size = atoi(argv[1]);
    num_processes = atoi(argv[2]);

    if (num_processes > size) {
        printf("The number of processes must be less than or equal to the size of the matrix");
        exit(1);
    }

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

    pid_t pid;
    for (int i = 0; i < num_processes; i++) {
        pid = fork();

        if (pid < 0) {
            printf("Error creating child process\n");
            exit(1);
        }
        else if (pid == 0) {
            int rows_per_process = size / num_processes;
            int start_row = i * rows_per_process;
            int end_row = (i + 1) * rows_per_process;
            multiply_matrices(start_row, end_row);
            exit(0);
        }
    }

    int status;
    for (int i = 0; i < num_processes; i++) {
        wait(&status);
    }

    clock_gettime(CLOCK_REALTIME, &end);
    execution_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("Execution Time: %f seconds\n", execution_time);

    for (int i = 0; i < size; i++) {
        free(matrix_a[i]);
        free(matrix_b[i]);
        free(matrix_result[i]);
    }

    free(matrix_result);

    return 0;
}