#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <mpi.h>
#define num_max 10

double execution_time = 0.0;
int **matrix_a;
int **matrix_b;
int **matrix_result;
int n;

void multiply_matrices(int start_row, int end_row)
{
    for (int i = start_row; i < end_row; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int add = 0;

            for (int k = 0; k < n; k++)
            {
                add += matrix_a[i][k] * matrix_b[k][j];
            }
            matrix_result[i][j] = add;
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
    MPI_Init(&argc, &argv);

    int num_processes, process_id;
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_id);

    if (argc != 2)
    {
        if (process_id == 0)
        {
            printf("You must provide one argument: size of the matrix\n");
        }
        MPI_Finalize();
        return 0;
    }

    srand(time(NULL));
    n = atoi(argv[1]);

    int rows_per_process = n / num_processes;
    int start_row = process_id * rows_per_process;
    int end_row = (process_id + 1) * rows_per_process;

    if (process_id == num_processes - 1)
    {
        end_row = n;
    }

    matrix_a = (int **)malloc(n * sizeof(int *));
    matrix_b = (int **)malloc(n * sizeof(int *));
    matrix_result = (int **)malloc(n * sizeof(int *));

    for (int i = 0; i < n; i++)
    {
        matrix_a[i] = (int *)malloc(n * sizeof(int));
        matrix_b[i] = (int *)malloc(n * sizeof(int));
        matrix_result[i] = (int *)malloc(n * sizeof(int));
    }

    if (process_id == 0)
    {
        read_matrix();
    }

    MPI_Bcast(matrix_a[0], n * n, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(matrix_b[0], n * n, MPI_INT, 0, MPI_COMM_WORLD);

    struct timespec start, end;
    MPI_Barrier(MPI_COMM_WORLD); // Synchronize all processes
    clock_gettime(CLOCK_REALTIME, &start);

    multiply_matrices(start_row, end_row);

    MPI_Barrier(MPI_COMM_WORLD); // Synchronize all processes
    clock_gettime(CLOCK_REALTIME, &end);

    execution_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    if (process_id == 0)
    {
        printf("%f\n", execution_time);
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

    MPI_Finalize();

    return 0;
}