#!/bin/bash
gcc -pthread processes.c
for k in 1 2 3 4 5 6 7 8 9 10 # Number of iterations
do
  {
      echo "Iteration: ${k}"
      echo ""
  } >> processes.csv
  for i in 100 500 1000 2000 3000  # Matrices sizes
  do
      {
         echo "Matrix size: ${i}"
         echo ""
      } >> processes.csv
      for j in 2 4 8 16 # Number of processes
      do
          {
              echo "Processes number: ${j}"
              ./a.out $i $j
              echo ""
          } >> processes.csv
      done
      echo "" >> processes.csv
      echo "Execution finished for matrix size: ${i}"
  done
  echo "" >> processes.csv
done