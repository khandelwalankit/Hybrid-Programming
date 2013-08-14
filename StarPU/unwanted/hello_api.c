/* This example is taken from StarPU examples. 
 * This example will demonstrate 
 *  1) task creation in StarPU
 *  2) Codelet definition
 *  3) callback function once task has been executed. Need to find where and why
 *  this is useful??
 *  4) Submission of task
 *  This program uses one CPU
 *
 *  */ 

#include <stdio.h>
#include <starpu.h>
#include <stdint.h>

#define FPRINTF(ofile, fmt, ...) do { if (!getenv("STARPU_SSILENT")) {fprintf(ofile, fmt, ## __VA_ARGS__); }} while(0)

// Defining Callback Function
void callback_func(void *callback_arg)
{
  FPRINTF(stdout, "Callback function got argument %p\n", callback_arg);
}
// Defining params to pass for readonly data 
struct params
{
  int i;
  float f;
};

// Defining CPU function to be executed
void cpu_func(void *buffers[], void *cl_arg)
{
  struct params *params = (struct params *) cl_arg;
  FPRINTF(stdout, "Hello World (params = {%i, %f})\n", params->i, params->f);
}

// Defining Main Function
int main (int argc, char **argv)
{
  struct starpu_codelet cl;
  struct starpu_task *task;
  struct params params = {1,2.0f};
  int ret;

  /* intialize Starpu: passing null argument means  default configuration for scheduling policies and number of processor/accelerators(??)
   */
  ret=starpu_init(NULL);
  if (ret == -ENODEV)
    return 77;
  STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

  /*Creating a non-blocking task. This task is not submitted to scheduler till starpu_task_submit function is called */

  task = starpu_task_create();
  starpu_codelet_init(&cl);

  //This codelet is only implemented on a CPU and its CPU implementation is function "cpu_func"
  cl.cpu_funcs[0] = cpu_func;
  
  // We do not need to pass any data or the data need not be manipulated by DSM
  cl.nbuffers = 0;
  cl.name="hello";

  // Task using codelet
  task->cl = &cl;

  /*Passing buffers that are not managed by DSM. Since these buffers are not managed by DSM the buffer bassed as cl_arg is not valid synchronization
  * medium This data is readonly*/
  task->cl_arg = &params;
  task->cl_arg_size = sizeof(params);

  //Call back function once the task is executed - will be called on CPU
  task->callback_func = callback_func;
  task->callback_arg = (void*) (uintptr_t) 0x42;

  //starpu_task_submit will be blocking call
  task->synchronous = 1;

  //submit the task to StarPU
  ret = starpu_task_submit(task);
  if (ret == -ENODEV) goto enodev;
  STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");

  //once all task are executed
  starpu_shutdown();
  return 0;

enodev:
  starpu_shutdown();
  return 77;
}
