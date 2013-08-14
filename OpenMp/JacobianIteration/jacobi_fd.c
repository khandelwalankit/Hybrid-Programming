#include<stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <malloc.h>
#include <math.h>

static float *A,*B; //Float to store Matrix

static int xdim= 1024;
static int ydim= 1024;

void initialize_matrix()
{  
  unsigned i,j;
  float count=1.0;
  A=(float *) malloc(xdim*ydim*sizeof(float));
  B=(float *) malloc(xdim*ydim*sizeof(float));
  
  //Initialize matrix A
  for(i=0;i<xdim;i++)
  {
    for (j=0;j<ydim;j++)
    {
      A[i*ydim+j]=count++;
      B[i*ydim+j]=(float)0;
    }
  }
  /*for(i=0;i<xdim;i++)
  {
    for(j=0;j<ydim;j++)
    {
      B[i*ydim+j]=(float)(0);
     }
  }*/ 
}  

void reassign_matrix()
{
  unsigned i,j;
  for (i=0;i<xdim;i++)
  {
    for (j=0;j<ydim;j++)
      {
        A[i*ydim+j]=B[i*ydim+j];
      }
  }
}
void calculate_matrix_fd(int n)
{ 
  //Formula used : B[i,j]=(A[i-1,j]+A[i+1,j]+A[i,j-1]+A[i,j+1])/4 or center=(north+south+west+east)/4
  //east,west,south,north would be used to store the values of four sides of particular matrix value
  //north=A[(i-1)*ydim+j]
  //south=A[(i+1)*ydim+j]
  //west=A[i*ydim+(j-1)]
  //east=A[i*ydim+(j+1)]
  //Pivot values are taken as 0
  unsigned i,j,k;
  float east,west,north,south;
  for (k=0;k<n;k++)
  {
#pragma omp parallel for private(east,west,north,south) shared(A,B)
    for (i=0;i<xdim;i++)
    {
      for (j=0;j<ydim;j++)
       {
        if(i==0 && j!=0)
        {
          north=0;
          west=A[i*ydim+(j-1)];
          south=A[(i+1)*ydim+j];
          if(j==ydim-1)
          {
            east=0;
          }
          else
          {
            east=A[i*ydim+(j+1)];
          }
        }
        else if(i==0 && j==0)
        {
          north=0;
          west=0;
          east=A[i*ydim+(j+1)];
          south=A[(i+1)*ydim+j];
        }
        else if(i!=0 && j==0)
        {
          west=0;
          north=A[(i-1)*ydim+j];
          east=A[i*ydim+(j+1)];
          if(i==xdim-1)
          {
            south=0;
          }
          else
          {
            south=A[(i+1)*ydim+j];
          }
        }
        else if(i!=0 && j!=0)
        {
          north=A[(i-1)*ydim+j];
          west=A[i*ydim+(j-1)];
          if(i==xdim-1)
          {
            south=0;
          }
          else
          {
            south=A[(i+1)*ydim+j];
          }
          if(j==ydim-1)
          {
            east=0;
          }
          else
          {
            east=A[i*ydim+(j+1)];
          }
        }
        
        B[i*ydim+j]=(north+south+east+west)/4;
      }
    }
    reassign_matrix();
  }
}
void print_matrix(float *mat)
{
  unsigned i,j;
  for (i=0;i<xdim;i++)
  {
    for(j=0;j<ydim;j++)
    {
      printf("%f  ",mat[i*ydim+j]);
    }
    printf("\n");
  }
} 
int main(int argc, char** argv)
{
  double cpu_time_used;
  time_t start;
  int number_of_iteration=atoi(&(*argv[1]));
  xdim = atoi(&(*argv[2]));
  ydim = atoi(&(*argv[3]));
  start=time(0);
  initialize_matrix();
  //printf("Initial Matrix is\n");
  //print_matrix(A);
  calculate_matrix_fd(number_of_iteration);
  //printf("Average matrix is\n");
  print_matrix(B);
  free(A);
  free(B);
  cpu_time_used=difftime(time(0),start);
  printf("The Time used is %f\n",cpu_time_used); 
  return 0;
}

