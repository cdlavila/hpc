#!/bin/bash
gcc -fopenmp secuencial.c -o secuential
for thread in {1..6}
do
    echo "THREAD: ${thread}" >> times.csv
    echo "Iteration, Size" >> times.csv
    for size in 100 500 1000 2000
    do
        echo "${size}" >> times.csv
        for i in {1..10}
        do
            OMP_NUM_THREADS=$thread ./secuential $size >> times.csv
        done
    done
    echo "------------------" >> times.csv
done