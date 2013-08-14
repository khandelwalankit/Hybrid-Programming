//Libraries Used
#include <iostream>
#include <malloc.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <tbb/task.h>
#include <tbb/tbb.h>
#include "../../common/matrix_operations.h"

//Namespace used in program
using namespace tbb;
using namespace std;

static float *A,*B,*C; //Float to store the matrix

//xdim, ydim,zdim specifies the dimension of the matrix
static int xdim= 1024;
static int ydim= 1024;
static int zdim= 1024;
static int slicex=128;
static int slicey=128;

//Initialize the matrix
void initialize_system()
{    
  A=(float *) malloc(xdim*zdim*sizeof(float));
  B=(float *) malloc(zdim*ydim*sizeof(float));
  C=(float *) malloc(xdim*ydim*sizeof(float));
  //Below Function defined in matrix_operations.h
  //here we are considering xdim==ydim==zdim
  initialize_matrix<char,int,float>('r',xdim,ydim,2,A,B);
  initialize_matrix<char,int,float>('n',xdim,ydim,1,C);
}  

//Computation unit for Tasks created
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
        int t_n=task_number-1;
        int x=0;
        int y=0;
        int xtemp=0;
        unsigned i,j,k;
        int no_of_blocks=xdim/slicex;
        //Below loop to find the offset
        for(i=1;i<=no_of_blocks;i++)
        {
            if(task_number<=(no_of_blocks*i))
            {
              xtemp=(i-1);
              x=xtemp*slicex;
              y=(t_n-(xtemp*no_of_blocks))*slicey;
              break;
            }
        }
        float* subA=&A[x*ydim];
        float* subB=&B[y];
        float* subC=&C[x*ydim+y];
        //Below Loop performs the multiplication
        for(i=0;i<slicex;i++)
        { 
          for(j=0;j<slicey;j++)
           {
            for(k=0;k<zdim;k++)
            {
              subC[i*ydim+j]+=subA[k+i*zdim]*subB[j+k*zdim];
            }
          }
        }
       
     }
    else
    {
        //Create List Of Tasks
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
  zdim = atoi(&(*argv[5]));
  start=time(0);
  initialize_system();
  unsigned i,no_of_task,task_number;

  no_of_task=(xdim*ydim)/(slicex*slicey);
  if(no_of_task==1)
  {
    task_number=1;
  }
  else
  {
    task_number=0;
  } 
  //Create Main Task which would inturn create child tasks
  Create_Task& root=*new(task::allocate_root()) Create_Task(no_of_task,task_number);
  task::spawn_root_and_wait(root);

  //print_matrix<float,int>(C,xdim,ydim);
  free(A);
  free(B);
  free(C);

  //Compute time utilized
  cpu_time_used=difftime(time(0),start);
  cout << "The time used is " << cpu_time_used << " seconds.\n";
  return 0;
}  



