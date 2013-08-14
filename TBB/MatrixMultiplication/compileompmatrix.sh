#!/bin/bash
echo "Removing earlier Computed file"
rm -i testresult_matrixmult.txt
echo "Compiling MatrixMult"
g++ -O3 -std=c++0x matrixmult_tomp.cpp -L/home/amkhande/tbbSource/lib/intel64/gcc4.4 -ltbb -o matrixmult_tomp
echo "Computation Started for matrixmult" >> testresult_matrixmult.txt
max=8
for i in `seq 1 $max`
 do
    echo "Executing test :$i times"
    echo "Executing test :$i times" >> testresult_matrixmult.txt
    echo "Running matrix multiplication for 1024X1024" >> testresult_matrixmult.txt
    ./matrixmult_tomp 1024 1024 1024 >> testresult_matrixmult.txt
    echo "Running matrix multiplication for 2048X2048" >> testresult_matrixmult.txt
    ./matrixmult_tomp 2048 2048 2048 >> testresult_matrixmult.txt
    echo "Running matrix multiplication for 4096X4096" >> testresult_matrixmult.txt
    ./matrixmult_tomp 4096 4096 4096 >> testresult_matrixmult.txt
 done
