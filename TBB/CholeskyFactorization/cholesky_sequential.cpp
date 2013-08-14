//Libraries included
#include <cstdio>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include "../../common/matrix_operations.h"

//namespace used
using namespace std;


/*Define matrix and related variables*/
static float *A;
static int xdim=1024;
static int ydim=1024;

//Initialize matrix
void initialize_system()
{    
  A=(float *) malloc(xdim*ydim*sizeof(float));
  initialize_matrix<char,int,float>('c',xdim,ydim,1,A); /*Function specified in matrix_operations.h*/
}  
   
int main(int argc, char** argv) 
 {
 double cpu_time_used;
 time_t start_time;
 int i,j,k;
 xdim=atoi(&(*argv[1]));
 ydim=atoi(&(*argv[2]));
 start_time=time(0); 
 //Initializing System
 initialize_system();
 
 for(i=0;i<ydim;i++)
 { 
   A[i*ydim+i]=sqrt(A[i*ydim+i]);
   for(j=i+1;j<xdim;j++)
   {
     A[j*ydim+i]=(1.0/A[i*ydim+i])*A[j*ydim+i];
   }
   for(j=i+1;j<xdim;j++)
    {
     for(k=i+1;k<ydim;k++)
      {
        A[j*ydim+k]=A[j*ydim+k]-(A[j*ydim+i]*A[k*ydim+i]);
     }
   }
 }
 //To verify uncomment below statement
 /*for(i=0;i<xdim;i++)
 {
     for(j=i+1;j<ydim;j++)
     {
       A[i*ydim+j]=0;
     }
 }*/
 cout<<"output"<<"\n";
 //Function specified in matrix_operations.h
 print_matrix<float,int>(A,xdim,ydim);
 //Time Taken
 free(A);
 cpu_time_used=difftime(time(0),start_time);
 cout << "The time used is " << cpu_time_used << " seconds.\n";
 return 0;
}
