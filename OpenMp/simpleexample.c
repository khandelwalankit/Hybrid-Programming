#include <stdio.h>
#include "omp.h"
int main()
{
  int shared_var=10;
  int tid;
  omp_set_num_threads(5);
#pragma omp parallel private(tid)
  {
    tid=omp_get_num_threads();
    int private_var=omp_get_thread_num();
    //printf("Shared Memory Usage of value %d Private Memory Usage of Value %d\n",shared_var,private_var);
    printf("%d\n",tid);
  }
}
