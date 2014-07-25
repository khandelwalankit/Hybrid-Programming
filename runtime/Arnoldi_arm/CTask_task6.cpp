#include "CTask.h"
using namespace std;
using namespace Tasks;
//tbb::tick_count CTask_task6::start       = tbb::tick_count();
//tbb::tick_count CTask_task6::end         = tbb::tick_count();
double CTask_task6::start   = starpu_timing_now();
double CTask_task6::end = starpu_timing_now();
double CTask_task6::cumulative_task_time = 0; 
void execute6(__attribute__((unused))void *descr[], void *arg)
{
  CTask_task6 *ct6 = static_cast<CTask_task6 *>(arg);  
  task_map.erase(make_pair(6,ct6->iter_count));
  DoubleMat v$3(n,1);
  DoubleMat f$2(ct6->f$2_data);
  DoubleMat H$1(ct6->H$1_data);
  ct6->beta$1  =  norm(f$2(),2) ;
  v$3()  =  f$2() / ct6->beta$1 ;
  ct6->PARAM_tmp5$1  =  ct6->j$2 - 1 ;
  H$1()(ct6->j$2 - 1,ct6->PARAM_tmp5$1 - 1)  =  ct6->beta$1 ;
  ct6->j$3  =  ct6->j$2 + 1 ;
  ct6->PARAM_tmp4$3  =  ct6->j$3 <= ct6->k$0 ;
  ct6->task_version.reserve(8);
  ct6->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct6->j$2} ;
  GetTask<CTask_task7>(7,ct6->task_version)->Add_v$3(v$3);
  ct6->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct6->j$2} ;
  GetTask<CTask_task8>(8,ct6->task_version)->Add_H$1(H$1);
  if(ct6->PARAM_tmp4$3){
    ct6->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct6->j$3} ;
    GetTask<CTask_task6>(6,ct6->task_version)->Add_j$2(ct6->j$3);
  }
  if(ct6->PARAM_tmp4$3){
    ct6->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct6->j$3} ;
    GetTask<CTask_task6>(6,ct6->task_version)->Add_k$0(ct6->k$0);
  }
  //root_dec_ref_count();
  //return  NULL;
}
