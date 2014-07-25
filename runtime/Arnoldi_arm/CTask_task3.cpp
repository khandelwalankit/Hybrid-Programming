#include "CTask.h"
using namespace std;
using namespace Tasks;
//tbb::tick_count CTask_task3::start       = tbb::tick_count();
//tbb::tick_count CTask_task3::end         = tbb::tick_count();
double CTask_task3::start   = starpu_timing_now();
double CTask_task3::end = starpu_timing_now();
double CTask_task3::cumulative_task_time = 0; 
void execute3(__attribute__((unused))void *descr[], void *arg)
{
  CTask_task3 *ct3 = static_cast<CTask_task3 *>(arg);
  task_map.erase(make_pair(3,ct3->iter_count));
  DoubleMat PARAM_tmp2$1(1,n);
  DoubleMat v$1(ct3->v$1_data);
  DoubleMat w$1(ct3->w$1_data);
  
  /*PARAM_tmp2$1()  =  trans(v$1()) ;
  ct3->alpha$1  =  as_scalar(PARAM_tmp2$1() * w$1()) ;*/
  ct3->alpha$1  =  as_scalar(trans(v$1()) * w$1()) ;
  ct3->j$1  =  2 ;
  ct3->PARAM_tmp4$1  =  ct3->j$1 <= ct3->k$0 ;
  ct3->task_version.reserve(8);
  ct3->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct3->j$1} ;
  GetTask<CTask_task4>(4,ct3->task_version)->Add_alpha$1(ct3->alpha$1);
  ct3->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct3->j$1} ;
  GetTask<CTask_task5>(5,ct3->task_version)->Add_alpha$1(ct3->alpha$1);
  //root_dec_ref_count();
  //return  NULL;
}
