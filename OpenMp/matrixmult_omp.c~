#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <malloc.h>
#include <math.h>
static float *A,*B,*C; //Float to store the matrix

//xdim, ydim,zdim specifies the dimension of the matrix
static int xdim= 2048;
static int ydim= 2048;
static int zdim= 2048;

void initialize_matrix()
 { 
  unsigned i,j;
  float count=1;
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
  for (i=0;i<xdim;i++)
  {  
    for(j=0;j<ydim;j++)
      {
      C[i*ydim+j]=(float)(0);
    }
  }
} 

void multiply_matrix()
{  
  unsigned i,j,k;
  int ldA=xdim;
  int ldB=zdim;
  int ldC=xdim;
  #pragma omp parallel for shared(A,B,C)
  for (i=0;i<xdim;i++)
    {
    for (j=0;j<ydim;j++)
      {
      for (k=0;k<zdim;k++)
       {
         C[i*ydim+j] += A[k+i*zdim] * B[j+k*ydim];
       }
    }
  }
} 

void print_matrix()
{ 
  unsigned i,j;
  for (i=0;i<xdim;i++)
   {
    for(j=0;j<ydim;j++)
     {
      printf("%f  ",C[i*ydim+j]);
      
    }
    printf("\n");
  }
} 

void print_initmatrix()
{  
  unsigned i,j;
  for (i=0;i<xdim;i++)
   {
    for(j=0;j<zdim;j++)
    {
      printf("%f  ",A[i*zdim+j]);
     }
    printf("\n");
  } 
  for (i=0;i<zdim;i++)
  {  
    for(j=0;j<ydim;j++)
    {
      printf("%f  ",B[i*ydim+j]);
      
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
  start=time(0);
  initialize_matrix();
  //print_initmatrix(); 
  multiply_matrix();
  //print_matrix();
  free(A);
  free(B);
  free(C);
  cpu_time_used=difftime(time(0),start);
  printf("The time used is %f\n",cpu_time_used);
  return 0;
} 


