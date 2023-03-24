#!/bin/bash
gcc transposed_cache.c
for k in 1 2 3 4 5 6 7 8 9 10 # Number of iterations
do
  {
      echo "Iteration: ${k}"
      echo ""
  } >> transposed_cache.csv
  for i in 100 500 1000 2000 3000  # Matrices sizes
  do
      {
          echo "Matrix size: ${i}"
          ./a.out $i 0
          echo ""
          echo ""
      } >> transposed_cache.csv
      echo "Iteration ${k}, execution finished for matrix size: ${i}"
  done
  echo "" >> transposed_cache.csv
done