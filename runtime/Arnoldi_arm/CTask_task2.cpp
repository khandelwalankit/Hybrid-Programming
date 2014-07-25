#include "CTask.h"
using namespace std;
using namespace Tasks;
//tbb::tick_count CTask_task2::start       = tbb::tick_count();
//tbb::tick_count CTask_task2::end         = tbb::tick_count();
double CTask_task2::start   = starpu_timing_now();
double CTask_task2::end = starpu_timing_now();
double CTask_task2::cumulative_task_time = 0; 
void execute2(__attribute__((unused))void *descr[],void *arg)
{
  CTask_task2 *ct2 = static_cast<CTask_task2 *>(arg); 
  task_map.erase(make_pair(2,ct2->iter_count));
  DoubleMat v$1(n,1);
  DoubleMat w$1(n,1);
  DoubleMat v$0(ct2->v$0_data);
  DoubleMat A$0(ct2->A$0_data);
  
  v$1()  =  v$0() / ct2->PARAM_tmp1$1 ;
  w$1()  =  A$0() * v$1() ;
  ct2->j$1  =  2 ;
  ct2->PARAM_tmp4$1  =  ct2->j$1 <= ct2->k$0 ;
  ct2->task_version.reserve(8);
  ct2->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct2->j$1} ;
  GetTask<CTask_task3>(3,ct2->task_version)->Add_v$1(v$1);
  ct2->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct2->j$1} ;
  GetTask<CTask_task4>(4,ct2->task_version)->Add_v$1(v$1);
  ct2->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct2->j$1} ;
  GetTask<CTask_task5>(5,ct2->task_version)->Add_v$1(v$1);
  ct2->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct2->j$1} ;
  GetTask<CTask_task3>(3,ct2->task_version)->Add_w$1(w$1);
  ct2->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct2->j$1} ;
  GetTask<CTask_task5>(5,ct2->task_version)->Add_w$1(w$1);
  //root_dec_ref_count();
  //return  NULL;
}
