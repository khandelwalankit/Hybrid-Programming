#!/bin/bash
echo "Setting environment variables"
PKG_CONFIG_PATH=/home/amkhande/lib/pkgconfig
export PKG_CONFIG_PATH
LD_LIBRARY_PATH=/home/amkhande/lib
export LD_LIBRARY_PATH
echo "Setting Work Stealing Scheduling"
STARPU_SCHED=ws
export STARPU_SCHED

echo "removing earlier test file"
rm -i testresult_matrixmult.txt
echo "Compiling MatrixMult"
gcc `pkg-config starpu-1.1 --cflags` matrixmult.c -fplugin=`pkg-config starpu-1.1 --variable=gccplugin` `pkg-config starpu-1.1 --libs` -o matrixmult
echo "Computation Started for matrixmult" >> testresult_matrixmult.txt
max=10
for i in `seq 1 $max`
  do
    echo "Executing test :$i times"
    echo "Executing test :$i times" >> testresult_matrixmult.txt
    echo "Running matrix multiplication for 1024X1024 and dividing in block of 512X512" >> testresult_matrixmult.txt
    ./matrixmult 2 2 1024 1024 1024 >> testresult_matrixmult.txt
    echo "Running matrix multiplication for 1024X1024 and dividing in block of 256X256" >> testresult_matrixmult.txt
    ./matrixmult 4 4 1024 1024 1024 >> testresult_matrixmult.txt
    echo "Running matrix multiplication for 1024X1024 and dividing in block of 128X128" >> testresult_matrixmult.txt
    ./matrixmult 8 8 1024 1024 1024 >> testresult_matrixmult.txt
    echo "Running matrix multiplication for 1024X1024 and dividing in block of 64X64" >> testresult_matrixmult.txt
    ./matrixmult 16 16 1024 1024 1024 >> testresult_matrixmult.txt
    echo "Running matrix multiplication for 1024X1024 and dividing in block of 32X32" >> testresult_matrixmult.txt
    ./matrixmult 32 32 1024 1024 1024 >> testresult_matrixmult.txt
    echo "Running matrix multiplication for 1024X1024 and dividing in block of 16X16" >> testresult_matrixmult.txt
    ./matrixmult 64 64 1024 1024 1024 >> testresult_matrixmult.txt
    echo "Running matrix multiplication for 2048X2048 and dividing in block of 512X512" >> testresult_matrixmult.txt
    ./matrixmult 4 4 2048 2048 2048 >> testresult_matrixmult.txt
    echo "Running matrix multiplication for 2048X2048 and dividing in block of 256X256" >> testresult_matrixmult.txt
    ./matrixmult 8 8 2048 2048 2048 >> testresult_matrixmult.txt
    echo "Running matrix multiplication for 2048X2048 and dividing in block of 128X128" >> testresult_matrixmult.txt
    ./matrixmult 16 16 2048 2048 2048 >> testresult_matrixmult.txt
    echo "Running matrix multiplication for 2048X2048 and dividing in block of 64X64" >> testresult_matrixmult.txt
    ./matrixmult 32 32 2048 2048 2048 >> testresult_matrixmult.txt
    echo "Running matrix multiplication for 2048X2048 and dividing in block of 32X32" >> testresult_matrixmult.txt
    ./matrixmult 64 64 2048 2048 2048 >> testresult_matrixmult.txt
    echo "Running matrix multiplication for 2048X2048 and dividing in block of 16X16" >> testresult_matrixmult.txt
    ./matrixmult 128 128 2048 2048 2048 >> testresult_matrixmult.txt
    echo "Running matrix multiplication for 4096X4096 and dividing in block of 512X512" >> testresult_matrixmult.txt
    ./matrixmult 8 8 4096 4096 4096 >> testresult_matrixmult.txt
    echo "Running matrix multiplication for 4096X4096 and dividing in block of 256X256" >> test.txt
    ./matrixmult 16 16 4096 4096 4096 >> testresult_matrixmult.txt
    echo "Running matrix multiplication for 4096X4096 and dividing in block of 128X128" >> testresult_matrixmult.txt
    ./matrixmult 32 32 4096 4096 4096 >> testresult_matrixmult.txt
    echo "Running matrix multiplication for 4096X4096 and dividing in block of 64X64" >> testresult_matrixmult.txt
    ./matrixmult 64 64 4096 4096 4096 >> testresult_matrixmult.txt
    echo "Running matrix multiplication for 4096X4096 and dividing in block of 32X32" >> testresult_matrixmult.txt
    ./matrixmult 128 128 4096 4096 4096 >> testresult_matrixmult.txt
    echo "Running matrix multiplication for 4096X4096 and dividing in block of 16X16" >> testresult_matrixmult.txt
    ./matrixmult 256 256 4096 4096 4096 >> testresult_matrixmult.txt
  done

echo "Computation Completed for matrixmult" >> testresult_matrixmult.txt

