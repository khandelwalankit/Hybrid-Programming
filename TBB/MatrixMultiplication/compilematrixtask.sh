#!/bin/bash
echo "Setting environment variables"
PATH=/home/amkhande/bin:/usr/local/bin:/usr/bin:/bin:/opt/bin:/usr/x86_64-pc-linux-gnu/gcc-bin/4.5.4:/usr/games/bin:/opt/cuda/bin:/opt/cuda/libnvvp:/home/amkhande/tbbSource/bin
export PATH
LD_LIBRARY_PATH=/home/amkhande/lib:/home/amkhande/tbbSource/lib/intel64/gcc4.4
export LD_LIBRARY_PATH
CPLUS_INCLUDE_PATH=/home/amkhande/tbbSource/include:/home/amkhande/include
export CPLUS_INCLUDE_PATH
C_INCLUDE_PATH=/home/amkhande/include
export C_INCLUDE_PATH

echo "removing earlier test file"
rm -i testresult_matrixmult.txt
echo "Compiling MatrixMult"
g++ -O3 -std=c++0x matrixmult_tbb.cpp -L/home/amkhande/tbbSource/lib/intel64/gcc4.4 -ltbb -o matrixmult_tbb
echo "Computation Started for matrixmult" >> testresult_matrixmult.txt
max=8
for i in `seq 1 $max`
  do
    echo "Executing test :$i times"
    echo "Executing test :$i times" >> testresult_matrixmult.txt
    echo "Running matrix multiplication for 1024X1024 and dividing in block of 512X512" >> testresult_matrixmult.txt
    ./matrixmult_tbb 512 512 1024 1024 1024 >> testresult_matrixmult.txt
    echo "Running matrix multiplication for 1024X1024 and dividing in block of 256X256" >> testresult_matrixmult.txt
    ./matrixmult_tbb 256 256 1024 1024 1024 >> testresult_matrixmult.txt
    echo "Running matrix multiplication for 1024X1024 and dividing in block of 128X128" >> testresult_matrixmult.txt
    ./matrixmult_tbb 128 128 1024 1024 1024 >> testresult_matrixmult.txt
    echo "Running matrix multiplication for 1024X1024 and dividing in block of 64X64" >> testresult_matrixmult.txt
    ./matrixmult_tbb 64 64 1024 1024 1024 >> testresult_matrixmult.txt
    echo "Running matrix multiplication for 1024X1024 and dividing in block of 32X32" >> testresult_matrixmult.txt
    ./matrixmult_tbb 32 32 1024 1024 1024 >> testresult_matrixmult.txt
    echo "Running matrix multiplication for 1024X1024 and dividing in block of 16X16" >> testresult_matrixmult.txt
    ./matrixmult_tbb 16 16 1024 1024 1024 >> testresult_matrixmult.txt
    echo "Running matrix multiplication for 2048X2048 and dividing in block of 512X512" >> testresult_matrixmult.txt
    ./matrixmult_tbb 512 512 2048 2048 2048 >> testresult_matrixmult.txt
    echo "Running matrix multiplication for 2048X2048 and dividing in block of 256X256" >> testresult_matrixmult.txt
    ./matrixmult_tbb 256 256 2048 2048 2048 >> testresult_matrixmult.txt
    echo "Running matrix multiplication for 2048X2048 and dividing in block of 128X128" >> testresult_matrixmult.txt
    ./matrixmult_tbb 128 128 2048 2048 2048 >> testresult_matrixmult.txt
    echo "Running matrix multiplication for 2048X2048 and dividing in block of 64X64" >> testresult_matrixmult.txt
    ./matrixmult_tbb 64 64 2048 2048 2048 >> testresult_matrixmult.txt
    echo "Running matrix multiplication for 2048X2048 and dividing in block of 32X32" >> testresult_matrixmult.txt
    ./matrixmult_tbb 32 32 2048 2048 2048 >> testresult_matrixmult.txt
    echo "Running matrix multiplication for 2048X2048 and dividing in block of 16X16" >> testresult_matrixmult.txt
    ./matrixmult_tbb 16 16 2048 2048 2048 >> testresult_matrixmult.txt
    echo "Running matrix multiplication for 4096X4096 and dividing in block of 512X512" >> testresult_matrixmult.txt
    ./matrixmult_tbb 512 512 4096 4096 4096 >> testresult_matrixmult.txt
    echo "Running matrix multiplication for 4096X4096 and dividing in block of 256X256" >> testresult_matrixmult.txt
    ./matrixmult_tbb 256 256 4096 4096 4096 >> testresult_matrixmult.txt
    echo "Running matrix multiplication for 4096X4096 and dividing in block of 128X128" >> testresult_matrixmult.txt
    ./matrixmult_tbb 128 128 4096 4096 4096 >> testresult_matrixmult.txt
    echo "Running matrix multiplication for 4096X4096 and dividing in block of 64X64" >> testresult_matrixmult.txt
    ./matrixmult_tbb 64 64 4096 4096 4096 >> testresult_matrixmult.txt
    echo "Running matrix multiplication for 4096X4096 and dividing in block of 32X32" >> testresult_matrixmult.txt
    ./matrixmult_tbb 32 32 4096 4096 4096 >> testresult_matrixmult.txt
    echo "Running matrix multiplication for 4096X4096 and dividing in block of 16X16" >> testresult_matrixmult.txt
    ./matrixmult_tbb 16 16 4096 4096 4096 >> testresult_matrixmult.txt
  done

echo "Computation Completed for matrixmult" >> testresult_matrixmult.txt

