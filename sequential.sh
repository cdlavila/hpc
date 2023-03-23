#!/bin/bash
gcc sequential.c
for k in 1 2 3 4 5 6 7 8 9 10 # Number of iterations
do
  {
      echo "Iteration: ${k}"
      echo ""
  } >> sequential.csv
  for i in 100 500 1000 2000 3000  # Matrices sizes
  do
      {
          echo "Matrix size: ${i}"
          ./a.out $i 0
          echo ""
          echo ""
      } >> sequential.csv
      echo "Iteration ${k}, execution finished for matrix size: ${i}"
  done
  echo "" >> sequential.csv
done