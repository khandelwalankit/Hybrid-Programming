//Libraries included
#include <cstdio>
#include <math.h>
#include <stdlib.h>
#include <iostream>

//namespace used
using namespace std;

template <typename INT,typename VARY>
int cholesky_seq(INT xdim,INT ydim,VARY *A) 
{ 
 int i,j,k;
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
 for(i=0;i<xdim;i++)
 {
     for(j=i+1;j<ydim;j++)
     {
       A[i*ydim+j]=0;
     }
 }
 return 0;
}

template <typename INT,typename VARY>
INT cholesky_verification(INT ret,INT xdim,INT ydim, VARY *A,VARY *B)
 {
  for(int i=0;i<xdim;i++)
  {
    for(int j=i+1;j<ydim;j++)
    {
      A[i*ydim+j]=0;
     }
  } 
  if(ret==0)
  {
    matrix_subtract<int,double>(0,0,xdim,ydim,ydim,A,B);
  } 
  for(int i=0;i<xdim;i++)
  {
    for(int j=0;j<ydim;j++)
    {
      if(A[i*ydim+j]>0.002 || A[i*ydim+j]< -0.002)
      {
        ret=1;
      }

     }
  }
  return ret;
}
