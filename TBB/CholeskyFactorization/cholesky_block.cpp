//Libraries included
#include <cstdio>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <tbb/tbb.h>
#include <tbb/task.h>
#include "tbb/flow_graph.h"
#include "../../common/matrix_operations.h"

//namespace used
using namespace std;
using namespace tbb;
using namespace tbb::flow;

/*Type Definition of nodes to create the dependency graph */
typedef continue_node<continue_msg> create_node;
typedef broadcast_node<continue_msg> start_node;

/*Define matrix and related variables*/
static float *A,*U;
static int xdim=1024;
static int ydim=1024;
static int slicex=128;
static int slicey=128;

//Initialize System by initilaizing matrix
void initialize_system()
{  
 A=(float *) malloc(xdim*ydim*sizeof(float));
 U=(float *) malloc(xdim*ydim*sizeof(float));
 //This function is present in matrix_operations.h
 //'c' indicates initialize A with cholesky values
 //'n' indicates initialize U with 0 values
 //'1' here indicates one argument needs to be initilaized
 initialize_matrix<char,int,float>('c',xdim,ydim,1,A);
 initialize_matrix<char,int,float>('n',xdim,ydim,1,U);
}
   
//Computation Unit- Cholesky
class cholesky 
{    
  int caller_func,block,block_x,block_y;
  public:
    cholesky( int cf, int bl,int blx,int bly) : caller_func(cf),block(bl),block_x(blx),block_y(bly) {}
      void operator()( continue_msg ) const {
        if(caller_func==1)
        {
          this->cholesky_first();
        }
        else if(caller_func==2)
        {
          this->cholesky_col();
        }
        else if(caller_func==3)
        {
          this->cholesky_tri();
        }
        else
        {
          //Dummy Counter - Do Nothing
        } 

       }
 
  void cholesky_first() const
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
            //cout<<"Value of A["<<j<<"]["<<k<<"]is :"<<A[j*ydim+k]<<"\n";
          }
        }
      }
      for(int t=(localx);t<(localx + slicex);t++)
       {
        for(int k=t+1;k<(localy + slicey);k++)
         {
          A[t*ydim+k]=A[k*ydim+t];
        }
      }
  } 
  void cholesky_col() const
  { 
      int localx=block_x*slicex;
      int localy=block_y*slicey;
      //Computation A[r,k]=A[r*k]*(A[k,k]^(-1))
      //Calculate inverse of block
      float *Inv; 
      Inv=(float *) malloc(slicex*slicey*sizeof(float));
      //calculate_inverse<int,float>(localy,slicex,slicey,ydim,A,Inv);
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
      //Calculate matrix multiplication
      matrix_multiplycol<int,float>(localx,localy,slicex,slicey,ydim,A,Inv);  
      delete [] Inv;

  }
  void cholesky_tri() const
  {
      int localx=block_x*slicex;
      int localy=block_y*slicey;
      int t_localx=block*slicex;
      int t_localy=block*slicey;
      float **Tran,**mult_temp;
      Tran=new float*[slicey];
      mult_temp=new float*[slicey];
      for(int i=0;i<slicey;i++)
      {
        Tran[i]=new float[slicey];
        mult_temp[i]=new float[slicey];
      }
      //Calculate Transpose of matrix
      calculate_transpose<int,float>(localy,t_localy,slicex,slicey,ydim,A,Tran);
      /*printf("The Transpose is :\n");
      for(int i=0;i<slicex;i++)
      {
        for(int j=0;j<slicey;j++)
        {
          printf(" %f ",Tran[i][j]);
        }
        printf("\n");
      }*/
      //Multiply A[r][k]*Tran
      matrix_multiplytri<int,float>(localx,t_localy,slicex,slicey,ydim,A,Tran,mult_temp);
      matrix_subtract<int,float>(localx,localy,slicex,slicey,ydim,A,mult_temp);
      delete[] Tran;
      delete[] mult_temp;
  }   
};

int main(int argc, char** argv) 
{   
 double cpu_time_used;
 time_t start_time;
 int i,j,k,partx,party;
 slicex=atoi(&(*argv[1]));
 slicey=atoi(&(*argv[2]));
 xdim=atoi(&(*argv[3]));
 ydim=atoi(&(*argv[4]));
 start_time=time(0); 
 //Initializing System
 initialize_system();
 
 //Number of parts the matrix would be divided into horizontally = partx and vertically = party
 partx=xdim/slicex;
 party=ydim/slicey;
 int total_blocks=partx*party;
 int count=0;
// cout<<"Total Blocks are"<<total_blocks<<"\n";
 //graph *g=new graph();
 graph g;
 int bl_row=0;
 int bl_col=0;
// cout<<"input"<<"\n";
// print_matrix(A,xdim,ydim);
 for(i=0;i<party;i++)
 {
   bl_col=i;
   bl_row=bl_col;
   //Computation -Node 1st Step= A[i,i]=chol(A[i,i]);
   //Check if we need to do only when bl_row==bl_col??
   //cout<<"Value of block ["<<bl_row<<"]["<<bl_col<<"]\n";
   //chol_first=new create_node(g,cholesky(1,bl_row,bl_col));
    
   start_node start(g);
   create_node *chol_first;
   chol_first=new create_node(g,cholesky(1,-1,bl_row,bl_col));
   make_edge(start,*chol_first);

   //Computation - 2nd Step 
   //A[r,i]=A[r,i]*(A[i,i]^(-1))
   int num_of_col_nodes=partx-bl_row;
   if(partx>1)
   {
     create_node *chol_col[num_of_col_nodes];
     int count=0;
     for(int r=bl_row+1;r<partx;r++)
     {
         chol_col[count]=new create_node(g,cholesky(2,-1,r,bl_col));
         make_edge(*chol_first,*chol_col[count]);
         count++;
     }
     (start).try_put(continue_msg());
     (g).wait_for_all();
     start_node start1(g);
     int number_of_triangle_nodes=(party-bl_col)*((party-bl_col)+1)/2;
     create_node *chol_tri[number_of_triangle_nodes];
     count=0;
     for(int r=bl_row+1;r<partx;r++)
     {
       for(int c=bl_col+1;c<=r;c++)
       {
        //Computation for triangle 
         chol_tri[count]=new create_node(g,cholesky(3,bl_row,r,c));
         make_edge(start1,*chol_tri[count]);
       }
     }
     start1.try_put(continue_msg());
     g.wait_for_all();
   }
   
 }

 //delete g;
  
  //To verify uncomment below statement
 /*for(i=0;i<xdim;i++) 
 {
     for(j=i+1;j<ydim;j++)
      {
       A[i*ydim+j]=0;
     }
 }*/
 //cout<<"output"<<"\n";
 //print_matrix<float,int>(A,xdim,ydim);
 /*for(i=0;i<xdim;i++)
 {
   for(j=0;j<ydim;j++)
   {
     U[j*ydim+i]=A[i*ydim+j];
   }
 }
 //Multiply Matrix to verify
 for(i=0;i<xdim;i++)
 {
   for(j=0;j<ydim;j++)
   {
     float sum=0;
     for(k=0;k<ydim;k++)
     {
        sum+=A[i*ydim+k]*U[k*ydim+j];
     }
     A[i*ydim+j]=sum;
   }
 }
 cout<<"verification"<<"\n";
 print_matrix(A,xdim,ydim);*/
 //Time Taken
 free(A);
 cpu_time_used=difftime(time(0),start_time);
 cout << "The time used is " << cpu_time_used << " seconds.\n";
 return 0;
}
