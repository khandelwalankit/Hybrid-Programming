#include "CTask.h"
using namespace std;
using namespace Tasks;
//tbb::tick_count CTask_task7::start       = tbb::tick_count();
//tbb::tick_count CTask_task7::end         = tbb::tick_count();
double CTask_task7::start   = starpu_timing_now();
double CTask_task7::end = starpu_timing_now();
double CTask_task7::cumulative_task_time = 0; 
void execute7(__attribute__((unused))void *descr[], void *arg)
{
  CTask_task7 *ct7 = static_cast<CTask_task7 *>(arg);
  task_map.erase(make_pair(7,ct7->iter_count));
  DoubleMat w$2(n,1);
  DoubleMat PARAM_tmp6$1(ct7->j$2,n);
  DoubleMat h$1(ct7->j$2,1);
  DoubleMat V$1(ct7->V$1_data);
  DoubleMat v$3(ct7->v$3_data);
  DoubleMat A$0(ct7->A$0_data);
  
  V$1()(span::all,ct7->j$2 - 1)  =  v$3() ;
  w$2()  =  A$0() * v$3() ;
  /*PARAM_tmp6$1()  =  trans(V$1()(span::all,span(1 - 1,ct7->j$2 - 1))) ;
  h$1()  =  PARAM_tmp6$1() * w$2() ;*/
  h$1()  =  trans(V$1()(span::all,span(1 - 1,ct7->j$2 - 1))) * w$2() ;
  ct7->j$3  =  ct7->j$2 + 1 ;
  ct7->PARAM_tmp4$3  =  ct7->j$3 <= ct7->k$0 ;
  ct7->task_version.reserve(8);
  if(ct7->PARAM_tmp4$3){
    ct7->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct7->j$3} ;
    GetTask<CTask_task7>(7,ct7->task_version)->Add_V$1(V$1);
  }
  ct7->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct7->j$2} ;
  GetTask<CTask_task8>(8,ct7->task_version)->Add_V$1(V$1);
  if(!ct7->PARAM_tmp4$3){
    ct7->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct7->j$3} ;
    GetTask<CTask_END>(0,ct7->task_version)->Add_V$1(V$1);
  }
  ct7->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct7->j$2} ;
  GetTask<CTask_task8>(8,ct7->task_version)->Add_w$2(w$2);
  if(ct7->PARAM_tmp4$3){
    ct7->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct7->j$3} ;
    GetTask<CTask_task7>(7,ct7->task_version)->Add_A$0(A$0);
  }
  ct7->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct7->j$2} ;
  GetTask<CTask_task8>(8,ct7->task_version)->Add_h$1(h$1);
  if(ct7->PARAM_tmp4$3){
    ct7->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct7->j$3} ;
    GetTask<CTask_task7>(7,ct7->task_version)->Add_j$2(ct7->j$3);
  }
  if(ct7->PARAM_tmp4$3){
    ct7->task_version  =  {0,0,0,0,0,0,0,(unsigned int) ct7->j$3} ;
    GetTask<CTask_task7>(7,ct7->task_version)->Add_k$0(ct7->k$0);
  }
  //root_dec_ref_count();
  //return  NULL;
}
