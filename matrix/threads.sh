#!/bin/bash
gcc -pthread threads.c
for k in 1 2 3 4 5 6 7 8 9 10 # Number of iterations
do
  {
      echo "Iteration: ${k}"
      echo ""
  } >> threads.csv
  for i in 100 500 1000 2000 # Matrices sizes
  do
      {
         echo "Matrix size: ${i}"
         echo ""
      } >> threads.csv
      for j in 2 4 8 16 # Number of threads
      do
          {
              echo "Threads number: ${j}"
              ./a.out $i $j
              echo ""
          } >> threads.csv
      done
      echo "" >> threads.csv
      echo "Iteration ${k}, execution finished for matrix size: ${i}"
  done
  echo "" >> threads.csv
done