#include "CTask.h"
using namespace std;
using namespace Tasks;
//tbb::tick_count CTask_task8::start       = tbb::tick_count();
//tbb::tick_count CTask_task8::end         = tbb::tick_count();
double CTask_task8::start   = starpu_timing_now();
double CTask_task8::end = starpu_timing_now();
double CTask_task8::cumulative_task_time = 0; 
void execute8(__attribute__((unused))void *descr[], void *arg)
{
  CTask_task8 *ct8 = static_cast<CTask_task8 *>(arg);
  task_map.erase(make_pair(8,ct8->iter_count));
  DoubleMat PARAM_tmp7$1(n,1);
  DoubleMat f$3(n,1);
  DoubleMat V$1(ct8->V$1_data);
  DoubleMat h$1(ct8->h$1_data);
  DoubleMat w$2(ct8->w$2_data);
  DoubleMat H$1(ct8->H$1_data);
  
  /*PARAM_tmp7$1()  =  V$1()(span::all,span(1 - 1,ct8->j$2 - 1)) * h$1() ;
  f$3()  =  w$2() - PARAM_tmp7$1() ;*/
  f$3()  =  w$2() - V$1()(span::all,span(1 - 1,ct8->j$2 - 1)) * h$1() ;
  H$1()(span(1 - 1,ct8->j$2 - 1),ct8->j$2 - 1)  =  h$1() ;
  ct8->j$3  =  ct8->j$2 + 1 ;
  ct8->PARAM_tmp4$3  =  ct8->j$3 <= ct8->k$0 ;
  ct8->task_version.reserve(8);
  if(ct8->PARAM_tmp4$3){
    ct8->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct8->j$3} ;
    GetTask<CTask_task6>(6,ct8->task_version)->Add_f$2(f$3);
  }
  if(!ct8->PARAM_tmp4$3){
    ct8->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct8->j$3} ;
    GetTask<CTask_END>(0,ct8->task_version)->Add_f$2(f$3);
  }
  if(ct8->PARAM_tmp4$3){
    ct8->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct8->j$3} ;
    GetTask<CTask_task6>(6,ct8->task_version)->Add_H$1(H$1);
  }
  if(!ct8->PARAM_tmp4$3){
    ct8->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct8->j$3} ;
    GetTask<CTask_END>(0,ct8->task_version)->Add_H$1(H$1);
  }
  if(ct8->PARAM_tmp4$3){
    ct8->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct8->j$3} ;
    GetTask<CTask_task8>(8,ct8->task_version)->Add_j$2(ct8->j$3);
  }
  if(ct8->PARAM_tmp4$3){
    ct8->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct8->j$3} ;
    GetTask<CTask_task8>(8,ct8->task_version)->Add_k$0(ct8->k$0);
  }
  //root_dec_ref_count();
  //return  NULL;
}
