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

//#define TBB_USE_DEBUG
//namespace used
using namespace std;
using namespace tbb;
using namespace tbb::flow;

/*Type Definition of nodes to create the dependency graph */
typedef continue_node<continue_msg> create_node;
typedef broadcast_node<continue_msg> start_node;

/*Define matrix and related variables*/
static float *A;
static int xdim=1024;
static int ydim=1024;
static int slicex=128;
static int slicey=128;

//Initialize matrix
void initialize_system()
{  
  unsigned i,j;
  A=(float *) malloc(xdim*ydim*sizeof(float));
  //Function specified in matrix_operations.h
  initialize_matrix<char,int,float>('c',xdim,ydim,1,A);
}  
   
//Computation Unit- Cholesky
class cholesky
{  
  int caller_func,flag,off,localx,localy;
  public:
    cholesky( int cf,int f,int offset,int i, int j) : caller_func(cf),flag(f),off(offset),localx(i),localy(j) {}
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
        //printf("First(%x)\n",this);
        int i=localy;
        A[i*ydim+i]=sqrt(A[i*ydim+i]);
        int temp=(i+1)%slicey;
        int tempblock=floor(i/slicey);
        if(temp>0)
        {
            for(int j=i+1;j<(tempblock+1)*slicex;j++)
            {
                A[j*ydim+i]=(1.0/A[i*ydim+i])*A[j*ydim+i];
            }
         }
    } 
    void cholesky_col() const
    {
        int tempcol=off%slicey;
        int i=localy+tempcol;
        for(int j=localx;j<(localx+slicex);j++)
        {
          A[j*ydim+i]=(1.0/A[i*ydim+i])*A[j*ydim+i];
        }
        //printf("Block[%d][%d](%x)\n",localx, localy,this);
  
    }
    void cholesky_tri() const
     { 
        if(flag==1)
        {
            int tempcol=off%slicey+1;
            int t=localy+tempcol;
            int k;
            if(off==localx)
            {
                k=t;
              } 
            else
             {
                k=localx;
             } 
            for(int j=k;j<localx+slicex;j++)
            {
                for(int i=t;i<(localy+slicey);i++)
                 {
                    A[j*ydim+i]=A[j*ydim+i]-(A[j*ydim+off]*A[i*ydim+off]);
                 }
              }

        } 
        else
        {
            //cout<<"off "<<off<<" localx "<<localx<<" localy "<<localy<<"\n";
            for(int j=localx;j<localx+slicex;j++)
             {
                for(int i=localy;i<localy+slicey;i++)
                {
                    A[j*ydim+i]=A[j*ydim+i]-(A[j*ydim+off]*A[i*ydim+off]);
                }
            }
         }
    }
};

int main(int argc, char** argv) 
{   
 double cpu_time_used;
 time_t starting_time;
 int i,j,k,partx,party;
 slicex=atoi(&(*argv[1]));
 slicey=atoi(&(*argv[2]));
 xdim=atoi(&(*argv[3]));
 ydim=atoi(&(*argv[4]));
 starting_time=time(0); 
 //Initializing System
 initialize_system();
 
 //Number of parts the matrix would be divided into horizontally = partx and vertically = party
 partx=xdim/slicex;
 party=ydim/slicey;
 int total_blocks=partx*party;
 int count=0;
 //cout<<"Total Blocks are"<<total_blocks<<"\n";
 graph g;
 for(i=0;i<ydim;i++)
 { 
   start_node start(g);
   create_node *chol_first;
   //create_node *chol_tri[i][total_blocks];
   count++;
   chol_first=new create_node(g,cholesky(1,0,0,i,i));
   make_edge(start,*chol_first);
   int tempblocki=floor(i/slicey);
   int no_of_nodes=partx-(tempblocki+1);
   create_node *chol_col[no_of_nodes];
   for(j=tempblocki+1;j<partx;j++)
   { 
     chol_col[i]=new create_node(g,cholesky(2,0,i,j*slicex,tempblocki*slicey));
     make_edge(*chol_first,*chol_col[i]);
   }
   (start).try_put(continue_msg());
   (g).wait_for_all();
   //delete chol_first;
   start_node start1(g);
   create_node *chol_again;
   chol_again=new create_node(g,cholesky(4,-1,-1,-1,-1));
   make_edge(start1,*chol_again);

   int tempblockj=floor((i+1)/slicex);
   if((count%slicey)!=0)
   {
     int temp=floor(i/slicex);
     int tmpnode=partx-(temp+1);
     no_of_nodes=(partx-tempblockj)+((tmpnode)*(tmpnode+1))/2;
   }
   else
   { 
     int temp=floor(i/slicex);
     int tmpnode=partx-(temp+1);
     no_of_nodes=((tmpnode)*(tmpnode+1))/2;
    }
   create_node *chol_tri[no_of_nodes];
   int count_nodes=0;
   //cout<<"The value of i is"<<i<<"\n";
   if((count%slicey)!=0)
   {  
    for(j=tempblockj;j<partx;j++)
    {  
        chol_tri[count_nodes]=new create_node(g,cholesky(3,1,i,j*slicex,tempblocki*slicey));
        make_edge(*chol_again,*chol_tri[count_nodes]);
        count_nodes++;
    } 
   } 
   tempblockj=floor(i/slicex);
   //cout<<"Numberof nodes:"<<no_of_nodes<<"\n";
   for(j=tempblockj+1;j<partx;j++)
   {
     for(k=tempblockj+1;k<j+1;k++)
     { 
       chol_tri[count_nodes]=new create_node(g,cholesky(3,2,i,j*slicex,k*slicey));
       make_edge(*chol_again,*chol_tri[count_nodes]);
       count_nodes++;
     } 
   }  

   //cout<<"The value of i is"<<i<<"\n";
   (start1).try_put(continue_msg());
   (g).wait_for_all(); 
   //delete chol_again;
   //delete g;
  }
 
 //To verify uncomment below statement
 for(i=0;i<xdim;i++)
 {
     for(j=i+1;j<ydim;j++)
     {
       A[i*ydim+j]=0;
     }
 }
 cout<<"output"<<"\n";
 //Function from matrix_operations.h
 print_matrix<float,int>(A,xdim,ydim);
 //Time Taken
 free(A);
 cpu_time_used=difftime(time(0),starting_time);
 cout << "The time used is " << cpu_time_used << " seconds.\n";
 return 0;
}
