#include <stdio.h>
#include <omp.h>

int main() {
    int i;
    int num_threads = 4;

#pragma omp parallel num_threads(num_threads)
    {
        int thread_id = omp_get_thread_num();
        printf("Hola desde el hilo %d de %d hilos.\n", thread_id, num_threads);

#pragma omp for
        for (i = 0; i < 8; i++) {
            printf("Iteración %d realizada por el hilo %d.\n", i, thread_id);
        }
    }

    return 0;
}