#include <iostream>
#include <malloc.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <tbb/task.h>
#include <tbb/tbb.h>

using namespace tbb;
using namespace std;

static float *A,*B,*C; //Float to store the matrix

//xdim, ydim,zdim specifies the dimension of the matrix
static int xdim= 1024;
static int ydim= 1024;
static int zdim= 1024;
static int slicex=128;
static int slicey=128;
/*
class Matrix_Multiply: public task
{
  public:
  const int task_number;
 // const Create_Task *root;
  Matrix_Multiply(int task_n):
    task_number(task_n)
  {}
  task* execute()  
  {
  // Pointer location based on task number
    float *subA, *subB, *subC;
    int rowoffset= slice*task_number;
    int columnoffset=slice*task_number;
    cout<<"Row Offset is :"<<rowoffset<<"\n";
    
  } 
};  

*/

void initialize_matrix()
{    
  unsigned i,j;
  A=(float *) malloc(xdim*zdim*sizeof(float));
  B=(float *) malloc(zdim*ydim*sizeof(float));
  C=(float *) malloc(xdim*ydim*sizeof(float));

  srand(2013);
  //printf("The value is :%f\n", drand48());
  //printf("The value is :%f\n", drand48());
  //printf("The value is :%f\n", drand48());
  float count=1;
  for (i=0;i< xdim;i++)
  { 
    for (j=0;j<zdim;j++)
     {
      A[i*zdim+j]=count++;

     }
  } 
  count=1;
  for (i=0;i<zdim;i++)
  { 
    for (j=0;j<ydim;j++)
     {
      B[i*ydim+j]=count++;
     }
  }
  //count =1; 
  for (i=0;i<xdim;i++)
  {   
    for(j=0;j<ydim;j++)
        {
      //C[i*ydim+j]=count++;
      C[i*ydim+j]=(float)(0);
    }
   }
  
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
        int t_n=task_number-1;
        int x=0;
        int y=0;
        int xtemp=0;
        unsigned i,j,k;
        int no_of_blocks=xdim/slicex;
        
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
        float* subA=&A[x*ydim];
        float* subB=&B[y];
        float* subC=&C[x*ydim+y]; 
        //cout<<"The Element positon is ["<<x<<"]["<<y<<"] and value is"<<subC[0]<<"\n";
        //cout<<"The Multiplication needed is "<<x<<"row of A and "<<y<<"column of B\n";
        //float* subC=(float *)C[x*ydim+y];
        //cout<<"The fields in Task Block "<<task_number<<"\n"; 
        for(i=0;i<slicex;i++)
        {
          for(j=0;j<slicey;j++)
          {
            //float sum=0.0;
           // cout<<"The element is"<<subC[i*ydim+j]<<"\n";
            for(k=0;k<zdim;k++)
            {
              subC[i*ydim+j]+=subA[k+i*zdim]*subB[j+k*zdim];
            }
          }
        }
       
    }
    else
    {
        //cout<<"Logic here"<<"\n";
         
        /*for(int i=1;i<=no_of_task;i++)
        {
          Create_Task& a=*new(allocate_child()) Create_Task(1,i);
          if(i<=no_of_task)
          {
            set_ref_count(no_of_task+1);
            spawn(a);  
          }
          else
          {
            cout<<"Ankit";
           // Create_Task& b=*new(allocate_child()) Create_Task(1,i);
            wait_for_all(a);
          }
        }*/
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
void print_matrix(float* mat)
{
  unsigned i,j;
  for(i=0;i<xdim;i++)
  {
    for(j=0;j<ydim;j++)
    {
      printf("%f ",mat[i*ydim+j]);
    }
    printf("\n");
  }
}
int main(int argc, char** argv)
  {   
  double cpu_time_used;
  time_t start;
  xdim = atoi(&(*argv[1]));
  ydim = atoi(&(*argv[2]));
  zdim = atoi(&(*argv[3]));
  slicex = atoi(&(*argv[4]));
  slicey = atoi(&(*argv[5]));
  start=time(0);
  initialize_matrix();
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
  Create_Task& root=*new(task::allocate_root()) Create_Task(no_of_task,task_number);
  /*for(i=1;i<no_of_task;i++)
  {
    Create_Task& a=*new(task::allocate_root()) Create_Task(i);
    task::spawn(a);
  }*/
  task::spawn_root_and_wait(root);
  //print_matrix(C);
  free(A);
  free(B);
  free(C);
  cpu_time_used=difftime(time(0),start);
  cout << "The time used is " << cpu_time_used << " seconds.\n";
  return 0;
}  



