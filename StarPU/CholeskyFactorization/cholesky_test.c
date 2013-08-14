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
#include "../../common/matrix_operations.h"

#define TAG(i, j, iter) ((starpu_tag_t)(((uint64_t)(iter)<<48)|((uint64_t)(j)<<24)|(i)))

static float *A; //For storing Matrix
static starpu_data_handle_t A_handle;

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

//This function would create a matrix A[1024][512] and B[512][1024]. For matrix storing AXB the size would be C[1024][1024]. Where C is initialized with 0 and A and B with random values.
static void initialize_system(void)
{ 
  unsigned i,j;
  A=(float *) malloc(xdim*ydim*sizeof(float));
  initialize_matrix<char,int,float>('c',xdim,ydim,1,A);
}   

//Cpu function
static void cholesky_first(void *descr[], __attribute__((unused)) void *arg)
{  
  float *subA;
  uint32_t nxA, nyA;
  uint32_t ldA;
  
  //The below function would return pointer to the block
  subA = (float *)STARPU_MATRIX_GET_PTR(descr[0]);

  //nx = return the number of elements in x -axis of the handle
  //ny = return the number of elements in y- axis of the handle
  nxA = STARPU_MATRIX_GET_NX(descr[0]);
  nyA = STARPU_MATRIX_GET_NY(descr[0]);
  
  //LD returns the number of elements in each row of matrix
  //It will be useful in calculating the offset to read and write in memory address
  ldA = STARPU_MATRIX_GET_LD(descr[0]);
  unsigned i,j,k;
  for(i=0;i<nyA;i++)
  { 
    //printf(" First element  %f",subA[i*ldA+i]);
    subA[i*ldA+i]=sqrt(subA[i*ldA+i]);
    for(j=i+1;j<nxA;j++)
    {
        subA[j*ldA+i]=(1.0/subA[i*ldA+i])*subA[j*ldA+i];
    }
    for(j=i+1;j<nxA;j++)
    {
        for(k=i+1;k<nyA;k++)
        {
            subA[j*ldA+k]=subA[j*ldA+k]-(subA[j*ldA+i]*subA[k*ldA+i]);
        }
    }
  }
  unsigned t;
  for(t=0;t<nxA;t++)
  {
    for(k=t+1;k<nyA;k++)
    {
        subA[t*ldA+k]=subA[k*ldA+t];
    }
  }
} 

static void cholesky_col(void *descr[], __attribute__((unused)) void *arg)
{  
  float *subA,*subA_R;
  uint32_t nxA, nyA,nxA_R,nyA_R;
  uint32_t ldA;
  
  //The below function would return pointer to the block
  subA = (float *)STARPU_MATRIX_GET_PTR(descr[0]);
  //Block for inversion
  subA_R = (float *)STARPU_MATRIX_GET_PTR(descr[1]); 
  //nx = return the number of elements in x -axis of the handle
  //ny = return the number of elements in y- axis of the handle
  nxA = STARPU_MATRIX_GET_NX(descr[0]);
  nyA = STARPU_MATRIX_GET_NY(descr[0]);
  //For inverting matrix
  nxA_R = STARPU_MATRIX_GET_NX(descr[1]);
  nyA_R = STARPU_MATRIX_GET_NY(descr[1]);

  ldA = STARPU_MATRIX_GET_LD(descr[0]);
  //printf("ldA is %d\n",ldA);
  unsigned i,j,k;
  //printf("This is Cholesky_Column\n");
  float *Inv; 
  Inv=(float *) malloc(nxA*nyA*sizeof(float));
  for(int i=0;i<nxA;i++)
  {
    for(int j=0;j<nyA;j++)
    {
        Inv[i*nyA+j]=subA_R[i*ldA+j];
    }
  }
  invert<int,float>(nxA,nyA,nyA,Inv);
  matrix_multiplycol<int,float>(0,0,nxA,nyA,ldA,subA,Inv);
  delete [] Inv;
} 

static void cholesky_tri(void *descr[], __attribute__((unused)) void *arg)
{  
  //subA_R2(for calculating transpose)
  float *subA,*subA_R1,*subA_R2;
  uint32_t nxA, nyA;
  uint32_t ldA;
  
  //The below function would return pointer to the block
  subA = (float *)STARPU_MATRIX_GET_PTR(descr[0]);
  subA_R1 = (float *)STARPU_MATRIX_GET_PTR(descr[1]);
  subA_R2 = (float *)STARPU_MATRIX_GET_PTR(descr[2]);

  //nx = return the number of elements in x -axis of the handle
  //ny = return the number of elements in y- axis of the handle
  nxA = STARPU_MATRIX_GET_NX(descr[0]);
  nyA = STARPU_MATRIX_GET_NY(descr[0]);
  
  //LD returns the number of elements in each row of matrix
  //It will be useful in calculating the offset to read and write in memory address
  ldA = STARPU_MATRIX_GET_LD(descr[0]);
  
  unsigned i,j,k;
  float **Tran,**mult_temp;
  Tran=new float*[nyA];
  mult_temp=new float*[nyA];
  for(int i=0;i<nyA;i++)
  {
    Tran[i]=new float[nyA];
    mult_temp[i]=new float[nyA];
  }
  calculate_transpose<int,float>(0,0,nxA,nyA,ldA,subA_R2,Tran);
  matrix_multiplytri<int,float>(0,0,nxA,nyA,ldA,subA_R1,Tran,mult_temp);
  matrix_subtract<int,float>(0,0,nxA,nyA,ldA,subA,mult_temp);
  //printf("This is Cholesky_Triangle\n");
  delete[] Tran;
  delete[] mult_temp;
}  

static struct starpu_perfmodel *mult_perf_model;
static struct starpu_codelet *ctrl_first_task;
static struct starpu_codelet *ctrl_column_task;
static struct starpu_codelet *ctrl_tri_task;

//Partiotion matrix
static void partition_matrix(void)
{ 
  starpu_matrix_data_register(&A_handle, 0, (uintptr_t)A, ydim,ydim,xdim, sizeof(float));
  
  //Here filter_func indicate how to partition the data and nchildren indicate chunk of data to be partitioned into
  struct starpu_data_filter *horiz,*vert;
  horiz = (struct starpu_data_filter *)malloc(sizeof(struct starpu_data_filter));
  horiz->filter_func={starpu_matrix_filter_block};
  horiz->nchildren=nslicey;
  vert=(struct starpu_data_filter *)malloc(sizeof(struct starpu_data_filter));
  vert->filter_func=starpu_matrix_filter_vertical_block;
  vert->nchildren=nslicex;
  starpu_data_map_filters(A_handle, 2, vert, horiz);
} 

static void express_deps0(int i,int j,int iter)
{
  if(iter>=1)
  {
    starpu_tag_declare_deps(TAG(i,j,iter),1,TAG(i,j,iter-1));
  }
}


static void express_deps1(int i,int j,int iter)
{
  if(iter>=1)
  {
    starpu_tag_declare_deps(TAG(i,j,iter),2,TAG(j,j,iter),TAG(i,j,iter-1));
  }
  else
  {
    starpu_tag_declare_deps(TAG(i,j,iter),1,TAG(j,j,iter));
  }
}

static void express_deps2(starpu_tag_t *tag_array,int ndep,int i,int j,int iter)
{
  starpu_tag_declare_deps_array(TAG(i,j,iter),ndep,tag_array);
}

//Below function is called to create task and to submit the task to cpu in this case 
static int launch_tasks(void)
{
  int ret;
  unsigned taskx,tasky;
  mult_perf_model=(struct starpu_perfmodel *)malloc(sizeof(struct starpu_perfmodel));
  mult_perf_model->type = STARPU_HISTORY_BASED;
  mult_perf_model->symbol = "mult_perf_model";
  for (tasky=0;tasky<nslicey;tasky++)
  {
    struct starpu_task *start_task = starpu_task_create();
    ctrl_first_task=(struct starpu_codelet *)malloc(sizeof(struct starpu_codelet));
    ctrl_first_task->cpu_funcs={cholesky_first,NULL};
    ctrl_first_task->nbuffers=1;
    ctrl_first_task->where=STARPU_CPU;
    ctrl_first_task->modes={STARPU_RW};
    ctrl_first_task->model=mult_perf_model;
    start_task->cl=ctrl_first_task;
    // starpu_data_get_sub_data returns handle. The function is defined in starpu_data_filter.h. This would create handle for each data partioned.
    start_task->handles[0]=starpu_data_get_sub_data(A_handle, 2, tasky, tasky);
    start_task->use_tag=1;
    start_task->tag_id=TAG(tasky,tasky,tasky);
    express_deps0(tasky,tasky,tasky);
    ret=starpu_task_submit(start_task);
    //starpu_task_wait(start_task);
    if (ret==-ENODEV) return ret;
    STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
    
    taskx=tasky;
    int number_of_col_tasks=nslicex-(taskx+1);
    struct starpu_task *col_task[number_of_col_tasks];
    starpu_tag_t col_tag_array[number_of_col_tasks];
    int col_task_count=0;
    for(taskx=tasky+1;taskx<nslicex;taskx++)
    {
      col_task[col_task_count]=starpu_task_create();
      ctrl_column_task=(struct starpu_codelet *)malloc(sizeof(struct starpu_codelet));
      ctrl_column_task->cpu_funcs = {cholesky_col,NULL};
      ctrl_column_task->where=STARPU_CPU;
      ctrl_column_task->nbuffers = 2;
      ctrl_column_task->modes = {STARPU_RW,STARPU_R};
      ctrl_column_task->model = mult_perf_model;
      col_task[col_task_count]->cl=ctrl_column_task;
      col_task[col_task_count]->handles[0]=starpu_data_get_sub_data(A_handle,2,taskx,tasky);
      col_task[col_task_count]->handles[1]=starpu_data_get_sub_data(A_handle,2,tasky,tasky);
      col_task[col_task_count]->use_tag=1;
      col_task[col_task_count]->tag_id=TAG(taskx,tasky,tasky);
      col_tag_array[col_task_count]=TAG(taskx,tasky,tasky);
      express_deps1(taskx,tasky,tasky);
      ret=starpu_task_submit(col_task[col_task_count]);
      if(ret==-ENODEV) return ret;
      STARPU_CHECK_RETURN_VALUE(ret,"starpu_task_submit");
      col_task_count++;
     }
    taskx=tasky;
    int number_of_tri_tasks=(nslicex-(taskx+1))*((nslicex-(taskx+1))+1)/2;
    struct starpu_task *tri_task[number_of_tri_tasks];
    int coltask=0;
    int tri_task_count=0;
    for(taskx=tasky+1;taskx<nslicex;taskx++)
    {
      for(coltask=tasky+1;coltask<=taskx;coltask++)
      {
        tri_task[tri_task_count]=starpu_task_create();
        ctrl_tri_task=(struct starpu_codelet *)malloc(sizeof(struct starpu_codelet));
        ctrl_tri_task->cpu_funcs = {cholesky_tri,NULL};
        ctrl_tri_task->nbuffers = 3;
        ctrl_tri_task->where=STARPU_CPU;
        ctrl_tri_task->modes = {STARPU_RW,STARPU_R,STARPU_R};
        ctrl_tri_task->model =mult_perf_model;
        tri_task[tri_task_count]->cl=ctrl_tri_task;
        tri_task[tri_task_count]->handles[0]=starpu_data_get_sub_data(A_handle,2,taskx,coltask);
        tri_task[tri_task_count]->handles[1]=starpu_data_get_sub_data(A_handle,2,taskx,tasky);
        tri_task[tri_task_count]->handles[2]=starpu_data_get_sub_data(A_handle,2,coltask,tasky);
        tri_task[tri_task_count]->use_tag=1;
        tri_task[tri_task_count]->tag_id=TAG(taskx,coltask,tasky);
        express_deps2(col_tag_array,col_task_count,taskx,coltask,tasky);
        ret=starpu_task_submit(tri_task[tri_task_count]);
        if(ret==-ENODEV) return ret;
        STARPU_CHECK_RETURN_VALUE(ret,"starpu_task_submit");
        tri_task_count++;

       }
    } 
    //starpu_task_wait_for_all();
  }
  return 0;
}  

int main(int argc, char **argv)
{ 
  
  int ret;
  double cpu_time_used;
  time_t start;
  int slicex,slicey;
  slicex = atoi(&(*argv[1]));
  slicey = atoi(&(*argv[2]));
  xdim = atoi(&(*argv[3]));
  ydim = atoi(&(*argv[4]));
  nslicex=xdim/slicex;
  nslicey=ydim/slicey;

  start=time(0);
  //initialize starpu. Here Null argument means that we are using default configuration for scheduling policies. 
  ret = starpu_init(NULL); //To place different scheduling policies we need to pass value in form of struct starpu_conf *conf
  if (ret == -ENODEV)
    return 77;
  
  STARPU_CHECK_RETURN_VALUE(0,"starpu_init");  // This is defined in starpu_util.h. If the ret value is not 0 then error message is printed

  //Initialize matrix
  initialize_system();
  //Partition Matrix
  partition_matrix();
  
  //submit tasks to multiply matrix according to partitioned data 
  ret=launch_tasks();
  if (ret==-ENODEV) goto enodev;
  starpu_task_wait_for_all();

  //After removing partition the subdata cannot be accessed
  starpu_data_unpartition(A_handle,0);
 
  //Removing handles from operation
  starpu_data_unregister(A_handle);

  //Free the memory taken by matrix
  free(A);
  
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

