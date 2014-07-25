#include "CTask.h"
using namespace std;
using namespace Tasks;
//tbb::tick_count CTask_task1::start       = tbb::tick_count();
//tbb::tick_count CTask_task1::end         = tbb::tick_count();
double CTask_task1::start   = starpu_timing_now();
double CTask_task1::end = starpu_timing_now();
double CTask_task1::cumulative_task_time = 0; 
void execute1(__attribute__((unused))void *descr[], void *arg)
{
  CTask_task1 *ct1 = static_cast<CTask_task1 *>(arg);
  task_map.erase(make_pair(1,ct1->iter_count));
  DoubleMat H$1(k,k);
  DoubleMat V$1(n,k);
  DoubleMat v$0(ct1->v$0_data);
  
  ct1->n$1  =  v$0().n_elem ;
  H$1().zeros();
  V$1().zeros();
  ct1->PARAM_tmp1$1  =  norm(v$0(),2) ;
  ct1->j$1  =  2 ;
  ct1->PARAM_tmp4$1  =  ct1->j$1 <= ct1->k$0 ;
  ct1->task_version.reserve(8);
  ct1->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct1->j$1} ;
  GetTask<CTask_task4>(4,ct1->task_version)->Add_H$1(H$1);
  ct1->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct1->j$1} ;
  GetTask<CTask_task4>(4,ct1->task_version)->Add_V$1(V$1);
  ct1->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct1->j$1} ;
  GetTask<CTask_task2>(2,ct1->task_version)->Add_PARAM_tmp1$1(ct1->PARAM_tmp1$1);
  //root_dec_ref_count();
  //return  NULL;
}
