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
static float *A,*L;
static int xdim=1024;
static int ydim=1024;
static int slicex=128;
static int slicey=128;

//Initialize matrix
void initialize_system()
{    
  A=(float *) malloc(xdim*ydim*sizeof(float));
  L=(float *) malloc(xdim*ydim*sizeof(float));
  //Below function defined in matrix_operations.h
  initialize_matrix<char,int,float>('c',xdim,ydim,1,A);
  initialize_matrix<char,int,float>('n',xdim,ydim,1,L);
} 

//Computation Unit- Cholesky
class cholesky
{  
  int localx,localy;
  public:
    cholesky( int i, int j) : localx(i),localy(j) {}
     void operator()( continue_msg ) const {
      for(int m=localx;m<(localx+slicex);m++)
       {
        for(int n=localy;n<(m+1);n++)
         {
          float s=0;
          for(int k=0;k<n;k++)
           {
            s+=L[m*ydim+k]*L[n*ydim+k];
          }
          L[m * ydim + n] = (m == n)?sqrt(A[m * ydim + m]-s):((1.0 / L[n * ydim + n]) * (A[m * ydim + n] - s));
         }
       }

     }
 };

int main(int argc, char** argv) { 
 double cpu_time_used;
 time_t start_time;
 int i,j,partx,party;
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

 //Total number of Slicex X SliceY blocks
 int total_blocks=partx*party;
 
 // Number of graph nodes to be created :
 int total_nodes=(partx*(partx+1))/2;
 graph *g=new graph();
 start_node start(*g);
 create_node *compute_chol[total_nodes];
 int x=0;
 int y=0;
 int tempy=y;
 for(i=0;i<total_nodes;i++)
 {   
    compute_chol[i]=new create_node(*g,cholesky(y*slicex,x*slicey));
    //The below computation is to pass approriate offset 
    if(y==(party-1))
    { 
      x++;
      y=tempy+1;
      tempy=y;
    }
    else
    {
      y++;
    } 
 
 }
 int count=0;
 //tempcount will maintain vertical dependencies
 int tempcount=0;
 //temp2count will maintain horizontal dependencies
 int temp2count=partx;
 //Below loops would create the appropriate dependencies between the nodes
 for(i=0;i<partx;i++)
 {
   for(j=i;j<party;j++)
   {
     if(i==0 && j==0)
     {
        make_edge(start,*compute_chol[count]);
        tempcount=count;
       }
     else if(i==j && i!=0)
     {   
        make_edge(*compute_chol[tempcount+1],*compute_chol[count]);
        tempcount=count;
     } 
     else
     { 
       make_edge(*compute_chol[tempcount],*compute_chol[count]);
       if(i!=0)
       {
         make_edge(*compute_chol[count-temp2count],*compute_chol[count]);
       }
     } 
     count++;
    } 
   temp2count--;
 }   
 start.try_put(continue_msg());
 (*g).wait_for_all();
 delete g;
 //To verify uncomment below statement
 cout<<"output"<<"\n";
 //This funmction is in matrix_operations.h
 print_matrix<float,int>(L,xdim,ydim);
 
 //Time Taken
 cpu_time_used=difftime(time(0),start_time);
 cout << "The time used is " << cpu_time_used << " seconds.\n";
 return 0;
}
