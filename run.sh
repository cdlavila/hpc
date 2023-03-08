#!/bin/bash
for i in 100 200 500  # Matrices sizes
do
    echo "Size ${i}" >> times.csv
    for k in {1..4} # Number of threads
    do
        echo "Threads ${k}: " >> times.csv
        for j in {1..3} # Repeat the process for each size and number of threads 3 times
        do
            # The results are saved in the file times.csv
            ./threads $i $k >> times.csv
        done
    done
    echo "" >> times.csv
done