#!/bin/bash
# Loop over the number of processes
for n in 1 2 4 8 # Number of processes
do
  # Initialize the CSV file with table headers
  echo "|    | 100                | 500                | 1000               | 2000               |" > mpi_${n}.csv
  echo "|----|--------------------|--------------------|--------------------|--------------------|" >> mpi_${n}.csv

  # Loop over iterations
  for k in 1 2 # Number of iterations
  do
    # Print the iteration number
    echo -n "| ${k}  |" >> mpi_${n}.csv

    # Loop over matrix sizes
    for i in 100 500 1000 2000 # Matrices sizes
    do
      # Execute the C program and store the output in a variable
      output=$(mpiexec -n $n ./matrix_mpi $i)

      # Extract the execution time from the output
      execution_time=$(echo "$output" | grep "Execution time:" | awk '{print $3}')

      # Print the execution time in the table
      echo -n " ${execution_time} |" >> mpi_${n}.csv

      # Print console output
      echo "Iteration ${k}, execution finished for matrix size: ${i} with ${n} processes"
    done

    # Print a newline in the table
    echo "" >> mpi_${n}.csv

    # Print a newline in the console
    echo
  done
done
