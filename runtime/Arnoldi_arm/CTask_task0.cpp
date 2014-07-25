#include "CTask.h"
using namespace std;
using namespace Tasks;
//tbb::tick_count CTask_task0::start       = tbb::tick_count();
//tbb::tick_count CTask_task0::end         = tbb::tick_count();
double CTask_task0::start       = starpu_timing_now();
double CTask_task0::end         = starpu_timing_now();
double CTask_task0::cumulative_task_time = 0; 
void execute0(__attribute__((unused))void *descr[], void *arg)
{
  CTask_task0 *ct0 = static_cast<CTask_task0 *>(arg);
  DoubleMat A$0(ct0->A$0_data);
  DoubleMat v$0(ct0->v$0_data);
  //set_ref_count(1);
  
  ct0->j$1  =  2 ;
  ct0->PARAM_tmp4$1  =  ct0->j$1 <= ct0->k$0 ;
  //iteration_vector task_version;
  ct0->task_version.reserve(8);
  ct0->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct0->j$1} ;
  GetTask<CTask_task2>(2,ct0->task_version)->Add_A$0(A$0);
  if(ct0->PARAM_tmp4$1){
    ct0->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct0->j$1} ;
    GetTask<CTask_task7>(7,ct0->task_version)->Add_A$0(A$0);
  }
  ct0->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct0->j$1} ;
  GetTask<CTask_task1>(1,ct0->task_version)->Add_v$0(v$0);
  ct0->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct0->j$1} ;
  GetTask<CTask_task2>(2,ct0->task_version)->Add_v$0(v$0);
  ct0->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct0->j$1} ;
  GetTask<CTask_task1>(1,ct0->task_version)->Add_k$0(ct0->k$0);
  ct0->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct0->j$1} ;
  GetTask<CTask_task2>(2,ct0->task_version)->Add_k$0(ct0->k$0);
  ct0->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct0->j$1} ;
  GetTask<CTask_task3>(3,ct0->task_version)->Add_k$0(ct0->k$0);
  ct0->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct0->j$1} ;
  GetTask<CTask_task4>(4,ct0->task_version)->Add_k$0(ct0->k$0);
  ct0->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct0->j$1} ;
  GetTask<CTask_task5>(5,ct0->task_version)->Add_k$0(ct0->k$0);
  ct0->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct0->j$1} ;
  GetTask<CTask_task6>(6,ct0->task_version)->Add_k$0(ct0->k$0);
  ct0->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct0->j$1} ;
  GetTask<CTask_task7>(7,ct0->task_version)->Add_k$0(ct0->k$0);
  ct0->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct0->j$1} ;
  GetTask<CTask_task8>(8,ct0->task_version)->Add_k$0(ct0->k$0);
  ct0->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct0->j$1} ;
  GetTask<CTask_task6>(6,ct0->task_version)->Add_j$2(ct0->j$1);
  ct0->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct0->j$1} ;
  GetTask<CTask_task7>(7,ct0->task_version)->Add_j$2(ct0->j$1);
  ct0->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct0->j$1} ;
  GetTask<CTask_task8>(8,ct0->task_version)->Add_j$2(ct0->j$1);
  starpu_task_wait_for_all();
  //return  NULL;
}
