#!/bin/bash
if [ $1 = "build"  ]; then
  echo "Setting environment variables"
  PATH=/home/amkhande/bin:/usr/local/bin:/usr/bin:/bin:/opt/bin:/usr/x86_64-pc-linux-gnu/gcc-bin/4.5.4:/usr/games/bin:/opt/cuda/bin:/opt/cuda/libnvvp:/home/amkhande/tbbSource/bin
  export PATH
  LD_LIBRARY_PATH=/home/amkhande/lib:/home/amkhande/tbbSource/lib/intel64/gcc4.4
  export LD_LIBRARY_PATH
  CPLUS_INCLUDE_PATH=/home/amkhande/tbbSource/include:/home/amkhande/include
  export CPLUS_INCLUDE_PATH
  C_INCLUDE_PATH=/home/amkhande/include
  export C_INCLUDE_PATH

  echo "Compiling matrixmult_tbb"
  g++ -O3 -std=c++0x MatrixMultiplication/matrixmult_tbb.cpp -L/home/amkhande/tbbSource/lib/intel64/gcc4.4 -ltbb -o MatrixMultiplication/matrixmult_tbb

  echo "Compiling jacobi_fd"
  g++ -O3 -std=c++0x JacobianIteration/jacobi_fd.cpp -L/home/amkhande/tbbSource/lib/intel64/gcc4.4 -ltbb -o JacobianIteration/jacobi_fd

  echo "Compiling cholesky"
  g++ -O3 -std=c++0x CholeskyFactorization/cholesky.cpp -L/home/amkhande/tbbSource/lib/intel64/gcc4.4 -ltbb -lrt -o CholeskyFactorization/cholesky
fi

if [ $1 = "clean" ]; then
  echo "Cleaning matrixmult_tbb"
  rm MatrixMultiplication/matrixmult_tbb
  echo "Cleaning jacobi_fd"
  rm JacobianIteration/jacobi_fd
  echo "Cleaning cholesky"
  rm CholeskyFactorization/cholesky
fi
