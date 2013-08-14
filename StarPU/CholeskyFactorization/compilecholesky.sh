#!/bin/bash
echo "Setting environment variables"
PKG_CONFIG_PATH=/home/amkhande/lib/pkgconfig
export PKG_CONFIG_PATH
LD_LIBRARY_PATH=/home/amkhande/lib:/home/amkhande/tbbSource/lib/intel64/gcc4.4
export LD_LIBRARY_PATH
PATH=/home/amkhande/bin:/usr/local/bin:/usr/bin:/bin:/opt/bin:/usr/x86_64-pc-linux-gnu/gcc-bin/4.5.4:/usr/games/bin:/opt/cuda/bin:/opt/cuda/libnvvp:/home/amkhande/tbbSource/bin
export PATH
echo "Setting Work Stealing Scheduling"
STARPU_SCHED=ws
export STARPU_SCHED
echo "Setting PATH for include file in C"
CPLUS_INCLUDE_PATH=/home/amkhande/tbbSource/include:/home/amkhande/include
export CPLUS_INCLUDE_PATH
C_INCLUDE_PATH=/home/amkhande/include
export C_INCLUDE_PATH

echo "removing earlier test file"
rm -i testresult_cholesky_block.txt
echo "Compiling Cholesky"
g++ -O3 `pkg-config starpu-1.1 --cflags` -std=c++0x cholesky_block.c  `pkg-config starpu-1.1 --libs` -o cholesky_block
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
    <<COMMENT echo "Running cholesky_block factorization for 4096X4096 and dividing in block of 16X16" >> testresult_cholesky_block.txt
    ./cholesky_block 16 16 4096 4096 >> testresult_cholesky_block.txt
COMMENT
  done

echo "Computation Completed for cholesky_block" >> testresult_cholesky_block.txt

