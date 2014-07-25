#include "CTask.h"
using namespace std;
using namespace Tasks;
//tbb::tick_count CTask_task4::start       = tbb::tick_count();
//tbb::tick_count CTask_task4::end         = tbb::tick_count();
double CTask_task4::start   = starpu_timing_now();
double CTask_task4::end = starpu_timing_now();
double CTask_task4::cumulative_task_time = 0; 
void execute4(__attribute__((unused))void *descr[], void *arg)
{
  CTask_task4 *ct4 = static_cast<CTask_task4 *>(arg);
  task_map.erase(make_pair(4,ct4->iter_count));
  DoubleMat V$1(ct4->V$1_data);
  DoubleMat v$1(ct4->v$1_data);
  DoubleMat H$1(ct4->H$1_data);
  
  V$1()(span::all,1 - 1)  =  v$1() ;
  H$1()(1 - 1,1 - 1)  =  ct4->alpha$1 ;
  ct4->j$1  =  2 ;
  ct4->PARAM_tmp4$1  =  ct4->j$1 <= ct4->k$0 ;
  ct4->task_version.reserve(8);
  ct4->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct4->j$1} ;
  GetTask<CTask_task7>(7,ct4->task_version)->Add_V$1(V$1);
  if(!ct4->PARAM_tmp4$1){
    ct4->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct4->j$1} ;
    GetTask<CTask_END>(0,ct4->task_version)->Add_V$1(V$1);
  }
  ct4->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct4->j$1} ;
  GetTask<CTask_task6>(6,ct4->task_version)->Add_H$1(H$1);
  if(!ct4->PARAM_tmp4$1){
    ct4->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct4->j$1} ;
    GetTask<CTask_END>(0,ct4->task_version)->Add_H$1(H$1);
  }
  //root_dec_ref_count();
  //return  NULL;
}
