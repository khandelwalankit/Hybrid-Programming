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
rm -i testresult_jacobi.txt
echo "Compiling Jacobian Iteration"
gcc `pkg-config starpu-1.1 --cflags` jacobi_fd.c `pkg-config starpu-1.1 --libs` -o jacobi_fd
echo "Computation Started for Jacobi" >> testresult_jacobi.txt
max=8
for i in `seq 1 $max`
  do
    echo "Executing test :$i times"
    echo "Executing test :$i times" >> testresult_jacobi.txt
    echo "Running Jacobi Iteration for 1024X1024 and dividing in block of 512X512" >> testresult_jacobi.txt
    ./jacobi_fd 2 1024 1024 1024 >> testresult_jacobi.txt
    echo "Running Jacobi Iteration for 1024X1024 and dividing in block of 256X256" >> testresult_jacobi.txt
   ./jacobi_fd 4 1024 1024 1024 >> testresult_jacobi.txt
    echo "Running Jacobi Iteration for 1024X1024 and dividing in block of 128X128" >> testresult_jacobi.txt
    ./jacobi_fd 8 1024 1024 1024 >> testresult_jacobi.txt
    echo "Running Jacobi Iteration for 1024X1024 and dividing in block of 64X64" >> testresult_jacobi.txt
    ./jacobi_fd 16 1024 1024 1024 >> testresult_jacobi.txt
    echo "Running Jacobi Iteration for 1024X1024 and dividing in block of 32X32" >> testresult_jacobi.txt
    ./jacobi_fd 32 1024 1024 1024 >> testresult_jacobi.txt
    echo "Running Jacobi Iteration for 1024X1024 and dividing in block of 16X16" >> testresult_jacobi.txt
    ./jacobi_fd 64 1024 1024 1024 >> testresult_jacobi.txt
    echo "Running Jacobi Iteration for 2048X2048 and dividing in block of 512X512" >> testresult_jacobi.txt
    ./jacobi_fd 4 2048 2048 2048 >> testresult_jacobi.txt
    echo "Running Jacobi Iteration for 2048X2048 and dividing in block of 256X256" >> testresult_jacobi.txt
    ./jacobi_fd 8 2048 2048 2048 >> testresult_jacobi.txt
    echo "Running Jacobi Iteration for 2048X2048 and dividing in block of 128X128" >> testresult_jacobi.txt
    ./jacobi_fd 16 2048 2048 2048 >> testresult_jacobi.txt
    echo "Running Jacobi Iteration for 2048X2048 and dividing in block of 64X64" >> testresult_jacobi.txt
    ./jacobi_fd 32 2048 2048 2048 >> testresult_jacobi.txt
    echo "Running Jacobi Iteration for 2048X2048 and dividing in block of 32X32" >> testresult_jacobi.txt
    ./jacobi_fd 64 2048 2048 2048 >> testresult_jacobi.txt
    <<COMMENT1
    echo "Running Jacobi Iteration for 2048X2048 and dividing in block of 16X16" >> testresult_jacobi.txt
    ./jacobi_fd 128 2048 2048 2048 >> testresult_jacobi.txt
COMMENT1
    echo "Running Jacobi Iteration for 4096X4096 and dividing in block of 512X512" >> testresult_jacobi.txt
    ./jacobi_fd 8 4096 4096 4096 >> testresult_jacobi.txt
    echo "Running Jacobi Iteration for 4096X4096 and dividing in block of 256X256" >> testresult_jacobi.txt
    ./jacobi_fd 16 4096 4096 4096 >> testresult_jacobi.txt
    echo "Running Jacobi Iteration for 4096X4096 and dividing in block of 128X128" >> testresult_jacobi.txt
    ./jacobi_fd 32 4096 4096 4096 >> testresult_jacobi.txt
    echo "Running Jacobi Iteration for 4096X4096 and dividing in block of 64X64" >> testresult_jacobi.txt
    ./jacobi_fd 64 4096 4096 4096 >> testresult_jacobi.txt
    <<COMMENT2
    echo "Running Jacobi Iteration for 4096X4096 and dividing in block of 32X32" >> testresult_jacobi.txt
    ./jacobi_fd 128 4096 4096 4096 >> testresult_jacobi.txt
    echo "Running Jacobi Iteration for 4096X4096 and dividing in block of 16X16" >> testresult_jacobi.txt
    ./jacobi_fd 256 4096 4096 4096 >> testresult_jacobi.txt
COMMENT2
  done

echo "Computation Completed for jacobi_fd" >> testresult_jacobi.txt

