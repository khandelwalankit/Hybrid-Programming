#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <starpu.h>


static float *A, *B;
static starpu_data_handle_t B_handle;

static unsigned int parts = 2;
static unsigned xdim = 3;
static unsigned ydim = 3;
static int no_of_iteration=1;



static void cpu_jacobi(void *descr[], __attribute__((unused)) void *arg)
{ 
  float *subB;
  uint32_t nxB, nyB;
  uint32_t ldB;
  float east,west,north,south;
  uint32_t offset,dimension,offset_row_shift;
  subB = (float *)STARPU_MATRIX_GET_PTR(descr[0]);
    
  nxB = STARPU_MATRIX_GET_NX(descr[0]);
  nyB = STARPU_MATRIX_GET_NY(descr[0]);
  
  ldB = STARPU_MATRIX_GET_LD(descr[0]);

  unsigned i,j;
  unsigned int x,y;
  offset=STARPU_MATRIX_GET_OFFSET(descr[0]);
  //Based on offset we need to calculate the location of north,south,east and west in matrix A. 
  offset_row_shift=offset;
  for(i=0;i<nxB;i++)
  {
    for(j=0;j<nyB;j++)
    {
        dimension=offset/4;
        x=floor(dimension/ydim);
        y=dimension%ydim;
        if(x==0 && y!=0)
        {
          north=0;
          west=A[x*ydim+(y-1)];
          south=A[(x+1)*ydim+y];
          if(y==ydim-1)
          {
            east=0;
          }
          else
          {
            east=A[x*ydim+(y+1)];
          }
         }
        else if(x==0 && y==0)
        {
          north=0;
          west=0;
          east=A[x*ydim+(y+1)];
          south=A[(x+1)*ydim+y];
        }  
        else if(x!=0 && y==0)
        { 
          west=0;
          north=A[(x-1)*ydim+y];
          east=A[x*ydim+(y+1)];
          if(x==xdim-1)
          {
            south=0;
          }
          else
          { 
            south=A[(x+1)*ydim+y];
          }
         }
        else if(x!=0 && y!=0)
        { 
          north=A[(x-1)*ydim+y];
          west=A[x*ydim+(y-1)];
          if(x==xdim-1)
          { 
            south=0;
          }
          else
          {
            south=A[(x+1)*ydim+y];
          }  
          if(y==ydim-1)
          {
            east=0;
          }  
          else
          { 
            east=A[x*ydim+(y+1)];
          } 
        } 
        subB[i*ydim+j]=(north+south+east+west)/4;
        offset=offset+4;
     }
     offset=offset_row_shift+ydim*4;
     offset_row_shift=offset;
  }
}
static void initialize_matrix(void)
 { 
  unsigned i,j;
  float count=1;
  A=(float *) malloc(xdim*ydim*sizeof(float));
  B=(float *) malloc(xdim*ydim*sizeof(float));
  for (i=0;i<xdim;i++)
  {  
    for (j=0;j<ydim;j++)
    {
      A[i*ydim+j] = count++;
      B[i*ydim+j] = (float)0;
     }
    }
 } 
static void partition_matrix(void)
{ 
  starpu_matrix_data_register(&B_handle, 0, (uintptr_t)B, xdim,xdim,ydim, sizeof(float));
  //Here filter_func indicate how to partition the data and nchildren indicate chunk of data to be partitioned into
  struct starpu_data_filter vert =
  { 
    .filter_func = starpu_matrix_filter_vertical_block,
    .nchildren = parts
  };
  struct starpu_data_filter horiz =
  {   
    .filter_func = starpu_matrix_filter_block,
    .nchildren = parts
  };

  starpu_data_map_filters(B_handle,2,&vert,&horiz);
     
}
static struct starpu_perfmodel mult_perf_model = 
{ 
    .type = STARPU_HISTORY_BASED,
    .symbol = "mult_perf_model"
}; 

static struct starpu_codelet cl =
{  
    .cpu_funcs = {cpu_jacobi,NULL},
    .nbuffers = 1,
    .modes = {STARPU_W},
    .model =&mult_perf_model
};  

static int launch_tasks(void)
{
    int ret;
    unsigned partx,party;
    for (partx=0;partx<parts;partx++)
    {
      for (party=0;party<parts;party++)
      {
      struct starpu_task *task = starpu_task_create();
      task->cl=&cl;
      task->handles[0]=starpu_data_get_sub_data(B_handle, 2, partx,party);
      ret=starpu_task_submit(task);
      if (ret==-ENODEV) return ret;
          STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
      }
    }
    return 0;
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

int main(int argc, char **argv)
{   
  int ret;
  double cpu_time_used;
  time_t start;
  unsigned i;
  parts = atoi(&(*argv[1]));
  xdim = atoi(&(*argv[2]));
  ydim = atoi(&(*argv[3]));
  no_of_iteration= atoi(&(*argv[4]));
  start=time(0);
  ret = starpu_init(NULL); //To place different scheduling policies we need to pass value in form of struct starpu_conf *conf
  if (ret == -ENODEV)
    return 77;
  STARPU_CHECK_RETURN_VALUE(0,"starpu_init");
  initialize_matrix();
  //print_matrix(A);
  for(i=1;i<=no_of_iteration;i++)
  {  
    partition_matrix();
    ret=launch_tasks();
    if (ret==-ENODEV) goto enodev;
    starpu_task_wait_for_all();
    if (i != no_of_iteration)
        reassign_matrix();
  } 
  starpu_data_unpartition(B_handle,0);
  starpu_data_unregister(B_handle);
  print_matrix(B);
  free(A);
  free(B);
  starpu_shutdown();
  cpu_time_used=difftime(time(0),start);
  printf("Time Taken by program: %f seconds \n",cpu_time_used);
  return 0;
  enodev:
    starpu_shutdown();
    return 77;
}
