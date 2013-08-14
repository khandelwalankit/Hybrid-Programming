#include<starpu.h>


static __global__ void matrix_mult()
{
  int i= blockIdx.x*blockDim.x+threadIdx.x;

}

static struct starpu_codelet cl=
{
  .cuda_funcs= {cuda_matrix,NULL},

};
