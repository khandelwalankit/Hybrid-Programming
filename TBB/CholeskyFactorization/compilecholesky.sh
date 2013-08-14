#!/bin/bash
echo "removing earlier test file"
rm -i testresult_cholesky_block.txt
echo "Compiling Cholesky"
g++ -O3 -std=c++0x cholesky_block.cpp -L/home/amkhande/tbbSource/lib/intel64/gcc4.4 -ltbb -lrt -o cholesky_block
echo "Computation Started for cholesky_block" >> testresult_cholesky_block.txt
max=8
for i in `seq 1 $max`
  do
    echo "Executing test :$i times"
    echo "Executing test :$i times" >> testresult_cholesky_block.txt
    echo "Running cholesky_block factorization for 1024X1024 and dividing in block of 512X512" >> testresult_cholesky_block.txt
    ./cholesky_block 512 512 1024 1024 >> testresult_cholesky_block.txt
    echo "Running cholesky_block factorization for 1024X1024 and dividing in block of 256X256" >> testresult_cholesky_block.txt
    ./cholesky_block 256 256 1024 1024 >> testresult_cholesky_block.txt
    echo "Running cholesky_block factorization for 1024X1024 and dividing in block of 128X128" >> testresult_cholesky_block.txt
    ./cholesky_block 128 128 1024 1024 >> testresult_cholesky_block.txt
    echo "Running cholesky_block factorization for 1024X1024 and dividing in block of 64X64" >> testresult_cholesky_block.txt
    ./cholesky_block 64 64 1024 1024 >> testresult_cholesky_block.txt
    echo "Running cholesky_block factorization for 1024X1024 and dividing in block of 32X32" >> testresult_cholesky_block.txt
    ./cholesky_block 32 32 1024 1024 >> testresult_cholesky_block.txt
    echo "Running cholesky_block factorization for 1024X1024 and dividing in block of 16X16" >> testresult_cholesky_block.txt
    ./cholesky_block 16 16 1024 1024 >> testresult_cholesky_block.txt
    echo "Running cholesky_block factorization for 2048X2048 and dividing in block of 512X512" >> testresult_cholesky_block.txt
    ./cholesky_block 512 512 2048 2048 >> testresult_cholesky_block.txt
    echo "Running cholesky_block factorization for 2048X2048 and dividing in block of 256X256" >> testresult_cholesky_block.txt
    ./cholesky_block 256 256 2048 2048 >> testresult_cholesky_block.txt
    echo "Running cholesky_block factorization for 2048X2048 and dividing in block of 128X128" >> testresult_cholesky_block.txt
    ./cholesky_block 128 128 2048 2048 >> testresult_cholesky_block.txt
    echo "Running cholesky_block factorization for 2048X2048 and dividing in block of 64X64" >> testresult_cholesky_block.txt
    ./cholesky_block 64 64 2048 2048 >> testresult_cholesky_block.txt
    echo "Running cholesky_block factorization for 2048X2048 and dividing in block of 32X32" >> testresult_cholesky_block.txt
    ./cholesky_block 32 32 2048 2048 >> testresult_cholesky_block.txt
    echo "Running cholesky_block factorization for 2048X2048 and dividing in block of 16X16" >> testresult_cholesky_block.txt
    ./cholesky_block 16 16 2048 2048 >> testresult_cholesky_block.txt
    echo "Running cholesky_block factorization for 4096X4096 and dividing in block of 512X512" >> testresult_cholesky_block.txt
    ./cholesky_block 512 512 4096 4096 >> testresult_cholesky_block.txt
    echo "Running cholesky_block factorization for 4096X4096 and dividing in block of 256X256" >> testresult_cholesky_block.txt
    ./cholesky_block 256 256 4096 4096 >> testresult_cholesky_block.txt
    echo "Running cholesky_block factorization for 4096X4096 and dividing in block of 128X128" >> testresult_cholesky_block.txt
    ./cholesky_block 128 128 4096 4096 >> testresult_cholesky_block.txt
    echo "Running cholesky_block factorization for 4096X4096 and dividing in block of 64X64" >> testresult_cholesky_block.txt
    ./cholesky_block 64 64 4096 4096 >> testresult_cholesky_block.txt
    echo "Running cholesky_block factorization for 4096X4096 and dividing in block of 32X32" >> testresult_cholesky_block.txt
    ./cholesky_block 32 32 4096 4096 >> testresult_cholesky_block.txt
    echo "Running cholesky_block factorization for 4096X4096 and dividing in block of 16X16" >> testresult_cholesky_block.txt
    ./cholesky_block 16 16 4096 4096 >> testresult_cholesky_block.txt
  done

echo "Computation Completed for cholesky_block" >> testresult_cholesky_block.txt
