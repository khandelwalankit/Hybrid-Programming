#!/bin/bash
echo "Setting environment variables"
PKG_CONFIG_PATH=/home/amkhande/lib/pkgconfig
export PKG_CONFIG_PATH
LD_LIBRARY_PATH=/home/amkhande/lib
export LD_LIBRARY_PATH
echo "Setting Work Stealing Strategy"
STARPU_SCHED=ws
export STARPU_SCHED
echo "Compiling MatrixMult"
gcc `pkg-config starpu-1.1 --cflags` matrixmult.c -fplugin=`pkg-config starpu-1.1 --variable=gccplugin` `pkg-config starpu-1.1 --libs` -o matrixmult
