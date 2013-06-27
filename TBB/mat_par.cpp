#include <iostream>
#include <malloc.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>

using namespace tbb;
using namespace std;

static float *A,*B,*C; //Float to store the matrix

//xdim, ydim,zdim specifies the dimension of the matrix
static int xdim= 1024;
static int ydim= 1024;
static int zdim= 1024;

class multiply_matrix
{
  public:
      void operator()(blocked_range<int> r) const
      {
        for (int i = r.begin(); i != r.end(); i++)
        {
          for (int j = 0; j < ydim; j++)
          {
            for (int k = 0; k < zdim; k++)
            {
              C[i*ydim+j] += A[k+i*zdim] * B[j+k*ydim];
            }
          }
        }
      }
};

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
  for (i=0;i< xdim;i++)
  { 
    for (j=0;j<zdim;j++)
     {
      A[i*zdim+j]=drand48();

    }
  }
  for (i=0;i<zdim;i++)
  { 
    for (j=0;j<ydim;j++)
     {
      B[i*ydim+j]=drand48();
     }
  } 
  for (i=0;i<xdim;i++)
  {  
    for(j=0;j<ydim;j++)
      {
      C[i*ydim+j]=(float)(0);
    }
  }
  
}


int main(int argc, char** argv)
 {   
  double cpu_time_used;
  time_t start;
  xdim = atoi(&(*argv[1]));
  ydim = atoi(&(*argv[2]));
  zdim = atoi(&(*argv[3]));
  start=time(0);
  initialize_matrix();
  //print_initmatrix(); 
  parallel_for(blocked_range<int>(0,xdim), multiply_matrix());
  //print_matrix();
  free(A);
  free(B);
  free(C);
  cpu_time_used=difftime(time(0),start);
  cout << "The time used is " << cpu_time_used << " seconds.\n";
  return 0;
}  



