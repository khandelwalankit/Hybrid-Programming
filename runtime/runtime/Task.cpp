//#include <tbb/task.h>
#include "Task.h"

tbb::spin_mutex printMutex;
tbb::spin_mutex mapSearchMutex;
void execute(__attribute__((unused))void *descr[], __attribute__((unused))void *arg);
namespace Tasks{
  TaskMap task_map;
  //tbb::task* root                   = NULL;
  
  /*double get_task_total_time(){
    return Task::cumulative_task_time;
  }*/
}
