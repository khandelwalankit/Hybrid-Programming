#!/bin/bash
echo "Removing earlier Computed file"
rm -i testresult_matrixmult.txt
echo "Compiling MatrixMult"
gcc -fopenmp -O3 matrixmult_omp.c -o matrixmult_omp
echo "Computation Started for matrixmult" >> testresult_matrixmult.txt
max=8
for i in `seq 1 $max`
    do
       echo "Executing test :$i times"
       echo "Executing test :$i times" >> testresult_matrixmult.txt
       echo "Running matrix multiplication for 1024X1024" >> testresult_matrixmult.txt
       ./matrixmult_omp 1024 1024 1024 >> testresult_matrixmult.txt
       echo "Running matrix multiplication for 2048X2048" >> testresult_matrixmult.txt
       ./matrixmult_omp 2048 2048 2048 >> testresult_matrixmult.txt
       echo "Running matrix multiplication for 4096X4096" >> testresult_matrixmult.txt
       ./matrixmult_omp 4096 4096 4096 >> testresult_matrixmult.txt
    done
