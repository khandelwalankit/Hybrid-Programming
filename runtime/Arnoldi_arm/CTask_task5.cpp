#include "CTask.h"
using namespace std;
using namespace Tasks;
//tbb::tick_count CTask_task5::start       = tbb::tick_count();
//tbb::tick_count CTask_task5::end         = tbb::tick_count();
double CTask_task5::start   = starpu_timing_now();
double CTask_task5::end = starpu_timing_now();
double CTask_task5::cumulative_task_time = 0; 
void execute5(__attribute__((unused))void *descr[], void *arg)
{
  CTask_task5 *ct5 = static_cast<CTask_task5 *>(arg);
  task_map.erase(make_pair(5,ct5->iter_count));
  DoubleMat PARAM_tmp3$1(n,1);
  DoubleMat f$1(n,1);
  DoubleMat v$1(ct5->v$1_data);
  DoubleMat w$1(ct5->w$1_data);
  
  /*PARAM_tmp3$1()  =  v$1() * alpha$1 ;
  f$1()  =  w$1() - PARAM_tmp3$1() ;*/
  f$1()  =  w$1() - v$1() * ct5->alpha$1 ;
  ct5->j$1  =  2 ;
  ct5->PARAM_tmp4$1  =  ct5->j$1 <= ct5->k$0 ;
  ct5->task_version.reserve(8);
  ct5->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct5->j$1} ;
  GetTask<CTask_task6>(6,ct5->task_version)->Add_f$2(f$1);
  //root_dec_ref_count();
  //return  NULL;
}
