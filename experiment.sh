#!/bin/bash

# Compile the C++ program
g++ -o kdtree_experiment std_kdtree.cpp -std=c++11

# Define the lists of parameters
types=(standard relaxed)                        # Type of tree
ns=(10 20 50 100 200 500 700 1000 2000 3000 5000 7000 10000 12000 15000 20000)     # tree sizes
ks=(2 3 5 7 10 15 20 25)                        # dimensions
qs=(10 15 20 25 30 35 50 100 200)                          # numbers of queries
ss=(1 2 3 4 5)                                  # numbers of specified coordinates

# CSV file to store results
csv_file="experiment_results.csv"

# Create or append the header to the CSV file
echo "type,n,k,q,s,Average visited nodes,Variance" > "$csv_file"

# Loop over all combinations of parameters
for t in "${types[@]}"; do
  for n in "${ns[@]}"; do
    for k in "${ks[@]}"; do
      for q in "${qs[@]}"; do
        for s in "${ss[@]}"; do
          if [ "$s" -le "$k" ]; then  # Ensure s does not exceed k
            echo "Running experiment with type=$t, n=$n, k=$k, q=$q, s=$s"
            # Run the C++ program and capture output
            output=$(./kdtree_experiment "$t" "$n" "$k" "$q" "$s")

            # Parse the output to extract average and variance
            average=$(echo "$output" | awk '/Average visited nodes/ {print $4}')
            variance=$(echo "$output" | awk '/Variance/ {print $2}')
            # Append results to CSV file
            echo "$t,$n,$k,$q,$s,$average,$variance" >> "$csv_file"
            echo "--------------------------------------"
          fi
        done
      done
    done
  done
done