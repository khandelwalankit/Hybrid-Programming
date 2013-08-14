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
#include "../../common/matrix_operations.h"
#include <omp.h>

float *A; //For storing Matrix


static unsigned int nslicex = 4;
static unsigned int nslicey = 4;
int xdim = 1024;
int ydim = 1024;
int slicex = 256;
int slicey = 256;


//This function would create a matrix A[1024][512] and B[512][1024]. For matrix storing AXB the size would be C[1024][1024]. Where C is initialized with 0 and A and B with random values.
static void initialize_system(void)
{  
  unsigned i,j;
  A=(float *) malloc(xdim*ydim*sizeof(float));
  initialize_matrix<char,int,float>('c',xdim,ydim,1,A);
}    

//Cpu function
static void cholesky_first(int block_x,int block_y)
{   
  int localx=block_x*slicex;
  int localy=block_y*slicey;
  for(int i=localy;i<(localy+slicey);i++)
  {  
    A[i*ydim+i]=sqrt(A[i*ydim+i]);
    for(int j=i+1;j<(localx+slicex);j++)
    { 
        A[j*ydim+i]=(1.0/A[i*ydim+i])*A[j*ydim+i];
     }
    for(int j=i+1;j<(localx+slicex);j++)
    {
        for(int k=i+1;k<(localy+slicey);k++)
        {
            A[j*ydim+k]=A[j*ydim+k]-(A[j*ydim+i]*A[k*ydim+i]);
         }
    }  
  } 
  for(int t=(localx);t<(localx+slicex);t++)
  {
    for(int k=t+1;k<(localy + slicey);k++)
    {
        A[t*ydim+k]=A[k*ydim+t];
    } 
  }  
}    

static void cholesky_col(int block_x,int block_y)
{    
  int localx=(block_x)*slicex;
  int localy=(block_y)*slicey;
  float *Inv;
  Inv=(float *)malloc (slicex*slicey*sizeof(float));
  int m=0;
  for(int i=localy;i<(localy+slicex);i++)
  {
    int n=0;
    for(int j=localy;j<(localy+slicey);j++)
    {
      Inv[m*slicey+n]=A[i*ydim+j];
      n++;
    }
    m++;
  }
  invert<int,float>(slicex,slicey,slicey,Inv);
  matrix_multiplycol<int,float>(localx,localy,slicex,slicey,ydim,A,Inv);  
  delete [] Inv;
}    

static void cholesky_tri(int block,int block_x,int block_y)
{    
  int localx=(block_x)*slicex;
  int localy=(block_y)*slicey;
  int t_localx=(block)*slicex;
  int t_localy=(block)*slicey;
  float **Tran,**mult_temp;
  Tran=new float*[slicey];
  mult_temp=new float*[slicey];
  for(int i=0;i<slicey;i++)
  {
    Tran[i]=new float[slicey];
    mult_temp[i]=new float[slicey];
  }
  calculate_transpose<int,float>(localy,t_localy,slicex,slicey,ydim,A,Tran);
  matrix_multiplytri<int,float>(localx,t_localy,slicex,slicey,ydim,A,Tran,mult_temp);
  matrix_subtract<int,float>(localx,localy,slicex,slicey,ydim,A,mult_temp);
  delete[] Tran;
  delete[] mult_temp;
}  



//Below function is called to create task and to submit the task to cpu in this case 
static int launch_tasks(void)
 {  
  int ret;
  unsigned taskx,tasky;

  for (tasky=0;tasky<nslicey;tasky++)
  { 
    #pragma omp task shared(A,slicex,slicey,tasky) 
        cholesky_first(tasky,tasky);
    //#pragma omp taskwait
    #pragma omp parallel
    {
      taskx=tasky;
      int number_of_col_tasks=nslicex-(taskx+1);
      int col_task_count=0;
      #pragma omp for
      for(taskx=tasky+1;taskx<nslicex;taskx++)
      { 
        #pragma omp task shared(A,slicex,slicey)
        cholesky_col(taskx,tasky);
      }
      //#pragma omp taskwait
      
      taskx=tasky;
      int number_of_tri_tasks=(nslicex-(taskx+1))*((nslicex-(taskx+1))+1)/2;
      int coltask=0;
      //printf("Task executed is %d\n",tasky);
      #pragma omp for
      for(taskx=tasky+1;taskx<nslicex;taskx++)
      {
        for(coltask=tasky+1;coltask<=taskx;coltask++)
        { 
          #pragma omp task shared(A,slicex,slicey)
          cholesky_tri(tasky,taskx,coltask);
        }
      }
   }
   //#pragma omp taskwait
  } 
  return 0;
}  

int main(int argc, char **argv)
{  
  int ret;
  double cpu_time_used;
  time_t start;
  slicex = atoi(&(*argv[1]));
  slicey = atoi(&(*argv[2]));
  xdim = atoi(&(*argv[3]));
  ydim = atoi(&(*argv[4]));
  nslicex=xdim/slicex;
  nslicey=ydim/slicey;

  start=time(0);
  //initialize starpu. Here Null argument means that we are using default configuration for scheduling policies. 
  //Initialize matrix
  initialize_system();
  
  //omp_set_dynamic(0);
  omp_set_num_threads(8);

  ret=launch_tasks();

  print_matrix<float,int>(A,xdim,ydim);
  //Free the memory taken by matrix
  free(A);
  
  //cpu_time_used = ((double) (end - start))/(CLOCKS_PER_SEC);
  cpu_time_used=difftime(time(0),start);
  //cpu_time_used = ((double)(end-start))/8*CLOCKS;
  printf("Time Taken by program: %f seconds \n",cpu_time_used);
  return 0;
}  


