#include <iostream>
#include <malloc.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <tbb/task.h>
#include <tbb/tbb.h>
#include "../../common/matrix_operations.h"

using namespace tbb;
using namespace std;

static float *A,*B; //Float to store the matrix

//xdim, ydim,zdim specifies the dimension of the matrix
static int xdim= 1024;
static int ydim= 1024;
static int parts=8;
static int no_of_iteration=1;
static int slicex=1;
static int slicey=1;

void initialize_system()
{      
  A=(float *) malloc(xdim*ydim*sizeof(float));
  B=(float *) malloc(xdim*ydim*sizeof(float));
  //Below function is defined in matrix_operations.h
  initialize_matrix<char,int,float>('s',xdim,ydim,1,A);
  initialize_matrix<char,int,float>('n',xdim,ydim,1,B);
}


class Create_Task : public task 
{
  public:
  const int no_of_task;
  const int task_number;
  Create_Task():
    no_of_task(0),task_number(0)
  {}
  Create_Task(int task_n,int task_num):
    no_of_task(task_n),task_number(task_num)
  { 
  }
  task* execute(void)
  { 
    if (task_number!=0)
    {
      double time_used;
      time_t task_start;
      clock_t t0,t1;
      task_start=time(0);
      int t_n=task_number-1;
      int x=0;
      int y=0;
      int xtemp=0;
      unsigned i,j;
      int no_of_blocks=parts;
      float east,west,south,north;
      uint32_t offset,offset_row_shift;
   
      for(i=1;i<=no_of_blocks;i++)
      {
        if(task_number<=(no_of_blocks*i))
        {
          xtemp=(i-1);
          x=xtemp*slicex;
          //x=(i-1)*no_of_blocks;
          y=(t_n-(xtemp*no_of_blocks))*slicey;
          break;
         }
      }
      offset=x*ydim+y;
      float* subB=&B[offset];
      //cout<<"The Current Value of A is "<<A[offset]<<"\n";
      offset_row_shift=offset;
      for(i=0;i<slicex;i++)
      { 
        for(j=0;j<slicey;j++)
        {
          x=floor(offset/ydim);
          y=offset%ydim;
          if (x==0 && y!=0)
          {
            north=0;
            west=A[x*ydim+(y-1)];
            south=A[(x+1)*ydim+y];
            if(y==ydim-1)
            {
              east=0;
            }
            else
            {
              east=A[x*ydim+(y+1)];
            }
          }
          else if(x==0 && y==0)
          {
            north=0;
            west=0;
            east=A[x*ydim+(y+1)];
            south=A[(x+1)*ydim+y];
          }
          else if(x!=0 && y==0)
          {
            west=0;
            north=A[(x-1)*ydim+y];
            east=A[x*ydim+(y+1)];
            if(x==xdim-1)
            {
              south=0;
            }
            else
            {
              south=A[(x+1)*ydim+y];
            }
          }
          else if(x!=0 && y!=0)
          {
            north=A[(x-1)*ydim+y];
            west=A[x*ydim+(y-1)];
            if(x==xdim-1)
            {
              south=0;
            }
            else
            {
              south=A[(x+1)*ydim+y];
            }
            if(y==ydim-1)
            {
              east=0;
            }
            else
            {
              east=A[x*ydim+(y+1)];
            }
          }
          subB[i*ydim+j]=(north+south+east+west)/4;
          offset=offset+1;
        }
        offset=offset_row_shift+ydim;
        offset_row_shift=offset;
      } 
     time_used=difftime(time(0),task_start);
     cout<<"Time used for task is :"<<time_used<<"\n";
    }
    else
    {
        //Attempt with Task List
        task_list list1;
        for(int i=1;i<=no_of_task;i++)
        {
          list1.push_back(*new(allocate_child()) Create_Task(1,i));
        }
        set_ref_count(no_of_task+1);
        spawn_and_wait_for_all(list1);
        
    }
    return NULL;
  }

};

int main(int argc, char** argv)
{   
  double cpu_time_used;
  time_t start;
  slicex = atoi(&(*argv[1]));
  slicey = atoi(&(*argv[2]));
  xdim = atoi(&(*argv[3]));
  ydim = atoi(&(*argv[4]));
  no_of_iteration = atoi(&(*argv[5]));
  start=time(0);
  initialize_system();
  unsigned i,no_of_task,task_number;
  parts=xdim/slicex;
  no_of_task=(xdim*ydim)/(slicex*slicey);
  if(no_of_task==1)
  {
    task_number=1;
  }
  else
  {
    task_number=0;
  }
  for(i=1;i<=no_of_iteration;i++)
  {
    Create_Task& root=*new(task::allocate_root()) Create_Task(no_of_task,task_number);
    task::spawn_root_and_wait(root);
    if (i!=no_of_iteration)
      swap_matrix<float>(A,B);
  }
  //print_matrix<float,int>(B,xdim,ydim);
  free(A);
  free(B);
  cpu_time_used=difftime(time(0),start);
  cout << "The time used is " << cpu_time_used << " seconds.\n";
  return 0;
}  



