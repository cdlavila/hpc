#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <mpi.h>

void matrix_multiply(int n, int local_n, int *local_A, int *B, int *local_C) {
    int i, j, k;
    for (i = 0; i < local_n; i++) {
        for (j = 0; j < n; j++) {
            local_C[i * n + j] = 0;
            for (k = 0; k < n; k++) {
                local_C[i * n + j] += local_A[i * n + k] * B[k * n + j];
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s [tamaño de la matriz] [numero de procesos]\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int num_procs = atoi(argv[2]);

    MPI_Init(NULL, NULL);

    int rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int local_n = n / num_procs;
    int remainder = n % num_procs;
    if (rank < remainder) {
        local_n++;
    }

    // Reserva memoria para las matrices B, local_A y local_C
    int *B = (int *)malloc(n * n * sizeof(int));
    int *local_A = (int *)malloc(local_n * n * sizeof(int));
    int *local_C = (int *)malloc(local_n * n * sizeof(int));

    // Inicializa la semilla del generador de números aleatorios
    srand(time(NULL));

    // Proceso 0 inicializa la matriz B con números aleatorios
    if (rank == 0) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                B[i * n + j] = rand() % 100;
            }
        }
    }

    // Scatter la matriz B a todos los procesos
    MPI_Bcast(B, n * n, MPI_INT, 0, MPI_COMM_WORLD);

    // Scatter la matriz A entre los procesos
    MPI_Scatter(B, local_n * n, MPI_INT, local_A, local_n * n, MPI_INT, 0, MPI_COMM_WORLD);

    struct timeval inicio, fin;
    gettimeofday(&inicio, NULL);

    // Multiplicación de matrices local
    matrix_multiply(n, local_n, local_A, B, local_C);

    gettimeofday(&fin, NULL);
    double tiempo_transcurrido = ((fin.tv_sec - inicio.tv_sec) * 1000000 + (fin.tv_usec - inicio.tv_usec)) / 1000000.0;

    double execution_time;
    MPI_Reduce(&tiempo_transcurrido, &execution_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Execution time: %f seconds\n", execution_time);
    }

    // Liberar memoria
    free(B);
    free(local_A);
    free(local_C);

    MPI_Finalize();

    return 0;
}