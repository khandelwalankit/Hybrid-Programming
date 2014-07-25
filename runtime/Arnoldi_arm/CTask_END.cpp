#include "CTask.h"
using namespace std;
using namespace Tasks;
//tbb::tick_count CTask_END::start       = tbb::tick_count();
//tbb::tick_count CTask_END::end         = tbb::tick_count();
double CTask_END::start   = starpu_timing_now();
double CTask_END::end = starpu_timing_now();
double CTask_END::cumulative_task_time = 0; 
void execute_END(__attribute__((unused))void *descr[], void *arg)
{
  CTask_END *cend = static_cast<CTask_END *>(arg);
  final_outputs.push_back(cend->V$1_data);
  final_outputs.push_back(cend->H$1_data);
  final_outputs.push_back(cend->f$2_data);
  //Tasks::root->decrement_ref_count();
  //return  NULL;
}
