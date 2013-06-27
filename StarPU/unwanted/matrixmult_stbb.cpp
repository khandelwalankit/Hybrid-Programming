/* StarPU --- Runtime system for heterogeneous multicore architectures. 
 *
 * Copyright (C) 2012  INRIA
 *
 * StarPU is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 *
 * StarPU is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU Lesser General Public License in COPYING.LGPL for more details.
 */
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <starpu.h>

static float *A, *B, *C; //For storing Matrix
static starpu_data_handle_t A_handle, B_handle, C_handle;

/*---------------------------------------------------------------------------------------------------------------
 *      |   Rows    |   Columns |   Elements in block       |
 *  A   | 1024      | 512       |   A[row]/nslicex          |A is partitioned horizontally into number of nslicex. 4 blocks with each block of 256*512
 *  B   | 512       | 1024      |   B[col]/nslicey          |B is partitioned vertically into number of nslicey. 4 blocks with each block of 512*256
 *  C   | 1024      | 1024      |   [C[row]/nslicex]nslicey |C is partitioned to contain 16 blocks of 256*256 elements
 */ 

static unsigned int nslicex = 4;
static unsigned int nslicey = 4;
static unsigned xdim = 1024;
static unsigned ydim = 1024;
static unsigned zdim = 1024;


//This function would create a matrix A[1024][512] and B[512][1024]. For matrix storing AXB the size would be C[1024][1024]. Where C is initialized with 0 and A and B with random values.
static void initialize_matrix(void)
{
  unsigned i,j;
  A=(float *) malloc(zdim*ydim*sizeof(float));
  B=(float *) malloc(xdim*zdim*sizeof(float));
  C=(float *) malloc(xdim*ydim*sizeof(float));
  srand(2013);
  for (j=0;j<ydim;j++)
  {  
    for (i=0;i<zdim;i++)
     {
      A[j+i*ydim] = (float)(starpu_drand48());
     }
  }
  for (j=0;j<zdim;j++)
  {  
    for (i=0;i<xdim;i++)
     {
      B[j+i*zdim] = (float)(starpu_drand48());
     }
  } 
  for (j=0;j<ydim;j++)
  {  
    for (i=0;i<xdim;i++)
    {
      C[j+i*ydim]=(float)(0);
    }
  } 
}   

//Cpu function
static void cpu_mult(void *descr[], __attribute__((unused)) void *arg)
{  
  float *subA, *subB, *subC;
  uint32_t nxC, nyC, nyA;
  uint32_t ldA, ldB, ldC;
  
  //The below function would return pointer to the block
  subA = (float *)STARPU_MATRIX_GET_PTR(descr[0]);
  subB = (float *)STARPU_MATRIX_GET_PTR(descr[1]);
  subC = (float *)STARPU_MATRIX_GET_PTR(descr[2]);

  //nx = return the number of elements in x -axis of the handle
  //ny = return the number of elements in y- axis of the handle
  nxC = STARPU_MATRIX_GET_NX(descr[2]);
  nyC = STARPU_MATRIX_GET_NY(descr[2]);
  nyA = STARPU_MATRIX_GET_NY(descr[0]);
  
  //LD returns the number of elements in each row of matrix
  //It will be useful in calculating the offset to read and write in memory address
  ldA = STARPU_MATRIX_GET_LD(descr[0]);
  ldB = STARPU_MATRIX_GET_LD(descr[1]);
  ldC = STARPU_MATRIX_GET_LD(descr[2]);
  
  unsigned i,j,k;
  float sum;
  #pragma omp parallel for private(sum) shared(subA,subB,subC)
  for (i=0;i<nyC;i++)
  {
    for(j=0;j<nxC;j++)
     {
      sum=0.0;
      for (k=0;k<nyA;k++)
      {
        sum += subA[j+k*ldA]*subB[k+i*ldB];
      }
      subC[j+i*ldC]=sum;
    }
  } 
} 

static struct starpu_perfmodel mult_perf_model = 
{
  .type = STARPU_HISTORY_BASED,
  .symbol = "mult_perf_model"
}; 

static struct starpu_codelet cl =
{ 
  .cpu_funcs = {cpu_mult,NULL},
  .nbuffers = 3,
  .modes = {STARPU_R,STARPU_R,STARPU_W},
  .model =&mult_perf_model
};

//Partiotion matrix
static void partition_matrix(void)
{ 
  //The starpu_data_register function are used to assign the handles the address pf the matrix, Here 0 in second argument indicates data given in 3rd argument  exists in main memory. Last argument specifies the size of each element which is helpful in setting offset while dividing data. Here leading dimension i.e. 4th argument is in column major ordering and hence width is provided. 
  //Calculating offset CM: Column*Total Number of rows +Rows [for first handle row is given by ydim and column by zdim]
  //The below function is defined in starpu_data_interfaces.h in include folder
  starpu_matrix_data_register(&A_handle, 0, (uintptr_t)A, ydim,ydim,zdim, sizeof(float));
  starpu_matrix_data_register(&B_handle, 0, (uintptr_t)B, zdim,zdim,xdim, sizeof(float));
  starpu_matrix_data_register(&C_handle, 0, (uintptr_t)C, ydim,ydim,xdim, sizeof(float));
  
  //Partition data using filter. Filters are interfaces to indicate how the data has to be partitioned. 
  //Here filter_func indicate how to partition the data and nchildren indicate chunk of data to be partitioned into
  struct starpu_data_filter vert =
  {
    .filter_func = starpu_matrix_filter_vertical_block,
    .nchildren = nslicex
     };
  struct starpu_data_filter horiz =
  {    
    .filter_func = starpu_matrix_filter_block,
    .nchildren = nslicey
  };

  //In matrix multiplication here we multiply the A[row] * B[column] which gives below partition
  starpu_data_partition(B_handle, &vert);
  starpu_data_partition(A_handle, &horiz);
  //column major partition for C
  starpu_data_map_filters(C_handle, 2, &vert, &horiz);
}  

//Below function is called to create task and to submit the task to cpu in this case 
//C[taskx, tasky] = A[tasky] B[taskx]
static int launch_tasks(void)
 {
  int ret;
  unsigned taskx,tasky;
  for (taskx=0;taskx<nslicex;taskx++)
    {
    for (tasky=0; tasky<nslicey; tasky++)
      {
      struct starpu_task *task = starpu_task_create();
      task->cl=&cl;
      // starpu_data_get_sub_data returns handle. The function is defined in starpu_data_filter.h. This would create handle for each data partioned.
      task->handles[0]=starpu_data_get_sub_data(A_handle, 1, tasky);
      task->handles[1]=starpu_data_get_sub_data(B_handle, 1, taskx);
      task->handles[2]=starpu_data_get_sub_data(C_handle, 2, taskx, tasky);
      ret=starpu_task_submit(task);
      if (ret==-ENODEV) return ret;
      STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
     }
  } 
  return 0;
}  

int main(int argc, char **argv)
{ 
  
  int ret;
  double cpu_time_used;
  time_t start;
  nslicex = atoi(&(*argv[1]));
  nslicey = atoi(&(*argv[2]));
  xdim = atoi(&(*argv[3]));
  ydim = atoi(&(*argv[4]));
  zdim = atoi(&(*argv[5]));
  start=time(0);
  //initialize starpu. Here Null argument means that we are using default configuration for scheduling policies. 
  ret = starpu_init(NULL); //To place different scheduling policies we need to pass value in form of struct starpu_conf *conf
  if (ret == -ENODEV)
    return 77;
  
  STARPU_CHECK_RETURN_VALUE(0,"starpu_init");  // This is defined in starpu_util.h. If the ret value is not 0 then error message is printed

  //Initialize matrix
  initialize_matrix();
  
  //Partition Matrix
  partition_matrix();
  
  //submit tasks to multiply matrix according to partitioned data 
  ret=launch_tasks();
  if (ret==-ENODEV) goto enodev;
  starpu_task_wait_for_all();

  //After removing partition the subdata cannot be accessed
  starpu_data_unpartition(A_handle,0);
  starpu_data_unpartition(B_handle,0);
  starpu_data_unpartition(C_handle,0);
 
  //Removing handles from operation
  starpu_data_unregister(A_handle);
  starpu_data_unregister(B_handle);
  starpu_data_unregister(C_handle);

  //Free the memory taken by matrix
  free(A);
  free(B);
  free(C);
  
  //starpu operation completion
  starpu_shutdown();
  //cpu_time_used = ((double) (end - start))/(CLOCKS_PER_SEC);
  cpu_time_used=difftime(time(0),start);
  //cpu_time_used = ((double)(end-start))/8*CLOCKS;
  printf("Time Taken by program: %f seconds \n",cpu_time_used);
  return 0;

enodev:
  starpu_shutdown();
  return 77;
} 

