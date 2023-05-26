#!/bin/bash
gcc sequential.c -o sequential

# Initialize the CSV file with table headers
echo "|    | 100                | 500                | 1000               | 2000               |" > sequential.csv
echo "|----|--------------------|--------------------|--------------------|--------------------|" >> sequential.csv

# Loop over iterations
for k in 1 2 3 4 5 6 7 8 9 10 # Number of iterations
do
  # Print the iteration number
  echo -n "| ${k}  |" >> sequential.csv

  # Loop over matrix sizes
  for i in 100 500 1000 2000 # Matrices sizes
  do
    # Execute the C program and store the output in a variable
    output=$(./sequential $i 0)

    # Extract the execution time from the output
    execution_time=$(echo "$output" | grep "Execution time:" | awk '{print $3}')

    # Print the execution time in the table
    echo -n " ${execution_time} |" >> sequential.csv

    # Print console output
    echo "Iteration ${k}, execution finished for matrix size: ${i}"
  done

  # Print a newline in the table
  echo "" >> sequential.csv

  # Print a newline in the console
  echo
done
